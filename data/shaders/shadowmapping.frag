#version 330
#extension GL_ARB_shading_language_include : require

#include </data/shaders/common/fragment_discard.glsl>
#include </data/shaders/common/random.glsl>

in vec3 g_worldCoord;

uniform vec3 lightWorldPos;

layout (location = 0) out vec3 out_dist;

uniform float alpha;

void main()
{
    float dist = length(g_worldCoord - lightWorldPos);
    float dx = dFdx(dist);
    float dy = dFdy(dist);

    out_dist = vec3(dist, dist * dist + 0.25 * (dx*dx + dy*dy), alpha);
}
