#define STRINGIFY(A) #A
const char *fragmentPointThicknessShader = STRINGIFY(

void main() {
	// calculate normal from texture coordinates
	vec3 normal;
	normal.xy = gl_TexCoord[0].xy*vec2(2.0, -2.0) + vec2(-1.0, 1.0);
	float mag = dot(normal.xy, normal.xy);
	if (mag > 1.0) discard;   // kill pixels outside circle
	normal.z = sqrt(1.0 - mag);

	gl_FragColor = vec4(normal.z*0.005);
}

);