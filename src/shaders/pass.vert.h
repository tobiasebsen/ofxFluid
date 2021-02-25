#define STRINGIFY(A) #A
static const char* vertexPassThroughShader = STRINGIFY(

void main() {
	gl_Position = vec4(gl_Vertex.xyz, 1.0);
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

);