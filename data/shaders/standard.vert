#version 330

layout(location = 0) in vec3 a_vertex;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(a_vertex, 1.0);
}
