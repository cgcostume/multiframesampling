#include "OmnidirectionalShadowmap.h"

#include "GroundPlane.h"

#include <limits>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>

#include <globjects/Shader.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/primitives/VertexDrawable.h>
#include <gloperate/primitives/PolygonalDrawable.h>

using namespace gl;

namespace
{
    const int size = 512;

    const float cameraNearPlane = 0.1f;
    const float cameraFarPlane = 4.0f;

    const glm::vec3 viewDirs[] = { glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1) };
    const glm::vec3 ups[] = { glm::vec3(0, -1, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0), glm::vec3(0, -1, 0) };

    const std::vector<glm::vec3> cubeVertices = {
        glm::vec3(-1.0, -1.0,  1.0),
        glm::vec3(1.0, -1.0,  1.0),
        glm::vec3(-1.0,  1.0,  1.0),
        glm::vec3(1.0,  1.0,  1.0),
        glm::vec3(-1.0, -1.0, -1.0),
        glm::vec3(1.0, -1.0, -1.0),
        glm::vec3(-1.0,  1.0, -1.0),
        glm::vec3(1.0,  1.0, -1.0)
    };

    const std::vector<glm::vec3> cubeData = {
        // degenerate triangle at start allows for reversing the cube triangle strip, in order to change the winding
        cubeVertices[1], cubeVertices[1], cubeVertices[0],
        cubeVertices[4], cubeVertices[2], cubeVertices[6],
        cubeVertices[7], cubeVertices[4], cubeVertices[5],
        cubeVertices[1], cubeVertices[7], cubeVertices[3],
        cubeVertices[2], cubeVertices[1], cubeVertices[0]
    };
}

OmnidirectionalShadowmap::OmnidirectionalShadowmap()
: m_blurSize(3)
{
    m_shadowmapProgram = new globjects::Program();
    m_shadowmapProgram->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shaders/shadowmapping.vert"),
        globjects::Shader::fromFile(GL_GEOMETRY_SHADER, "data/shaders/shadowmapping.geom"),
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaders/shadowmapping.frag"));

    m_blurProgram = new globjects::Program();
    m_blurProgram->attach(
        globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shaders/cubemapblur.vert"),
        globjects::Shader::fromFile(GL_GEOMETRY_SHADER, "data/shaders/cubemapblur.geom"),
        globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shaders/cubemapblur.frag"));

    m_fbo = new globjects::Framebuffer();
    m_colorTexture = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);
    m_depthTexture = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);
    setupFbo(m_fbo, m_colorTexture, m_depthTexture, size);

    m_blurredFbo = new globjects::Framebuffer();
    m_colorTextureBlur = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);
    setupFbo(m_blurredFbo, m_colorTextureBlur, nullptr, size);

    m_blurredFboTemp = new globjects::Framebuffer();
    m_colorTextureBlurTemp = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);
    setupFbo(m_blurredFboTemp, m_colorTextureBlurTemp, nullptr, size);

    m_cube = new gloperate::VertexDrawable(
        cubeData,
        gl::GL_TRIANGLE_STRIP);

    m_cube->setFormats({ gloperate::Format(3, GL_FLOAT, 0) });
    m_cube->bindAttributes({ 0 });
    m_cube->enableAll();
}

void OmnidirectionalShadowmap::setupFbo(globjects::Framebuffer * fbo, globjects::Texture * colorBuffer, globjects::Texture * depthBuffer, int size)
{
    colorBuffer->bind();
    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(GL_RG32F), size, size, 0, GL_RG, GL_FLOAT, nullptr);
    }
    colorBuffer->unbind();
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, colorBuffer);

    if (depthBuffer)
    {
        depthBuffer->bind();
        for (int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<GLint>(GL_DEPTH_COMPONENT), size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
        depthBuffer->unbind();
        fbo->attachTexture(GL_DEPTH_ATTACHMENT, depthBuffer);
    }

    fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
    fbo->checkStatus();
    fbo->unbind();
}

void OmnidirectionalShadowmap::render(const glm::vec3 &eye, const PolygonalDrawables& drawables, const GroundPlane& groundPlane)
{
    auto getTransforms = [](const glm::vec3 &eye) -> std::vector<glm::mat4>
    {
        std::vector<glm::mat4> transforms(6);

        for (int i = 0; i < 6; ++i)
        {
            auto curViewDir = viewDirs[i] + eye;
            auto projection = glm::perspective(glm::radians(90.0f), 1.0f, cameraNearPlane, cameraFarPlane);
            auto view = glm::lookAt(eye, curViewDir, ups[i]);
            transforms[i] = projection * view;
        }

        return std::move(transforms);
    };

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glViewport(0, 0, size, size);

    m_fbo->bind();
    m_fbo->clear(GL_DEPTH_BUFFER_BIT);
    m_fbo->clearBuffer(GL_COLOR, 0, glm::vec4(std::numeric_limits<float>::max()));
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);

    m_shadowmapProgram->use();
    m_shadowmapProgram->setUniform("transforms", getTransforms(eye));
    m_shadowmapProgram->setUniform("lightWorldPos", eye);
    groundPlane.draw(m_shadowmapProgram);

    m_shadowmapProgram->use();
    for (const auto & drawable : drawables)
    { 
        drawable->draw();
    }
    m_shadowmapProgram->release();

    m_blurProgram->setUniform("transforms", getTransforms(glm::vec3(0.0f)));
    m_blurProgram->setUniform("sizeFactor", 1.0f / size);
    m_blurProgram->setUniform("kernelsize", m_blurSize);
    m_blurProgram->setUniform("shadowMap", 0);

    // blur x pass
    m_blurProgram->use();

    m_blurredFboTemp->bind();
    m_colorTexture->bindActive(GL_TEXTURE0);
    m_blurProgram->setUniform("direction", glm::vec2(1.0f, 0.0f));

    m_cube->draw();

    // blur y pass
    m_blurredFbo->bind();
    m_colorTextureBlurTemp->bindActive(GL_TEXTURE0);
    m_blurProgram->setUniform("direction", glm::vec2(0.0f, 1.0f));
    
    m_cube->draw();

    m_colorTextureBlurTemp->unbindActive(GL_TEXTURE0);
    m_blurProgram->release();
    m_blurredFbo->unbind();
}

void OmnidirectionalShadowmap::setBlurSize(int blurSize)
{
    m_blurSize = blurSize;
}

globjects::Texture * OmnidirectionalShadowmap::distanceTexture()
{
    return m_colorTextureBlur;
}
