#version 460 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
	FragColor = vec4(objectColor, 1.0);
}
