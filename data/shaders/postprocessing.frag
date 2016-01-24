#version 330

in vec2 v_uv;

out vec3 outColor;

uniform sampler2D colorSampler;
uniform sampler2D normalSampler;
uniform sampler2D depthSampler;
uniform sampler1D ssaoKernelSampler;
uniform sampler2D ssaoNoiseSampler;

uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 projectionInverseMatrix;
uniform float farZ;
uniform vec2 screenSize;
uniform vec4 samplerSizes;

const float radius = 1.0;

// returns values in [nearZ:farZ]
float linearDepth(const in vec2 uv)
{
    float d = texture(depthSampler, uv, 0).x;
    return projectionMatrix[3][2] / (d + projectionMatrix[2][2]);
}

mat3 noised(const in vec3 normal, in vec2 uv)
{
    uv *= screenSize * samplerSizes[3];

    vec3 random = texture(ssaoNoiseSampler, uv).xyz;

    // orientation matrix
    vec3 t = normalize(random - normal * dot(random, normal));
    vec3 b = cross(normal, t);

    return mat3(t, b, normal);
}

void main()
{
    float d = linearDepth(v_uv);

    if (d > farZ)
        outColor = texture(colorSampler, v_uv).rgb;

    vec4 eye = (projectionInverseMatrix * vec4(2.0*(v_uv - vec2(0.5)), 1.0, 1.0));
    eye.xyz /= eye.w;
    eye.xyz /= farZ;
    // eye has a z of -1 here

    vec3 origin = eye.xyz * d;

    vec3 screenspaceNormal = normalMatrix * texture(normalSampler, v_uv, 0).xyz;

    // randomized orientation matrix for hemisphere based on face normal
    mat3 m = noised(screenspaceNormal, v_uv);

    float ao = 0.0;

    for (float i = 0.0; i < samplerSizes[0]; ++i)
    {
        vec3 s = m * texture(ssaoKernelSampler, i * samplerSizes[1]).xyz;

        s *= 2.0 * radius;
        s += origin;

        vec4 s_offset = projectionMatrix * vec4(s, 1.0);
        s_offset.xyz /= s_offset.w;

        s_offset.xy = s_offset.xy * 0.5 + 0.5;

        float sd = -linearDepth(s_offset.xy);

        float ndcRangeCheck = 1.0 - float(any(greaterThan(s_offset.xyz, vec3(1.0))) || any(lessThan(s_offset.xyz, vec3(0.0))));
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(-origin.z + sd));
        ao += rangeCheck * ndcRangeCheck * float(sd > s.z);
    }

    float ssao = 1.0 - (ao * samplerSizes[1]);

    outColor = texture(colorSampler, v_uv).rgb * vec3(ssao);
}
