#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 normal;
out vec3 fragPos;
out vec3 color;

uniform vec3 worldPos;
uniform mat4 viewProjection;

void main() {
    gl_Position = viewProjection * vec4(aPos + worldPos, 1.0);

    // normal = mat3(transpose(inverse(model))) * aNormal;
    // fragPos = vec3(model * vec4(aPos, 1.0));
    normal = aNormal;
    fragPos = aPos + worldPos;
    color = aColor;
}