#pragma once

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>


namespace globjects
{
    class Framebuffer;
    class Texture;
}

namespace gloperate
{
    class AbstractViewportCapability;
}

class PostProcessingStage : public gloperate::AbstractStage
{
public:
    PostProcessingStage();

    virtual void initialize() override;

    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;

    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> depth;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> normal;


protected:
    virtual void process() override;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
};
