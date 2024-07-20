#version 460 core

in vec3 normal;
in vec3 fragPos;
in vec3 color;
in vec4 fragPosLightSpace;

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
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // perform perspective divide to convert into NDC
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

void main() {
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * light.color;

    vec3 norm = normalize(normal);
    vec3 lightDir = light.posOrDir.w == 0 ? normalize(-vec3(light.posOrDir)) : normalize(vec3(light.posOrDir) - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.1;
    vec3 viewDist = viewPos - fragPos;
    vec3 viewDir = normalize(viewDist);
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

    // vec3 result = (diffuse + ambient + specular) * color * attenuation * light.intensity;

        // calculate shadow
    float shadow = ShadowCalculation(fragPosLightSpace, norm, lightDir);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color * attenuation * light.intensity;    

    // Fog
//     float fog_maxdist = 5000;
//     float fog_mindist = 1000;
//     vec3 fog_colour = vec3(0.8);

// // Calculate fog
//     float dist = length(viewDist);
//     float fog_factor = (fog_maxdist - dist) /
//         (fog_maxdist - fog_mindist);
//     fog_factor = clamp(fog_factor, 0.0, 1.0);
    // result = mix(fog_colour, result, fog_factor);

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(vec3(shadow), 1.0);
    // FragColor = vec4(norm, 1.0);
}
