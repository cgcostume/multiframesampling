#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shaders/common/shadowmapping.glsl>
#include </data/shaders/common/fragment_discard.glsl>
#include </data/shaders/common/random.glsl>

in vec3 v_normal;
in vec3 v_worldCoord;
in vec3 v_uv;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

uniform samplerCube shadowmap;
uniform sampler2D masksTexture;
uniform sampler3D noiseTexture;
uniform sampler2D diffuseTexture;
uniform bool useDiffuseTexture;
uniform float masksOffset;
uniform float alpha;
uniform vec3 worldLightPos;

const vec3 ambientLight = vec3(0.25);

void main()
{
    if (fragmentDiscard(alpha, 0, masksTexture, rand(noiseTexture, v_worldCoord, masksOffset)))
    {
        discard;
    }

    float shadowFactor = omnishadowmapComparisonVSM(shadowmap, v_worldCoord, worldLightPos);

    if (useDiffuseTexture)
    {
        outColor = texture(diffuseTexture, v_uv.xy).rgb;
    }
    else
    {
        outColor = vec3(1.0);
    }

    outColor *= shadowFactor;
    outColor += ambientLight;

    outNormal = v_normal;
}
