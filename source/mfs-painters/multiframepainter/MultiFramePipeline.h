#pragma once

#include <gloperate/pipeline/AbstractPipeline.h>
#include <gloperate/resources/ResourceManager.h>

namespace gloperate
{
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
    class AbstractViewportCapability;
}

class MultiFramePipeline : public gloperate::AbstractPipeline
{
public:
    MultiFramePipeline();

    gloperate::Data<gloperate::ResourceManager *> resourceManager;
    gloperate::Data<std::string> modelFilename;
    gloperate::Data<int> multiFrameCount;

    gloperate::Data<gloperate::AbstractViewportCapability *> viewport;
    gloperate::Data<gloperate::AbstractCameraCapability *> camera;
    gloperate::Data<gloperate::AbstractPerspectiveProjectionCapability *> projection;
};
