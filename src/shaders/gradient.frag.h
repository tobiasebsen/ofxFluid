#define STRINGIFY(A) #A
static const char* fragmentCompositeShader = STRINGIFY(

uniform float depthNear;
uniform float depthFar;

uniform vec4 colorNear;
uniform vec4 colorFar;

uniform vec2 clipPosToEye;

uniform sampler2D tPosition;

vec3 viewportToEyeSpace(vec2 coord, float eyeZ) {
	vec2 uv = (coord*2.0 - vec2(1.0))*clipPosToEye;
	return vec3(-uv*eyeZ, eyeZ);
}

void main() {
	// sample from depth texture
	// discard fragment if depth is zero (frament not defined)
	vec4 eye = texture2D(tPosition, gl_TexCoord[0].xy);
	if (eye.z == 0.0) discard;
	gl_FragDepth = eye.w;


	vec3 eyePos = viewportToEyeSpace(gl_TexCoord[0].xy, eye.z);
	vec4 worldPos = gl_ModelViewMatrixInverse * vec4(eyePos, 1.0);

	float gradient = (worldPos.z - depthNear) / (depthFar - depthNear);

	gl_FragColor = vec4(mix(colorNear.xyz, colorFar.xyz, gradient), 1.0);
}

);