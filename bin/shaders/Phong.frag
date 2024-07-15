#version 460 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    float ambientStrength = 0.35;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;

    vec3 result = (diffuse + ambient + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}
