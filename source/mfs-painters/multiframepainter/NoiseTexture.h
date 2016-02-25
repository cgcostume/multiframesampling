#pragma once

#include <vector>

#include <glbinding/gl/types.h>

#include <globjects/base/ref_ptr.h>


namespace globjects
{
    class Texture;
}

class NoiseTexture
{
public:
    NoiseTexture(
        unsigned int inputDimensions, 
        unsigned int outputDimensions,
        gl::GLsizei textureSize = 64);

    void bindActive(unsigned int index) const;
    void unbindActive(unsigned int index) const;

protected:
    static gl::GLenum targetForDimensions(unsigned int dimensions);
    static gl::GLenum internalFormatForDimensions(unsigned int dimensions);
    static gl::GLenum formatForDimensions(unsigned int dimensions);

    static globjects::Texture * createTexture(
        unsigned int inputDimensions,
        unsigned int outputDimensions,
        gl::GLsizei textureSize);
    
    static std::vector<float> generateNoise(
        unsigned int inputDimensions,
        unsigned int outputDimensions,
        gl::GLsizei textureSize);

protected:
    globjects::ref_ptr<globjects::Texture> m_texture;
};
