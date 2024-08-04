#line 2 10
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
