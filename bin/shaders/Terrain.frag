#version 460 core

in vec3 normal;
in vec3 fragPos;
in vec3 color;

out vec4 FragColor;

struct Light {
    vec4 posOrDir;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float intensity;

};

uniform Light light;
uniform vec3 viewPos;

void main() {
    float ambientStrength = 0.65;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(normal);
    vec3 lightDir = light.posOrDir.w == 0 ? normalize(-vec3(light.posOrDir)) : normalize(vec3(light.posOrDir) - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.2;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular = specularStrength * spec * light.color;

    float attenuation;
    if(light.posOrDir.w == 0) {
        attenuation = 1.0; // Directional light
    } else {
        float distance = length(vec3(light.posOrDir) - fragPos);
        attenuation = 1.0 / (light.constant + light.linear * distance +
            light.quadratic * (distance * distance)); // Point light
    }

    vec3 result = (diffuse + ambient + specular) * color * attenuation * light.intensity;

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(norm, 1.0);
}
