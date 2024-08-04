#version 460 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
	FragColor = texture(tex, texCoords);

	float depthValue = texture(tex, texCoords).r;
	FragColor = vec4(vec3(depthValue), 1.0);

	// FragColor = vec4(texCoords.xy, 0.0, 1.0);
}
