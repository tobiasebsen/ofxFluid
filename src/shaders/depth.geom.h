#define STRINGIFY(A) #A
static const char* geometryEllipsoidDepthShader =
"#version 120\n"
"#extension GL_EXT_geometry_shader4 : enable\n"
STRINGIFY(

void main() {
	vec3 pos = gl_PositionIn[0].xyz;
	vec4 bounds = gl_TexCoordIn[0][0];
	vec4 ndcPos = gl_TexCoordIn[0][5];

	// frustrum culling
	const float ndcBound = 1.0;
	if (ndcPos.x < -ndcBound) return;
	if (ndcPos.x > ndcBound) return;
	if (ndcPos.y < -ndcBound) return;
	if (ndcPos.y > ndcBound) return;

	float xmin = bounds.x;
	float xmax = bounds.y;
	float ymin = bounds.z;
	float ymax = bounds.w;

	// inv quadric transform
	gl_TexCoord[0] = gl_TexCoordIn[0][1];
	gl_TexCoord[1] = gl_TexCoordIn[0][2];
	gl_TexCoord[2] = gl_TexCoordIn[0][3];
	gl_TexCoord[3] = gl_TexCoordIn[0][4];

	gl_Position = vec4(xmin, ymax, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(xmin, ymin, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(xmax, ymax, 0.0, 1.0);
	EmitVertex();

	gl_Position = vec4(xmax, ymin, 0.0, 1.0);
	EmitVertex();
}

);