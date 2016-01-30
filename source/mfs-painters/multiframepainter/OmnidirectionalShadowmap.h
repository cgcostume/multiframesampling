#pragma once

#include "TypeDefinitions.h"

#include <vector>

#include <glm/fwd.hpp>

#include <globjects/base/ref_ptr.h>

namespace globjects
{
    class Program;
    class Framebuffer;
    class Texture;
}

namespace gloperate
{
    class VertexDrawable;
}


class OmnidirectionalShadowmap
{
public:
    OmnidirectionalShadowmap();

    void render(const glm::vec3 &eye, const PolygonalDrawables& drawables);
    void setBlurSize(int blurSize);

    globjects::Texture * distanceTexture();

protected:
    void setupFbo(globjects::Framebuffer * fbo, globjects::Texture * colorBuffer, globjects::Texture * depthBuffer, int size);

protected:
    int m_blurSize;
    globjects::ref_ptr<gloperate::VertexDrawable> m_cube;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    globjects::ref_ptr<globjects::Framebuffer> m_fboFaces;
    globjects::ref_ptr<globjects::Texture> m_colorTexture;
    globjects::ref_ptr<globjects::Texture> m_depthTexture;

    globjects::ref_ptr<globjects::Framebuffer> m_blurredFboTemp;
    globjects::ref_ptr<globjects::Texture> m_colorTextureBlurTemp;

    globjects::ref_ptr<globjects::Framebuffer> m_blurredFbo;
    globjects::ref_ptr<globjects::Texture> m_colorTextureBlur;

    globjects::ref_ptr<globjects::Program> m_shadowmapProgram;
    globjects::ref_ptr<globjects::Program> m_blurProgram;
};
