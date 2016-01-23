#include "PostProcessingStage.h"

#include <glbinding/gl/gl.h>

#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/painter/AbstractViewportCapability.h>

using namespace gl;

PostProcessingStage::PostProcessingStage()
{
    addInput("viewport", viewport);
    addInput("color", color);
    addInput("depth", depth);
    addInput("normal", normal);
}

void PostProcessingStage::initialize()
{
    m_fbo = new globjects::Framebuffer();
    m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, color.data());
    m_fbo->attachTexture(GL_DEPTH_ATTACHMENT, depth.data());
}

void PostProcessingStage::process()
{
    auto rect = std::array<GLint, 4> {
        viewport.data()->x(),
        viewport.data()->y(),
        viewport.data()->width(),
        viewport.data()->height()
    };

    auto defaultFbo = globjects::Framebuffer::defaultFBO();
    m_fbo->blit(GL_COLOR_ATTACHMENT0, rect, defaultFbo, GL_FRONT_LEFT, rect, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
