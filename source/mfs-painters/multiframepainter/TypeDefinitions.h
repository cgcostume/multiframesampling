#pragma once

#include <vector>
#include <memory>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <globjects/base/ref_ptr.h>

namespace globjects
{
    class Texture;
}

namespace gloperate
{
    class PolygonalDrawable;
}

using PolygonalDrawables = std::vector<std::unique_ptr<gloperate::PolygonalDrawable>>;
using IdTextureMap = std::map<unsigned int, globjects::ref_ptr<globjects::Texture>>;

struct PresetInformation
{
    glm::vec3 camEye;
    glm::vec3 camCenter;
    glm::vec2 nearFar;
    glm::vec3 lightPosition;
    float lightMaxShift;
    float groundHeight;
};
