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

// 0: integer hash
// 1: float hash (aliasing based)
#define METHOD 1

// 0: cubic
// 1: quintic
#define INTERPOLANT 1

#if METHOD==0
vec3 hash(ivec3 p)     // this hash is not production ready, please
{                        // replace this by something better
    ivec3 n = ivec3(p.x * 127 + p.y * 311 + p.z * 74, p.x * 269 + p.y * 183 + p.z * 246, p.x * 113 + p.y * 271 + p.z * 124);

	// 1D hash by Hugo Elias
    n = (n << 13) ^ n;
    n = n * (n * n * 15731 + 789221) + 1376312589;
    return -1.0 + 2.0 * vec3(n & ivec3(0x0fffffff)) / float(0x0fffffff);
}
#else
vec3 hash(vec3 p)      // this hash is not production ready, please
{                        // replace this by something better
    p = vec3(dot(p, vec3(127.1, 311.7, 74.7)), dot(p, vec3(269.5, 183.3, 246.1)), dot(p, vec3(113.5, 271.9, 124.6)));

    return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}
#endif

// return value noise (in x) and its derivatives (in yzw)
vec4 noised(in vec3 x) {
    // grid
    #if METHOD==0
    ivec3 i = ivec3(floor(x));
    #else
    vec3 i = floor(x);
    #endif
    vec3 f = fract(x);

    #if INTERPOLANT==1
    // quintic interpolant
    vec3 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
    vec3 du = 30.0 * f * f * (f * (f - 2.0) + 1.0);
    #else
    // cubic interpolant
    vec3 u = f * f * (3.0 - 2.0 * f);
    vec3 du = 6.0 * f * (1.0 - f);
    #endif    

    // gradients
    #if METHOD==0
    vec3 ga = hash(i + ivec3(0, 0, 0));
    vec3 gb = hash(i + ivec3(1, 0, 0));
    vec3 gc = hash(i + ivec3(0, 1, 0));
    vec3 gd = hash(i + ivec3(1, 1, 0));
    vec3 ge = hash(i + ivec3(0, 0, 1));
    vec3 gf = hash(i + ivec3(1, 0, 1));
    vec3 gg = hash(i + ivec3(0, 1, 1));
    vec3 gh = hash(i + ivec3(1, 1, 1));
    #else
    vec3 ga = hash(i + vec3(0.0, 0.0, 0.0));
    vec3 gb = hash(i + vec3(1.0, 0.0, 0.0));
    vec3 gc = hash(i + vec3(0.0, 1.0, 0.0));
    vec3 gd = hash(i + vec3(1.0, 1.0, 0.0));
    vec3 ge = hash(i + vec3(0.0, 0.0, 1.0));
    vec3 gf = hash(i + vec3(1.0, 0.0, 1.0));
    vec3 gg = hash(i + vec3(0.0, 1.0, 1.0));
    vec3 gh = hash(i + vec3(1.0, 1.0, 1.0));
    #endif

    // projections
    float va = dot(ga, f - vec3(0.0, 0.0, 0.0));
    float vb = dot(gb, f - vec3(1.0, 0.0, 0.0));
    float vc = dot(gc, f - vec3(0.0, 1.0, 0.0));
    float vd = dot(gd, f - vec3(1.0, 1.0, 0.0));
    float ve = dot(ge, f - vec3(0.0, 0.0, 1.0));
    float vf = dot(gf, f - vec3(1.0, 0.0, 1.0));
    float vg = dot(gg, f - vec3(0.0, 1.0, 1.0));
    float vh = dot(gh, f - vec3(1.0, 1.0, 1.0));

    // interpolations
    vec4 noise = vec4(va + u.x * (vb - va) + u.y * (vc - va) + u.z * (ve - va) + u.x * u.y * (va - vb - vc + vd) + u.y * u.z * (va - vc - ve + vg) + u.z * u.x * (va - vb - ve + vf) + (-va + vb + vc - vd + ve - vf - vg + vh) * u.x * u.y * u.z,    // value
    ga + u.x * (gb - ga) + u.y * (gc - ga) + u.z * (ge - ga) + u.x * u.y * (ga - gb - gc + gd) + u.y * u.z * (ga - gc - ge + gg) + u.z * u.x * (ga - gb - ge + gf) + (-ga + gb + gc - gd + ge - gf - gg + gh) * u.x * u.y * u.z +   // derivatives
        du * (vec3(vb, vc, ve) - va + u.yzx * vec3(va - vb - vc + vd, va - vc - ve + vg, va - vb - ve + vf) + u.zxy * vec3(va - vb - ve + vf, va - vb - vc + vd, va - vc - ve + vg) + u.yzx * u.zxy * (-va + vb + vc - vd + ve - vf - vg + vh)));
    noise.x = 0.5 + noise.x; // remap to [0,1]
    return noise;
}

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

    if(normalizedHeight == 0.0) {
        // water color
        color = vec3(0.1, 0.3, 0.8);
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
        rockyColor = vec3(51 / 255.0, 45 / 255.0, 48 / 255.0);
        float rockySteepness = 1 - (normalizedHeight - 1.5) / 20;
        float rockToSnowTransition = 0.05;
        if(normal.y < rockySteepness) {
            color = rockyColor;
        } else if(normal.y < rockySteepness + rockToSnowTransition) {
            color = mix(rockyColor, snowColor, (normal.y - rockySteepness) / rockToSnowTransition);
        } else {
            color = snowColor;
        }

    }

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

    float fogDensity = 0.0001;
    float fragDistance = length(viewDist);
    float fogFactor = 1.0 - exp(-fogDensity * fragDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Mix the texture color with the fog color based on the fog factor
    result = mix(result, vec3(191.0 / 255, 215.0 / 255, 227.0 / 255), fogFactor);

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(heightMapSample.r, heightMapSample.r, heightMapSample.r, 1.0);
}
