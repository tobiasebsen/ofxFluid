#define STRINGIFY(A) #A
static const char *fragmentEllipsoidDepthShader = "#version 120\n" STRINGIFY(

uniform vec3 invViewport;
uniform vec3 invProjection;

float Sign(float x) { return x < 0.0 ? -1.0 : 1.0; }

bool solveQuadratic(float a, float b, float c, out float minT, out float maxT) {
	if (a == 0.0 && b == 0.0) {
		minT = maxT = 0.0;
		return true;
	}

	float discriminant = b*b - 4.0*a*c;

	if (discriminant < 0.0) {
		return false;
	}

	float t = -0.5*(b + Sign(b)*sqrt(discriminant));
	minT = t / a;
	maxT = c / t;

	if (minT > maxT) {
		float tmp = minT;
		minT = maxT;
		maxT = tmp;
	}

	return true;
}

float sqr(float x) { return x*x; }

void main() {
	// transform from view space to parameter space
	mat4 invQuadric;
	invQuadric[0] = gl_TexCoord[0];
	invQuadric[1] = gl_TexCoord[1];
	invQuadric[2] = gl_TexCoord[2];
	invQuadric[3] = gl_TexCoord[3];

	vec4 ndcPos = vec4(gl_FragCoord.xy*invViewport.xy*vec2(2.0, 2.0) - vec2(1.0, 1.0), -1.0, 1.0);
	vec4 viewDir = gl_ProjectionMatrixInverse*ndcPos;

	// ray to parameter space
	vec4 dir = invQuadric*vec4(viewDir.xyz, 0.0);
	vec4 origin = invQuadric[3];

	// set up quadratric equation
	float a = sqr(dir.x) + sqr(dir.y) + sqr(dir.z);// - sqr(dir.w);
	float b = dir.x*origin.x + dir.y*origin.y + dir.z*origin.z - dir.w*origin.w;
	float c = sqr(origin.x) + sqr(origin.y) + sqr(origin.z) - sqr(origin.w);

	float minT;
	float maxT;

	if (solveQuadratic(a, 2.0*b, c, minT, maxT)) {
		vec3 eyePos = viewDir.xyz*minT;
		vec4 ndcPos = gl_ProjectionMatrix * vec4(eyePos, 1.0);
		ndcPos.z /= ndcPos.w;

		gl_FragColor = vec4(eyePos.z, 1.0, 1.0, 1.0);
		gl_FragDepth = ndcPos.z*0.5 + 0.5;

		return;
	}
	else
		discard;

	gl_FragColor = vec4(0.5, 0.0, 0.0, 1.0);
}

);