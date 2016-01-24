#include "MultiFramePipeline.h"

#include "ModelLoadingStage.h"
#include "RasterizationStage.h"
#include "PostprocessingStage.h"
#include "FrameAccumulationStage.h"


MultiFramePipeline::MultiFramePipeline()
: gloperate::AbstractPipeline("MultiframeSampling")
, resourceManager(nullptr)
, modelFilename("data/transparency_scene.obj")
, multiFrameCount(64)
{
    auto modelLoadingStage = new ModelLoadingStage();
    auto rasterizationStage = new RasterizationStage();
    auto postprocessingStage = new PostprocessingStage();
    auto frameAccumulationStage = new FrameAccumulationStage();

    modelLoadingStage->resourceManager = resourceManager;
    modelLoadingStage->modelFilename = modelFilename;

    rasterizationStage->projection = projection;
    rasterizationStage->camera = camera;
    rasterizationStage->viewport = viewport;
    rasterizationStage->multiFrameCount = multiFrameCount;
    rasterizationStage->drawables = modelLoadingStage->drawables;

    postprocessingStage->viewport = viewport;
    postprocessingStage->camera = camera;
    postprocessingStage->projection = projection;
    postprocessingStage->color = rasterizationStage->color;
    postprocessingStage->normal = rasterizationStage->normal;
    postprocessingStage->depth = rasterizationStage->depth;

    frameAccumulationStage->viewport = viewport;
    frameAccumulationStage->currentFrame = rasterizationStage->currentFrame;
    frameAccumulationStage->frame = postprocessingStage->postprocessedFrame;
    frameAccumulationStage->depth = rasterizationStage->depth;

    addStages(modelLoadingStage, rasterizationStage, postprocessingStage, frameAccumulationStage);
}
