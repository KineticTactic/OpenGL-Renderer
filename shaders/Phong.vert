#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 viewProjection;

void main() {
    gl_Position = viewProjection * model * vec4(aPos, 1.0);

    normal = mat3(transpose(inverse(model))) * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0));
}