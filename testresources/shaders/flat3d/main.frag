#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 camPos;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;


void main() {
    outColor = texture(tex, fragPos);
}