#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aOffset;

out vec3 normal;
out vec2 texCoord;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 viewProjection;

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
	// float scale = rand(vec2(gl_InstanceID, gl_InstanceID)) * 1 + 2.0;

	gl_Position = viewProjection * model * vec4(aPos + aOffset, 1.0);
	// gl_Position = viewProjection * model * vec4(aPos * scale + aOffset + vec3(0, scale / 2, 0), 1.0);
	// gl_Position = vec4(aPos, 1.0);

	// normal = mat3(transpose(inverse(model))) * aNormal;
	normal = vec3(0.0, 1.0, 0.0);
	fragPos = vec3(model * vec4(aPos, 1.0));
	texCoord = aTex;
}