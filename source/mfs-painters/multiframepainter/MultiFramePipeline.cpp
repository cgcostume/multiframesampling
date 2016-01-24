#include "MultiFramePipeline.h"

#include "ModelLoadingStage.h"
#include "RasterizationStage.h"
#include "FrameAccumulationStage.h"


MultiFramePipeline::MultiFramePipeline()
: gloperate::AbstractPipeline("MultiframeSampling")
, resourceManager(nullptr)
, modelFilename("data/transparency_scene.obj")
, multiFrameCount(64)
{
    auto modelLoadingStage = new ModelLoadingStage();
    auto rasterizationStage = new RasterizationStage();
    auto frameAccumulationStage = new FrameAccumulationStage();

    modelLoadingStage->resourceManager = resourceManager;
    modelLoadingStage->modelFilename = modelFilename;

    rasterizationStage->projection = projection;
    rasterizationStage->camera = camera;
    rasterizationStage->viewport = viewport;
    rasterizationStage->multiFrameCount = multiFrameCount;
    rasterizationStage->drawables = modelLoadingStage->drawables;

    frameAccumulationStage->viewport = viewport;
    frameAccumulationStage->currentFrame = rasterizationStage->currentFrame;
    frameAccumulationStage->frame = rasterizationStage->color;
    frameAccumulationStage->depth = rasterizationStage->depth;

    addStages(modelLoadingStage, rasterizationStage, frameAccumulationStage);
}
