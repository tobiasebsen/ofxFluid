#define STRINGIFY(A) #A
static const char* fragmentGeometryShader = STRINGIFY(

uniform sampler2D tex;
uniform vec2 invTexScale;
uniform vec2 clipPosToEye;

vec3 viewportToEyeSpace(vec2 coord, float eyeZ) {
	// find position at z=1 plane
	vec2 uv = (coord*2.0 - vec2(1.0))*clipPosToEye;
	return vec3(-uv*eyeZ, eyeZ);
}

void main() {

	float eyeZ = texture2D(tex, gl_TexCoord[0].xy).x;
	if (eyeZ == 0.0)
		discard;

	// reconstruct eye space pos from depth
	vec3 eyePos = viewportToEyeSpace(gl_TexCoord[0].xy, eyeZ);
	gl_FragData[0].xyz = eyePos;

	vec4 clipPos = gl_ProjectionMatrix*vec4(0.0, 0.0, eyeZ, 1.0);
	float d = clipPos.z / clipPos.w;
	gl_FragData[0].w = d * 0.5 + 0.5;;

	// finite difference approx for normals, can't take dFdx because
	// the one-sided difference is incorrect at shape boundaries
	vec3 zl = eyePos - viewportToEyeSpace(gl_TexCoord[0].xy - vec2(invTexScale.x, 0.0), texture2D(tex, gl_TexCoord[0].xy - vec2(invTexScale.x, 0.0)).x);
	vec3 zr = viewportToEyeSpace(gl_TexCoord[0].xy + vec2(invTexScale.x, 0.0), texture2D(tex, gl_TexCoord[0].xy + vec2(invTexScale.x, 0.0)).x) - eyePos;
	vec3 zt = viewportToEyeSpace(gl_TexCoord[0].xy + vec2(0.0, invTexScale.y), texture2D(tex, gl_TexCoord[0].xy + vec2(0.0, invTexScale.y)).x) - eyePos;
	vec3 zb = eyePos - viewportToEyeSpace(gl_TexCoord[0].xy - vec2(0.0, invTexScale.y), texture2D(tex, gl_TexCoord[0].xy - vec2(0.0, invTexScale.y)).x);

	vec3 dx = zl;
	vec3 dy = zt;

	if (abs(zr.z) < abs(zl.z))
		dx = zr;

	if (abs(zb.z) < abs(zt.z))
		dy = zb;

	vec3 n = normalize(cross(dx, dy));
	//vec3 v = normalize(eyePos);
	//vec3 h = normalize(v + l);

	gl_FragData[1].xyz = n;
	gl_FragData[1].w = 1.0;
}

);