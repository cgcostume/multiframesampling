#pragma once

#include <map>

#include <glm/vec3.hpp>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

enum class TextureType
{
    Diffuse,
    Specular,
    Emissive,
    Bump
};

enum class BumpType
{
    None,
    Height,
    Normal
};

class Material
{
public:
    using TextureMap = std::map<TextureType, globjects::ref_ptr<globjects::Texture>>;

    Material() = default;

    glm::vec3 specularColor;

    const TextureMap& textureMap() const;
    void addTexture(TextureType type, globjects::ref_ptr<globjects::Texture> texture);
    bool hasTexture(TextureType type) const;

protected:
    TextureMap m_textureMap;
};
