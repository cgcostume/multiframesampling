#include "RasterizationStage.h"

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/boolean.h>

#include <glm/gtc/random.hpp>

#include <globjects/Framebuffer.h>
#include <globjects/Texture.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractCameraCapability.h>

#include <gloperate/primitives/PolygonalDrawable.h>

using namespace gl;
using gloperate::make_unique;

namespace
{
    const auto lightPosition = glm::vec3(0.0f, 2.0f, 0.0f);
}

RasterizationStage::RasterizationStage()
{
    currentFrame.data() = 1;

    addInput("projection", projection);
    addInput("viewport", viewport);
    addInput("camera", camera);
    addInput("drawables", drawables);

    addOutput("color", color);
    addOutput("normal", normal);
    addOutput("depth", depth);
}

void RasterizationStage::initialize()
{
    setupGLState();

    m_shadowmap = make_unique<OmnidirectionalShadowmap>();
    m_groundPlane = make_unique<GroundPlane>(-1.5f);

    color.data() = globjects::Texture::createDefault(GL_TEXTURE_2D);
    normal.data() = globjects::Texture::createDefault(GL_TEXTURE_2D);
    depth.data() = globjects::Texture::createDefault(GL_TEXTURE_2D);

    m_fbo = new globjects::Framebuffer();
    m_fbo->attachTexture(GL_COLOR_ATTACHMENT0, color.data());
    m_fbo->attachTexture(GL_COLOR_ATTACHMENT1, normal.data());
    m_fbo->attachTexture(GL_DEPTH_ATTACHMENT, depth.data());

    camera.data()->setEye({ 1.0575, 0.7301, -1.59997 });
    camera.data()->setCenter({ -0.618056, -0.782045, 1.98035 });

    m_program = new globjects::Program();
    m_program->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shaders/model.vert"),
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaders/model.frag")
    );
}

void RasterizationStage::process()
{
    if (viewport.hasChanged())
    {
        resizeTextures(viewport.data()->width(), viewport.data()->height());
    }

    currentFrame.data() += 1;
    for (auto input : this->inputs())
    {
        if (input->hasChanged())
        {
            currentFrame.data() = 1;
            alwaysProcess(true);
        }
    }

    if (currentFrame.data() > multiFrameCount.data())
    {
        alwaysProcess(false);
        return;
    }

    render();

    invalidateOutputs();
}

void RasterizationStage::resizeTextures(int width, int height)
{
    color.data()->image2D(0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    normal.data()->image2D(0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    depth.data()->image2D(0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    m_fbo->printStatus(true);
}

void RasterizationStage::render()
{
    auto frameLightOffset = glm::circularRand(0.05f);
    auto frameLightPosition = lightPosition + glm::vec3(frameLightOffset.x, 0.0, frameLightOffset.y);

    m_shadowmap->render(frameLightPosition, drawables.data(), *m_groundPlane.get());

    glViewport(viewport.data()->x(),
               viewport.data()->y(),
               viewport.data()->width(),
               viewport.data()->height());

    m_fbo->bind();
    m_fbo->setDrawBuffers({
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    });

    m_fbo->clearBuffer(GL_COLOR, 0, glm::vec4(0.0f));
    m_fbo->clearBuffer(GL_COLOR, 1, glm::vec4(0.0f));
    m_fbo->clearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0.0f);

    m_program->use();

    // TODO: use glkernel
    auto range = 0.5f;
    auto subpixelSample = glm::vec2(
        glm::linearRand(-range, range),
        glm::linearRand(-range, range)
    );
    auto viewportSize = glm::vec2(viewport.data()->width(), viewport.data()->height());

    for (auto program : std::vector<globjects::Program*>{ m_program, m_groundPlane->program() })
    {
        program->setUniform("shadowmap", 0);
        program->setUniform("worldLightPos", frameLightPosition);

        program->setUniform("modelView", camera.data()->view());
        program->setUniform("projection", projection.data()->projection());

        // offset needs to be doubled, because ndc range is [-1;1] and not [0;1]
        program->setUniform("ndcOffset", 2.0f * subpixelSample / viewportSize);
        
        program->setUniform("cocPoint", glm::diskRand(0.0f));
        program->setUniform("focalDist", 3.f);
    }
    
    m_shadowmap->distanceTexture()->bindActive(0);

    for (auto& drawable : drawables.data())
    {
        drawable->draw();
    }

    m_program->release();

    m_groundPlane->draw();

    m_fbo->unbind();
}

void RasterizationStage::setupGLState()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
