#pragma once

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>

#include "TypeDefinitions.h"

namespace globjects
{
    class Framebuffer;
    class Texture;
}

namespace gloperate
{
    class AbstractViewportCapability;
    class AbstractPerspectiveProjectionCapability;
    class AbstractCameraCapability;

    class ScreenAlignedQuad;
}

class PostprocessingStage : public gloperate::AbstractStage
{
public:
    PostprocessingStage();

    virtual void initialize() override;

    gloperate::InputSlot<gloperate::AbstractPerspectiveProjectionCapability *> projection;
    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
    gloperate::InputSlot<gloperate::AbstractCameraCapability *> camera;
    gloperate::InputSlot<PresetInformation> presetInformation;
    gloperate::InputSlot<bool> useReflections;

    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> color;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> normal;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> depth;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> worldPos;
    gloperate::InputSlot<globjects::ref_ptr<globjects::Texture>> reflectMask;

    gloperate::Data<globjects::ref_ptr<globjects::Texture>> postprocessedFrame;

protected:
    virtual void process() override;

    void resizeTexture(int width, int height);

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<gloperate::ScreenAlignedQuad> m_screenAlignedQuad;

    globjects::ref_ptr<globjects::Texture> m_ssaoKernelTexture;
    globjects::ref_ptr<globjects::Texture> m_ssaoNoiseTexture;
};
