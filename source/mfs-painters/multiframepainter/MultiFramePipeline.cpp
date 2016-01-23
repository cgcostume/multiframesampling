#include "MultiFramePipeline.h"

#include "ModelLoadingStage.h"
#include "RasterizationStage.h"
#include "PostProcessingStage.h"


MultiFramePipeline::MultiFramePipeline()
: gloperate::AbstractPipeline("MultiframeSampling")
, resourceManager(nullptr)
, modelFilename("data/transparency_scene.obj")
, multiFrameCount(64)
{
    auto modelLoadingStage = new ModelLoadingStage();
    auto rasterizationStage = new RasterizationStage();
    auto postProcessingStage = new PostProcessingStage();

    modelLoadingStage->resourceManager = resourceManager;
    modelLoadingStage->modelFilename = modelFilename;

    rasterizationStage->projection = projection;
    rasterizationStage->camera = camera;
    rasterizationStage->viewport = viewport;
    rasterizationStage->multiFrameCount = multiFrameCount;
    rasterizationStage->drawables = modelLoadingStage->drawables;

    postProcessingStage->viewport = viewport;
    postProcessingStage->currentFrame = rasterizationStage->currentFrame;
    postProcessingStage->color = rasterizationStage->color;
    postProcessingStage->normal = rasterizationStage->normal;
    postProcessingStage->depth = rasterizationStage->depth;

    addStages(modelLoadingStage, rasterizationStage, postProcessingStage);
}
