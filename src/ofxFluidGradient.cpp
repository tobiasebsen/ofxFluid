#include "ofxFluidGradient.h"
#include "shaders\pass.vert.h"
#include "shaders\gradient.frag.h"

void ofxFluidGradient::init() {

	if (!gradientProgram.isLoaded()) {
		gradientProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
		gradientProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentCompositeShader);
		gradientProgram.linkProgram();
	}
}

void ofxFluidGradient::composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer) {

	if (!gradientProgram.isLoaded())
		init();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	gradientProgram.begin();

	gradientProgram.setUniform1f("depthNear", depthNear);
	gradientProgram.setUniform1f("depthFar", depthFar);
	gradientProgram.setUniform4f("colorNear", ofxFluidScene::colorToVec(colorNear));
	gradientProgram.setUniform4f("colorFar", ofxFluidScene::colorToVec(colorFar));
	gradientProgram.setUniform2f("invTexScale", 1.0f / scene.screenWidth, 1.0f / scene.screenHeight);

	gradientProgram.setUniformTexture("tPosition", renderer.getPositionTexture(), 0);

	scene.begin();
	scene.drawQuad(renderer.getPositionTexture());
	scene.end();

	gradientProgram.end();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
}

ofParameterGroup & ofxFluidGradient::getParameters() {
	if (params.size() == 0) {
		params.setName("GRADIENT");
		params.add(depthNear.set("Depth near", 2, -10, 10));
		params.add(depthFar.set("Depth far", -2, -10, 10));
		params.add(colorNear.set("Color near", ofColor::white));
		params.add(colorFar.set("Color far", ofColor::black));
	}
	return params;
}
