#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shaders/common/shadowmapping.glsl>

uniform samplerCube shadowmap;
uniform vec3 worldLightPos;

in vec3 worldPos;

layout (location = 0) out vec3 out_color;
layout (location = 1) out vec3 out_normal;

void main()
{
    out_color = vec3(1.0) * omnishadowmapComparisonVSM(shadowmap, worldPos, worldLightPos);
    out_normal = vec3(0.0, 1.0, 0.0);
}
