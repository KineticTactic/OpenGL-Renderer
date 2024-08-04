#version 460 core

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D grassTex;

void main() {
	vec4 col = texture(grassTex, texCoord);
	if(col.a < 0.1)
		discard;
	FragColor = col;
}
