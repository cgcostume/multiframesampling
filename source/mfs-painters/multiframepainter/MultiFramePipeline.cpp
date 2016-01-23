#include "MultiFramePipeline.h"

#include "ModelLoadingStage.h"
#include "RasterizationStage.h"
#include "PostProcessingStage.h"


MultiFramePipeline::MultiFramePipeline()
: gloperate::AbstractPipeline("MultiframeSampling")
, resourceManager(nullptr)
, modelFilename("data/transparency_scene.obj")
{
    auto modelLoadingStage = new ModelLoadingStage();
    auto rasterizationStage = new RasterizationStage();
    //auto postProcessingStage = new PostProcessingStage();

    modelLoadingStage->resourceManager = resourceManager;
    modelLoadingStage->modelFilename = modelFilename;

    rasterizationStage->projection = projection;
    rasterizationStage->camera = camera;
    rasterizationStage->viewport = viewport;
    rasterizationStage->targetFBO = targetFBO;
    rasterizationStage->model = modelLoadingStage->model;

    addStages(modelLoadingStage, rasterizationStage);//, postProcessingStage);
}


