#pragma once

#include <vector>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

namespace globjects
{
    class Program;
    class Texture;
    class Framebuffer;
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
    RasterizationStage();

    virtual void initialize() override;

    using PolygonalDrawables = std::vector<std::unique_ptr<gloperate::PolygonalDrawable>>;

    gloperate::InputSlot<gloperate::AbstractPerspectiveProjectionCapability *> projection;
    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<PolygonalDrawables> drawables;

    gloperate::Data<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> normal;
    gloperate::Data<globjects::ref_ptr<globjects::Texture>> depth;


protected:
    virtual void process() override;

    void resizeTextures(int width, int height);
    void setupGLState();
    void render();

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Program> m_program;
};
