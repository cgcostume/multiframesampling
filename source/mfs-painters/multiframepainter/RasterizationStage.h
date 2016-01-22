#pragma once

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

namespace globjects
{
    class Program;
}

namespace gloperate
{
    class AbstractPerspectiveProjectionCapability;
    class AbstractViewportCapability;
    class AbstractCameraCapability;
    class AbstractTargetFramebufferCapability;

    class PolygonalDrawable;
}

class RasterizationStage : public gloperate::AbstractStage
{
public:
    virtual void initialize() override;

    gloperate::InputSlot<gloperate::AbstractPerspectiveProjectionCapability *> projection;
    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<gloperate::AbstractTargetFramebufferCapability *> targetFBO;

    gloperate::InputSlot<gloperate::PolygonalDrawable *> model;

protected:
    virtual void process() override;

    void setupGLState();
    void render();

    globjects::ref_ptr<globjects::Program> m_program;
};
