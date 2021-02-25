#include "ofxFluidDepth.h"
#include "shaders\depth.vert.h"
#include "shaders\depth.geom.h"
#include "shaders\depth.frag.h"

void ofxFluidDepth::allocate(int width, int height) {

	ofFbo::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.useDepth = true;
	settings.internalformat = GL_LUMINANCE32F_ARB;
	settings.textureTarget = GL_TEXTURE_RECTANGLE;
	settings.depthStencilAsTexture = false;
	depthFbo.allocate(settings);

	if (!ellipsoidDepthProgram.isLoaded()) {
		ellipsoidDepthProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexEllipsoidDepthShader);
		ellipsoidDepthProgram.setGeometryInputType(GL_POINTS);
		ellipsoidDepthProgram.setGeometryOutputType(GL_TRIANGLE_STRIP);
		ellipsoidDepthProgram.setGeometryOutputCount(4);
		ellipsoidDepthProgram.setupShaderFromSource(GL_GEOMETRY_SHADER, geometryEllipsoidDepthShader);
		ellipsoidDepthProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentEllipsoidDepthShader);
		ellipsoidDepthProgram.linkProgram();
	}
}

void ofxFluidDepth::render(ofxFluidScene & scene, const ofxFluidBuffers & buffers, int first, int total) {

	if (!depthFbo.isAllocated() || depthFbo.getWidth() != scene.screenWidth || depthFbo.getHeight() != scene.screenHeight)
		allocate(scene.screenWidth, scene.screenHeight);

	depthFbo.begin(false);

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float screenWidth = depthFbo.getWidth();
	const float screenAspect = screenWidth / depthFbo.getHeight();;
	//const float fov = camera.getFov() * DEG_TO_RAD;
	const float viewHeight = tanf(DEG_TO_RAD * scene.fov / 2.0f);

	ellipsoidDepthProgram.begin();
	ellipsoidDepthProgram.setUniform3f("invViewport", 1.0f / screenWidth, screenAspect / screenWidth, 1.0f);
	ellipsoidDepthProgram.setUniform3f("invProjection", screenAspect * viewHeight, viewHeight, 1.0f);

	int s1 = ellipsoidDepthProgram.getAttributeLocation("q1");
	glEnableVertexAttribArray(s1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.getAnisotropyAttrib(0));
	glVertexAttribPointer(s1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	int s2 = ellipsoidDepthProgram.getAttributeLocation("q2");
	glEnableVertexAttribArray(s2);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.getAnisotropyAttrib(1));
	glVertexAttribPointer(s2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	int s3 = ellipsoidDepthProgram.getAttributeLocation("q3");
	glEnableVertexAttribArray(s3);
	glBindBuffer(GL_ARRAY_BUFFER, buffers.getAnisotropyAttrib(2));
	glVertexAttribPointer(s3, 4, GL_FLOAT, GL_FALSE, 0, 0);

	buffers.draw(first, total);

	glDisableVertexAttribArray(s1);
	glDisableVertexAttribArray(s2);
	glDisableVertexAttribArray(s3);

	ellipsoidDepthProgram.end();

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	depthFbo.end();
}

const ofTexture & ofxFluidDepth::getTexture() const {
	return depthFbo.getTexture();
}

ofParameterGroup & ofxFluidDepth::getParameters() {
	if (params.size() == 0) {
		params.setName("DEPTH");
	}
	return params;
}
