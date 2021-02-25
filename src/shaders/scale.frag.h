#define STRINGIFY(A) #A
static const char* fragmentScaleShader = STRINGIFY(

uniform sampler2D tex;
uniform float uAdd;
uniform float uMultiply;

void main() {
	vec3 sample = texture2D(tex, gl_TexCoord[0].xy).xyz;
	gl_FragColor = vec4(vec3(uAdd) + sample * uMultiply, 1.0);
}

);

static const char* fragmentScaleRectShader = STRINGIFY(

uniform sampler2DRect tex;
uniform float uAdd;
uniform float uMultiply;

void main() {
	vec3 sample = texture2DRect(tex, gl_TexCoord[0].xy).xyz;
	gl_FragColor = vec4(vec3(uAdd) + sample * uMultiply, 1.0);
}

);