#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aOffset;

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 viewProjection;

void main() {
	gl_Position = viewProjection * model * vec4(aPos * 2 + aOffset, 1.0);
	// gl_Position = vec4(aPos, 1.0);

	// normal = mat3(transpose(inverse(model))) * aNormal;
	normal = vec3(0.0, 1.0, 0.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	texCoord = aTex;
}