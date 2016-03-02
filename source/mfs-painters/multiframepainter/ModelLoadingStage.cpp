#include "ModelLoadingStage.h"

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/primitives/PolygonalGeometry.h>
#include <gloperate/primitives/Scene.h>
#include <gloperate/resources/ResourceManager.h>
#include <gloperate/base/make_unique.hpp>

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
}

ModelLoadingStage::ModelLoadingStage()
{
    addInput("resouceManager", resourceManager);
    addInput("preset", preset);

    addOutput("drawables", drawables);
    addOutput("presetInformation", presetInformation);
    addOutput("textureMap", textureMap);
}

void ModelLoadingStage::process()
{
    auto modelFilename = getFilename(preset.data());
    presetInformation.data() = getPresetInformation(preset.data());

    drawables.data() = PolygonalDrawables{};
    textureMap.data() = IdTextureMap{};

    auto scene = resourceManager.data()->load<gloperate::Scene>(modelFilename);

    for (auto mesh : scene->meshes())
    {
        drawables->push_back(make_unique<gloperate::PolygonalDrawable>(*mesh));
    }

    auto dir = getDirectory(modelFilename);

    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

    for (auto& materialPair : scene->materials())
    {
        auto id = materialPair.first;
        auto& filename = materialPair.second;
        replace(filename, "\\", "/");

        globjects::ref_ptr<globjects::Texture> tex = resourceManager.data()->load<globjects::Texture>(dir + "/" + filename);

        if (tex)
        {
            textureMap.data()[id] = tex;
            textureMap.data()[id]->setParameter(GL_TEXTURE_WRAP_R, GL_REPEAT);
            textureMap.data()[id]->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            textureMap.data()[id]->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            textureMap.data()[id]->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            textureMap.data()[id]->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            textureMap.data()[id]->setParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
            textureMap.data()[id]->generateMipmap();
        }
    }

    invalidateOutputs();
}

PresetInformation ModelLoadingStage::getPresetInformation(Preset preset)
{
    static const std::map<Preset, PresetInformation> conversion {
        //                          camera eye              camera center          near;far         light position          light radius     ground height
        { Preset::Imrod,          { { -10.0, 31.2, 10.65 }, { 30, 5.5, -30.0 },    { 0.3, 500.0 },  { 0, 40, 0 },           1.0f,            0.0f } },
        { Preset::CrytekSponza,   { { -1300, 250, -23 },    { 0.9, -1.9, -2.1 },   { 5.0, 5000.0 }, { 450, 270, -30 },      15.0f,          -10.0f } },
        { Preset::DabrovicSponza, { { -10.0, 12.6, 0.9 },   { 3.2, 0.28, -1.82 },  { 0.3, 50.0 },   { 0, 18, 0 },           1.0f,            0.0f } },
        { Preset::Jakobi,         { { 0.39, 0.49, -0.63 },  { 0.05, -0.04, -0.1 }, { 0.02, 8.0 },   { -0.4, 1.2, -0.7 },    0.05f,          -0.12f } },
        { Preset::Megacity,       { { 0.26, 0.23, -0.35 },  { 0.14, 0.0, -0.14 },  { 0.02, 8.0 },   { -0.4, 1.2, -1.5 },    0.05f,          -0.12f } },
        { Preset::Mitusba,        { { 0.2, 3.7, 4.3 },      { 0.16, 0.07, -1.25 }, { 0.3, 60.0 },   { 10, 20, 0 },          0.7f,            0.0f } },
        { Preset::Transparency,   { { -1.9, 4.2, 4.6 },     { -0.06, 0.02, 0.56 }, { 0.3, 60.0 },   { 0, 5, 0 },            0.1f,           -1.4f } }
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
