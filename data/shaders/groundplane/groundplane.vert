#version 330

layout(location = 0) in vec3 in_position;

uniform mat4 mvp;

out vec3 worldPos;

void main()
{
    gl_Position = mvp * vec4(in_position, 1.0);
    worldPos = in_position;
}
