#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shaders/common/shadowmapping.glsl>

uniform samplerCube shadowmap;
uniform vec3 worldLightPos;

in vec3 g_worldCoord;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outNormal;

const float ambientFactor = 0.25;
const vec3 color = vec3(1.0);

void main()
{
    outColor = color * omnishadowmapComparisonVSM(shadowmap, g_worldCoord, worldLightPos);
    outColor += ambientFactor * color;
    outColor = clamp(outColor, 0.0, 1.0);

    outNormal = vec3(0.0, 1.0, 0.0);
}
