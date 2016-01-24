#include "FrameAccumulationStage.h"

#include <glbinding/gl/gl.h>

#include <glm/vec4.hpp>

#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Framebuffer.h>

#include <gloperate/primitives/ScreenAlignedQuad.h>
#include <gloperate/painter/AbstractViewportCapability.h>

using namespace gl;

FrameAccumulationStage::FrameAccumulationStage()
{
    addInput("viewport", viewport);
    addInput("frame", frame);
    addInput("depth", depth);
    addInput("currentFrame", currentFrame);
}

void FrameAccumulationStage::initialize()
{
    m_accumulation = globjects::Texture::createDefault(GL_TEXTURE_2D);
    
    m_fbo = new globjects::Framebuffer();
    m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, m_accumulation);
    m_fbo->attachTexture(GL_DEPTH_ATTACHMENT, depth.data());

    m_screenAlignedQuad = new gloperate::ScreenAlignedQuad(
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaders/accumulation.frag")
    );
}

void FrameAccumulationStage::process()
{
    if (viewport.hasChanged())
    {
        resizeTexture(viewport.data()->width(), viewport.data()->height());
    }

    if (currentFrame.data() == 1)
    {
        m_fbo->clearBuffer(GL_COLOR, 0, glm::vec4(0.0f));
    }

    glDepthMask(GL_FALSE);
    m_fbo->bind();
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_accumulation->bindActive(0);
    frame.data()->bindActive(1);
    m_screenAlignedQuad->program()->setUniform("accumBuffer", 0);
    m_screenAlignedQuad->program()->setUniform("frameBuffer", 1);
    m_screenAlignedQuad->program()->setUniform("weight", 1.0f / currentFrame.data());

    m_screenAlignedQuad->draw();
    m_accumulation->unbindActive(0);
    frame.data()->unbindActive(1);

    m_fbo->unbind();
    glDepthMask(GL_TRUE);

    auto rect = std::array<GLint, 4> {
        viewport.data()->x(),
        viewport.data()->y(),
        viewport.data()->width(),
        viewport.data()->height()
    };

    auto defaultFbo = globjects::Framebuffer::defaultFBO();
    m_fbo->blit(GL_COLOR_ATTACHMENT0, rect, defaultFbo, GL_FRONT_LEFT, rect, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void FrameAccumulationStage::resizeTexture(int width, int height)
{
    m_accumulation->image2D(0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    m_fbo->printStatus(true);
}
