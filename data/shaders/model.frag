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
uniform sampler2D bumpTexture;
uniform int bumpType;

uniform float masksOffset;
uniform float alpha;
uniform vec3 worldLightPos;

#define BUMP_HEIGHT 1
#define BUMP_NORMAL 2
const float ambientFactor = 0.25;

// taken from http://www.thetenthplanet.de/archives/1180
mat3 cotangent_frame(vec3 N, vec3 p, vec2 uv)
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);

    // solve the linear system
    vec3 dp2perp = cross(dp2, N);
    vec3 dp1perp = cross(N, dp1);
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt(max(dot(T,T), dot(B,B)));
    return mat3(T * invmax, B * invmax, N);
}

void main()
{
    if (fragmentDiscard(alpha, 0, masksTexture, rand(noiseTexture, v_worldCoord, masksOffset)))
    {
        discard;
    }

    vec2 uv = v_uv.xy;

    vec3 L = normalize(worldLightPos - v_worldCoord);
    vec3 N = normalize(v_normal);

    mat3 tbn = cotangent_frame(N, v_worldCoord, uv);

    float shadowFactor = omnishadowmapComparisonVSM(shadowmap, v_worldCoord, worldLightPos);

    vec3 color = vec3(1.0);
    if (useDiffuseTexture)
    {
        color = texture(diffuseTexture, uv).rgb;
    }

    if (bumpType == BUMP_HEIGHT)
    {
        float A = textureOffset(bumpTexture, uv, ivec2( 1, 0)).x;
        float B = textureOffset(bumpTexture, uv, ivec2(-1, 0)).x;
        float C = textureOffset(bumpTexture, uv, ivec2( 0, 1)).x;
        float D = textureOffset(bumpTexture, uv, ivec2( 0,-1)).x;

        vec3 normalBump = vec3(B-A, D-C, 0.1);
        normalBump = tbn * normalBump;
        N = normalize(normalBump);
    }
    else if (bumpType == BUMP_NORMAL)
    {
        vec3 normalSample = texture(bumpTexture, uv).rgb * 2.0 - 1.0;
        N = normalize(tbn * normalSample);
    }

    vec3 ambientColor = ambientFactor * color;
    vec3 diffuseColor = color * max(0.0, dot(N, L)) * shadowFactor;

    outColor = ambientColor + diffuseColor;
    outColor = clamp(outColor, 0.0, 1.0);

    outNormal = v_normal;
}
