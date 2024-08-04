#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

out vec2 TexCoord;

uniform float chunkSize;

void main() {
    gl_Position = vec4(aPos * chunkSize, 1.0);
    TexCoord = aTex;
}