#include "ModelLoadingStage.h"

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
}

void ModelLoadingStage::process()
{
    std::string modelFilename;

    switch (preset.data())
    {
    case Preset::Imrod:
        modelFilename = "data/Imrod/Imrod.obj";
        break;
    case Preset::Jakobi:
        modelFilename = "data/jakobi/jakobikirchplatz4.obj";
        break;
    case Preset::CrytekSponza:
        modelFilename = "data/crytek-sponza/sponza.obj";
        break;
    case Preset::DabrovicSponza:
        modelFilename = "data/dabrovic-sponza/sponza.obj";
        break;
    case Preset::Megacity:
        modelFilename = "data/megacity/simple2.obj";
        break;
    case Preset::Mitusba:
        modelFilename = "data/mitsuba/mitsuba.obj";
        break;
    case Preset::Transparency:
        modelFilename = "data/transparency_scene.obj";
        break;
    default:
        break;
    }

    drawables.data() = PolygonalDrawables{};
    textureMap.data() = IdTextureMap{};

    auto scene = resourceManager.data()->load<gloperate::Scene>(modelFilename);

    for (auto mesh : scene->meshes())
    {
        drawables->push_back(make_unique<gloperate::PolygonalDrawable>(*mesh));
    }

    auto dir = getDirectory(modelFilename);

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
        }
    }

    invalidateOutputs();
}
