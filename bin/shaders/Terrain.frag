#version 460 core

in vec3 normal;
in vec3 fragPos;
in vec3 color;

out vec4 FragColor;

struct Light {
    vec3 pos;
    vec3 color;
    float constant;
    float linear;
    float quadratic;

};

uniform Light light;
uniform vec3 viewPos;

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

    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    vec3 result = (diffuse + ambient + specular) * color * attenuation;

    FragColor = vec4(result, 1.0);
}
