#include "ofxFluidBlur.h"
#include "shaders\pass.vert.h"
#include "shaders\blur.frag.h"

void ofxFluidBlur::allocate(int width, int height) {

	ofFbo::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.useDepth = true;
	settings.internalformat = GL_LUMINANCE32F_ARB;
	settings.textureTarget = GL_TEXTURE_2D;
	settings.depthStencilAsTexture = false;

	depthBlurFbo.allocate(settings);

	if (!depthBlurProgram.isLoaded()) {
		depthBlurProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
		depthBlurProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentBlurDepthShader);
		depthBlurProgram.linkProgram();
	}
}

void ofxFluidBlur::render(ofxFluidScene & scene, const ofTexture & depthTex, const ofTexture & thicknessTex) {

	if (!depthBlurFbo.isAllocated() || depthBlurFbo.getWidth() != scene.screenWidth || depthBlurFbo.getHeight() != scene.screenHeight)
		allocate(scene.screenWidth, scene.screenHeight);

	depthBlurFbo.begin(false);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float screenWidth = depthBlurFbo.getWidth();
	const float screenAspect = screenWidth / depthBlurFbo.getHeight();
	//const float fov = camera.getFov() * DEG_TO_RAD;

	depthBlurProgram.begin();
	depthBlurProgram.setUniformTexture("depthTex", depthTex, 0);
	depthBlurProgram.setUniformTexture("thicknessTex", thicknessTex, 1);

	depthBlurProgram.setUniform1f("blurRadiusWorld", scene.radius*0.5f);
	depthBlurProgram.setUniform1f("blurScale", screenWidth / screenAspect * (1.0f / (tanf(DEG_TO_RAD * scene.fov*0.5f))));
	depthBlurProgram.setUniform2f("invTexScale", 1.0f / screenAspect, 1.0f);
	depthBlurProgram.setUniform1f("blurFalloff", blurFalloff);
	depthBlurProgram.setUniform1f("debug", 0);

	scene.drawQuad(depthTex);

	depthBlurProgram.end();

	glDepthMask(GL_TRUE);

	depthBlurFbo.end();
}

const ofTexture & ofxFluidBlur::getTexture() const {
	return depthBlurFbo.getTexture();
}

ofParameterGroup & ofxFluidBlur::getParameters() {
	if (params.size() == 0) {
		params.setName("BLUR");
		params.add(blurFalloff.set("Blur fall-off", 2.5, 0, 5));
	}
	return params;
}
