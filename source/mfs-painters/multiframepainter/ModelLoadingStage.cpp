#include "ModelLoadingStage.h"

#include <gloperate/primitives/PolygonalDrawable.h>
#include <gloperate/resources/ResourceManager.h>

#include <gloperate-assimp/AssimpMeshLoader.h>

void ModelLoadingStage::process()
{
    resourceManager.data()->addLoader(new gloperate_assimp::AssimpMeshLoader());
    model = resourceManager.data()->load<gloperate::PolygonalDrawable>(modelFilename.data());

    invalidateOutputs();
}
