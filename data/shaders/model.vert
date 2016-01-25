#version 330

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;

out vec3 v_normal;

uniform mat4 mvp;
uniform vec2 ndcOffset;

void main()
{
    vec4 ndcVertex = mvp * vec4(a_vertex, 1.0);
    ndcVertex.xy += ndcOffset * ndcVertex.w;

    v_normal = a_normal;
    gl_Position = ndcVertex;
}
