#version 460 core

// in vec3 normal;
in vec3 fragPos;
// in vec3 color;
in vec4 fragPosLightSpace;
in vec2 texCoord;

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
uniform sampler2D rockTex;
uniform sampler2D snowTex;
uniform sampler2D grassFieldTex;
uniform sampler2D heightMap;

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

float sum(vec3 v) {
    return v.x + v.y + v.z;
}

// Lifted from https://www.shadertoy.com/view/Xtl3zf
vec4 texture_UV(in sampler2D srcTexture, in vec2 x) {
    float k = texture(srcTexture, 0.0025 * x.xy).x; // cheap (cache friendly) lookup
    float l = k * 8.0;
    float f = fract(l);

    float ia = floor(l + 0.5); // suslik's method (see comments)
    float ib = floor(l);
    f = min(f, 1.0 - f) * 2.0;

    vec2 offa = sin(vec2(3.0, 7.0) * ia); // can replace with any other hash
    vec2 offb = sin(vec2(3.0, 7.0) * ib); // can replace with any other hash

    vec4 cola = texture(srcTexture, vec2(x.xy + offa));
    vec4 colb = texture(srcTexture, vec2(x.xy + offb));

    return mix(cola, colb, smoothstep(0.2, 0.8, f - 0.1 * sum(cola.xyz - colb.xyz)));
}

vec4 triplanarUV(vec3 pos, vec3 normal, sampler2D tex) {
    float _TRI_SCALE = 2000.0;
    vec4 dx = texture_UV(rockTex, vec2(pos.zy / _TRI_SCALE));
    vec4 dy = texture_UV(rockTex, vec2(pos.xz / _TRI_SCALE));
    vec4 dz = texture_UV(rockTex, vec2(pos.xy / _TRI_SCALE));

    vec3 weights = abs(normal.xyz);
    weights = weights / (weights.x + weights.y + weights.z);

    return dx * weights.x + dy * weights.y + dz * weights.z;
}

vec4 hash4(vec2 p) {
    return fract(sin(vec4(1.0 + dot(p, vec2(37.0, 17.0)), 2.0 + dot(p, vec2(11.0, 47.0)), 3.0 + dot(p, vec2(41.0, 29.0)), 4.0 + dot(p, vec2(23.0, 31.0)))) * 103.0);
}

vec4 textureNoTile(sampler2D samp, in vec2 uv) {
    ivec2 iuv = ivec2(floor(uv));
    vec2 fuv = fract(uv);

    // generate per-tile transform
    vec4 ofa = hash4(iuv + ivec2(0, 0));
    vec4 ofb = hash4(iuv + ivec2(1, 0));
    vec4 ofc = hash4(iuv + ivec2(0, 1));
    vec4 ofd = hash4(iuv + ivec2(1, 1));

    vec2 ddx = dFdx(uv);
    vec2 ddy = dFdy(uv);

    // transform per-tile uvs
    ofa.zw = sign(ofa.zw - 0.5);
    ofb.zw = sign(ofb.zw - 0.5);
    ofc.zw = sign(ofc.zw - 0.5);
    ofd.zw = sign(ofd.zw - 0.5);

    // uv's, and derivatives (for correct mipmapping)
    vec2 uva = uv * ofa.zw + ofa.xy, ddxa = ddx * ofa.zw, ddya = ddy * ofa.zw;
    vec2 uvb = uv * ofb.zw + ofb.xy, ddxb = ddx * ofb.zw, ddyb = ddy * ofb.zw;
    vec2 uvc = uv * ofc.zw + ofc.xy, ddxc = ddx * ofc.zw, ddyc = ddy * ofc.zw;
    vec2 uvd = uv * ofd.zw + ofd.xy, ddxd = ddx * ofd.zw, ddyd = ddy * ofd.zw;

    // fetch and blend
    vec2 b = smoothstep(0.25, 0.75, fuv);

    return mix(mix(textureGrad(samp, uva, ddxa, ddya), textureGrad(samp, uvb, ddxb, ddyb), b.x), mix(textureGrad(samp, uvc, ddxc, ddyc), textureGrad(samp, uvd, ddxd, ddyd), b.x), b.y);
}

vec3 getColorFromHeightAndNormal(float height, vec3 normal) {
    vec3 color;

    // Normalize height to the range [0, 1]
    float normalizedHeight = height / 256.0;
    // float normalizedHeight = height / 5.0;
    float steepness = exp(-0.8 * normal.y);

    if(normalizedHeight < 0.5) {
        // water color
        color = vec3(0.0, 0.0, 0.5);
    } else if(normalizedHeight < 2.5) {
        // plains color
        // color = vec3(0.39, 0.61, 0.2);
        color = textureNoTile(grassFieldTex, vec2(fragPos.x, fragPos.z) / 5.0).rgb;
    } else if(normalizedHeight < 3.5) {
        // rocky color
        color = vec3(0.37, 0.22, 0.16);
    } else {
        // snowy color
        vec3 snowColor = vec3(1.0);
        // vec3 snowColor = mix(vec3(1.0), textureNoTile(snowTex, vec2(fragPos.x, fragPos.z) / 20.0).rgb, 0.0);
        vec3 rockyColor = textureNoTile(rockTex, vec2(fragPos.x, fragPos.z) / 20.0).rgb;
        if(normal.y < 0.5) {
            color = rockyColor;
        } else if(normal.y < 0.5) {
            color = mix(rockyColor, snowColor, (normal.y - 0.4) / 0.1);
        } else {
            color = snowColor;
        }

    }

    // Blend between grass and rocky color based on steepness
    // color = mix(rockyColor, snowColor, weight);
    // color = texture(rockTex, vec2(fragPos.x, fragPos.z) / 1000.0).rgb;
    // color = triplanarUV(fragPos, normal, rockTex).rgb;
    return color;
}

void main() {
    vec4 heightMapSample = texture(heightMap, texCoord);
    vec3 normal = normalize(vec3(heightMapSample.y, 1.0, heightMapSample.z));

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * light.color;

    // vec3 norm = normalize(normal);
    vec3 lightDir = light.posOrDir.w == 0 ? normalize(-vec3(light.posOrDir)) : normalize(vec3(light.posOrDir) - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    float specularStrength = 0.8;
    vec3 viewDist = viewPos - fragPos;
    vec3 viewDir = normalize(viewDist);
    vec3 reflectDir = reflect(-lightDir, normal);
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

    vec3 color = getColorFromHeightAndNormal(fragPos.y, normal);
    float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * color * attenuation * light.intensity;

    // float fogDensity = 0.00000001;
    // float fogFactor = exp(-fogDensity * abs(length(viewDist)));
    // vec3 fogColor = vec3(0.5, 0.5, 0.5);
    // result = mix(result, fogColor, fogFactor);

    // float fogStart = 1000.0;
    // float fogEnd = 4000.0;
    float fogDensity = 0.00015;
    float fragDistance = length(viewDist);
    float fogFactor = 1.0 - exp(-fogDensity * fragDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Mix the texture color with the fog color based on the fog factor
    result = mix(result, vec3(191.0 / 255, 215.0 / 255, 227.0 / 255), fogFactor);

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(vec3(heightMapSample) / 500.0, 1.0);
    // FragColor = vec4(norma.x, norma.y, norma.z, 1.0);

    // FragColor = vec4(vec3(shadow), 1.0);
    // FragColor = vec4(norm, 1.0);
}
