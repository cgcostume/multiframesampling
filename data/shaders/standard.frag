#version 330

in vec3 v_normal;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outNormal;

void main()
{
    outColor = v_normal * 0.5 + 0.5;
    outNormal = v_normal;
}
