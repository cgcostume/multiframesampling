#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shaders/common/shadowmapping.glsl>

in vec3 v_normal;
in vec3 v_worldCoord;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

uniform samplerCube shadowmap;
uniform vec3 worldLightPos;

void main()
{
    float shadowFactor = omnishadowmapComparison(shadowmap, v_worldCoord, worldLightPos);

    outColor = v_normal * 0.5 + 0.5;
    outColor *= shadowFactor;

    outNormal = v_normal;
}
