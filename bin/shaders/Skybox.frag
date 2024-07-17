#version 460 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, texCoords);
	// FragColor = vec4(texCoords, 1.0);
}