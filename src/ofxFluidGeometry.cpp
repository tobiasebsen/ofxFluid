#include "ofxFluidGeometry.h"
#include "shaders\pass.vert.h"
#include "shaders\geometry.frag.h"

void ofxFluidGeometry::allocate(int width, int height) {

	ofFbo::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.useDepth = true;
	settings.colorFormats.push_back(GL_RGBA32F_ARB);
	settings.colorFormats.push_back(GL_RGBA32F_ARB);
	settings.numColorbuffers = 2;
	settings.internalformat = GL_RGBA32F_ARB;
	settings.textureTarget = GL_TEXTURE_2D;
	settings.depthStencilAsTexture = false;

	geometryFbo.allocate(settings);

	if (!geometryProgram.isLoaded()) {
		geometryProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
		geometryProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentGeometryShader);
		geometryProgram.linkProgram();
	}
}

void ofxFluidGeometry::render(ofxFluidScene & scene, const ofTexture & depthTex) {

	if (!geometryFbo.isAllocated() || geometryFbo.getWidth() != scene.screenWidth || geometryFbo.getHeight() != scene.screenHeight)
		allocate(scene.screenWidth, scene.screenHeight);

	geometryFbo.begin(false);
	geometryFbo.activateAllDrawBuffers();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float screenWidth = geometryFbo.getWidth();
	const float screenAspect = screenWidth / geometryFbo.getHeight();
	//const float fov = camera.getFov() * DEG_TO_RAD;

	geometryProgram.begin();
	geometryProgram.setUniform2f("invTexScale", 1.0f / screenWidth, screenAspect / screenWidth);
	geometryProgram.setUniform2f("clipPosToEye", tanf(DEG_TO_RAD*scene.fov*0.5f)*screenAspect, tanf(DEG_TO_RAD*scene.fov*0.5f));
	geometryProgram.setUniformTexture("tex", depthTex, 0);

	scene.drawQuad(depthTex);

	geometryProgram.end();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	geometryFbo.end();
}

const ofTexture & ofxFluidGeometry::getPositionTexture() const {
	return geometryFbo.getTexture(0);
}

const ofTexture & ofxFluidGeometry::getNormalTexture() const {
	return geometryFbo.getTexture(1);
}
