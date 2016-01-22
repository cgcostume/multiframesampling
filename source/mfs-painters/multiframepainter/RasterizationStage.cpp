#include "RasterizationStage.h"

#include <glbinding/gl/gl.h>

#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>

#include <gloperate/painter/AbstractPerspectiveProjectionCapability.h>
#include <gloperate/painter/AbstractViewportCapability.h>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/AbstractTargetFramebufferCapability.h>

#include <gloperate/primitives/PolygonalDrawable.h>

using namespace gl;


void RasterizationStage::initialize()
{
    setupGLState();

    camera.data()->setEye({ 1.0575, 0.7301, -1.59997 });
    camera.data()->setCenter({ -0.618056, -0.782045, 1.98035 });

    m_program = new globjects::Program();
    m_program->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shaders/standard.vert"),
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaders/standard.frag")
    );
}

void RasterizationStage::process()
{
    render();
}

void RasterizationStage::render()
{
    glViewport(viewport.data()->x(),
               viewport.data()->y(),
               viewport.data()->width(),
               viewport.data()->height());

    //targetFBO.data()->framebuffer()->bind();
    m_program->use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->setUniform("mvp", projection.data()->projection() * camera.data()->view());

    model.data()->draw();

    m_program->release();
    //globjects::Framebuffer::unbind();
}

void RasterizationStage::setupGLState()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}
