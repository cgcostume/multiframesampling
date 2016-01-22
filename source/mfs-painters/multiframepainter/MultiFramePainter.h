#pragma once

#include <gloperate/pipeline/PipelinePainter.h>

#include "MultiFramePipeline.h"


namespace gloperate 
{
    class ResourceManager;
    class AbstractTargetFramebufferCapability;
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;
}


class MultiFramePainter : public gloperate::PipelinePainter
{
public:
    MultiFramePainter(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo);
    virtual ~MultiFramePainter();


protected:
    virtual void onInitialize() override;
    virtual void onPaint() override;


protected:
    MultiFramePipeline m_pipeline;

    /* Capabilities */
    gloperate::AbstractTargetFramebufferCapability * m_targetFramebufferCapability;
    gloperate::AbstractViewportCapability * m_viewportCapability;
    gloperate::AbstractPerspectiveProjectionCapability * m_projectionCapability;
    gloperate::AbstractCameraCapability * m_cameraCapability;
};
