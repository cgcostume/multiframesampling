#include "ModelLoadingStage.h"

#include <iostream>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/primitives/PolygonalGeometry.h>
#include <gloperate/primitives/Scene.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/base/make_unique.hpp>

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

using namespace gl;
using gloperate::make_unique;

namespace
{
    std::string getDirectory(const std::string& path)
    {
        size_t found = path.find_last_of("/\\");
        return(path.substr(0, found));
    }

    std::string & replace(std::string & subj, std::string old, std::string neu)
    {
        size_t uiui = subj.find(old);
        if (uiui != std::string::npos)
        {
            subj.erase(uiui, old.size());
            subj.insert(uiui, neu);
        }
        return subj;
    }

    auto textureTypes = {
        aiTextureType_DIFFUSE,
        aiTextureType_EMISSIVE,
        aiTextureType_SPECULAR,
        aiTextureType_HEIGHT
    };

    TextureType translateAssimpTextureType(aiTextureType aiTexType)
    {
        static const std::map<aiTextureType, TextureType> conversion {
            { aiTextureType_DIFFUSE, TextureType::Diffuse },
            { aiTextureType_SPECULAR, TextureType::Specular },
            { aiTextureType_EMISSIVE, TextureType::Emissive },
            { aiTextureType_HEIGHT, TextureType::Bump },
        };

        return conversion.at(aiTexType);
    }
}

ModelLoadingStage::ModelLoadingStage()
{
    addInput("resouceManager", resourceManager);
    addInput("preset", preset);

    addOutput("drawablesMap", drawablesMap);
    addOutput("presetInformation", presetInformation);
    addOutput("materialMap", materialMap);
}

void ModelLoadingStage::process()
{
    drawablesMap.data() = IdDrawablesMap{};
    materialMap.data() = IdMaterialMap{};

    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);

    auto modelFilename = getFilename(preset.data());
    auto dir = getDirectory(modelFilename);

    presetInformation.data() = getPresetInformation(preset.data());

    const aiScene* assimpScene = aiImportFile(
        modelFilename.c_str(),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType |
        aiProcess_GenNormals);

    if (!assimpScene)
        std::cout << aiGetErrorString();

    for (unsigned int m = 0; m < assimpScene->mNumMaterials; m++)
    {
        auto mat = loadMaterial(assimpScene->mMaterials[m], dir);
        materialMap.data()[m] = mat;
        drawablesMap.data()[m] = PolygonalDrawables{};
    }
    aiReleaseImport(assimpScene);

    auto scene = resourceManager.data()->load<gloperate::Scene>(modelFilename);
    for (auto mesh : scene->meshes())
    {
        auto& drawables = drawablesMap.data()[mesh->materialIndex()];
        drawables.push_back(make_unique<gloperate::PolygonalDrawable>(*mesh));
    }

    invalidateOutputs();
}

globjects::ref_ptr<globjects::Texture> ModelLoadingStage::loadTexture(const std::string& filename) const
{
    globjects::ref_ptr<globjects::Texture> tex = resourceManager.data()->load<globjects::Texture>(filename);
    tex->setParameter(GL_TEXTURE_WRAP_R, GL_REPEAT);
    tex->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    tex->setParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, m_maxAnisotropy);
    tex->generateMipmap();

    return tex;
}

Material ModelLoadingStage::loadMaterial(aiMaterial* aiMat, const std::string& directory) const
{
    Material material;

    for (aiTextureType aiTexType : textureTypes)
    {
        auto type = translateAssimpTextureType(aiTexType);

        aiString texPath;
        aiReturn ret = aiMat->GetTexture(aiTexType, 0, &texPath);

        if (ret != aiReturn_SUCCESS)
            continue;

        std::string texPathStd = std::string(texPath.C_Str());

        auto texture = loadTexture(directory + "/" + texPathStd);

        material.addTexture(type, texture);
    }

    return material;
}

PresetInformation ModelLoadingStage::getPresetInformation(Preset preset)
{
    static const std::map<Preset, PresetInformation> conversion {
        //                          camera eye              camera center          near;far         light position          light radius     ground height  bump mapping type
        { Preset::Imrod,          { { -10.0, 31.2, 10.65 }, { 30, 5.5, -30.0 },    { 0.3, 500.0 },  { 0, 52, 0 },           1.0f,            0.0f,          BumpType::Normal } },
        { Preset::CrytekSponza,   { { -1300, 250, -23 },    { 0.9, -1.9, -2.1 },   { 5.0, 5000.0 }, { 450, 270, -30 },      15.0f,          -10.0f,         BumpType::Height } },
        { Preset::DabrovicSponza, { { -10.0, 12.6, 0.9 },   { 3.2, 0.28, -1.82 },  { 0.3, 50.0 },   { 0, 18, 0 },           1.0f,            0.0f,          BumpType::Height } },
        { Preset::Jakobi,         { { 0.39, 0.49, -0.63 },  { 0.05, -0.04, -0.1 }, { 0.02, 8.0 },   { -0.4, 1.2, -0.7 },    0.05f,          -0.12f,         BumpType::None } },
        { Preset::Megacity,       { { 0.26, 0.23, -0.35 },  { 0.14, 0.0, -0.14 },  { 0.02, 8.0 },   { -0.4, 1.2, -1.5 },    0.05f,          -0.12f,         BumpType::None } },
        { Preset::Mitusba,        { { 0.2, 3.7, 4.3 },      { 0.16, 0.07, -1.25 }, { 0.3, 60.0 },   { 10, 20, 0 },          0.7f,            0.0f,          BumpType::None } },
        { Preset::Transparency,   { { -1.9, 4.2, 4.6 },     { -0.06, 0.02, 0.56 }, { 0.3, 60.0 },   { 0, 5, 0 },            0.1f,           -1.4f,          BumpType::None } }
    };

    return conversion.at(preset);
}

std::string ModelLoadingStage::getFilename(Preset preset)
{
    static const std::map<Preset, std::string> conversion {
        { Preset::Imrod, "data/Imrod/Imrod.obj" },
        { Preset::CrytekSponza, "data/crytek-sponza/sponza.obj" },
        { Preset::DabrovicSponza, "data/dabrovic-sponza/sponza.obj" },
        { Preset::Jakobi, "data/jakobi/jakobikirchplatz4.obj" },
        { Preset::Megacity, "data/megacity/simple2.obj" },
        { Preset::Mitusba, "data/mitsuba/mitsuba.obj" },
        { Preset::Transparency, "data/transparency_scene.obj" }
    };

    return conversion.at(preset);
}
