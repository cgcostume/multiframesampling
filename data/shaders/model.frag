#version 330

in vec3 v_normal;
in vec3 v_worldCoord;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

uniform samplerCube shadowmap;
uniform vec3 worldLightPos;

float linstep(float low, float high, float v)
{
    return clamp((v-low)/(high-low), 0.0, 1.0);
}

float VSM(vec2 moments, float compare)
{
    float p = smoothstep(compare-0.001, compare, moments.x);
    float variance = max(moments.y - moments.x*moments.x, -0.001);
    float d = compare - moments.x;
    float p_max = linstep(0.2, 1.0, variance / (variance + d*d));
    return clamp(max(p, p_max), 0.0, 1.0);
}

float omnishadowmapComparison()
{
    vec3 lightDirection = v_worldCoord - worldLightPos;
    float dist = length(lightDirection);
    vec2 moments = texture(shadowmap, lightDirection).rg;
    // TODO: option to use standard shadow mapping
    //return float(dist <= moments.x + 0.001);
    return VSM(moments, dist);
}

void main()
{
    float shadowFactor = omnishadowmapComparison();

    outColor = v_normal * 0.5 + 0.5;
    outColor *= shadowFactor;

    outNormal = v_normal;
}
