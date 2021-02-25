#define STRINGIFY(A) #A
static const char* fragmentCompositeShader = STRINGIFY(

uniform sampler2D tPosition;
uniform sampler2D tNormal;

uniform vec3 lightDir;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform float shininess;

void main() {
	// sample from depth texture
	// discard fragment if depth is zero (frament not defined)
	vec4 pos = texture2D(tPosition, gl_TexCoord[0].xy);
	if (pos.z == 0.0) discard;
	gl_FragDepth = pos.w;

	vec3 n = texture2D(tNormal, gl_TexCoord[0].xy).xyz;
	vec3 l = normalize((gl_ModelViewMatrix*vec4(lightDir.x, -lightDir.y, lightDir.z, 0.0)).xyz);
	float lambertian = max(dot(n, l), 0.0);

	vec3 v = -normalize(pos.xyz);
	vec3 h = normalize(v + l);
	float ln = dot(l, -n);// *attenuation;

	float specular = pow(max(ln, 0.0), shininess);
	vec3 color = ambientColor.xyz + diffuseColor.xyz * max(ln, 0.0) + vec3(specular);


	gl_FragColor.xyz = color;
	gl_FragColor.w = 1.0;
}

);