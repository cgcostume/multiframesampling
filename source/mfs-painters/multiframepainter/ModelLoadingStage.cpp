#include "ModelLoadingStage.h"

#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/primitives/Scene.h>
#include <gloperate/resources/ResourceManager.h>

void ModelLoadingStage::process()
{
    auto x = resourceManager.data()->load<gloperate::Scene>(modelFilename.data());

    model = new gloperate::PolygonalDrawable(*x->meshes().front());

    invalidateOutputs();
}
