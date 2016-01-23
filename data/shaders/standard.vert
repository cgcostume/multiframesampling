#version 330

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;

out vec3 v_normal;

uniform mat4 mvp;

void main()
{

    v_normal = a_normal;
    gl_Position = mvp * vec4(a_vertex, 1.0);
}
