#include "ModelLoadingStage.h"

#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/primitives/Scene.h>
#include <gloperate/resources/ResourceManager.h>

ModelLoadingStage::ModelLoadingStage()
{
    addInput("resouceManager", resourceManager);
    addInput("modelFilename", modelFilename);

    addOutput("drawables", drawables);
}

void ModelLoadingStage::process()
{
    drawables.data() = PolygonalDrawables{};
    auto scene = resourceManager.data()->load<gloperate::Scene>(modelFilename.data());

    for (auto mesh : scene->meshes())
    {
        drawables->push_back(std::make_unique<gloperate::PolygonalDrawable>(*mesh));
    }

    invalidateOutputs();
}
