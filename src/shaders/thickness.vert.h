#define STRINGIFY(A) #A
const char *vertexPointDepthShader = STRINGIFY(

uniform float pointRadius;  // point size in world space
uniform float pointScale;   // scale to calculate size in pixels

void main() {
	// calculate window-space point size
	gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xyz, 1.0);
	gl_PointSize = pointScale * (pointRadius / gl_Position.w);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0);
}
);
