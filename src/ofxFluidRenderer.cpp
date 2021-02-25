#include "ofxFluidRenderer.h"
#include "shaders\pass.vert.h"
#include "shaders\gradient.frag.h"
#include "shaders\scale.frag.h"

//--------------------------------------------------------------------------------
bool ofxFluidRenderer::isEnabled() const {
	return enable;
}

//--------------------------------------------------------------------------------
const ofParameterGroup & ofxFluidRenderer::getParameters() {
	if (params.size() == 0) {
		params.setName("RENDERER");
		params.add(enable.set("Enable", true));
		params.add(thickness.getParameters());
		params.add(blur.getParameters());
	}
	return params;
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::render(ofxFluidScene & scene, const ofxFluidBuffers & buffers, int first, int total) {

	scene.begin();
	ofDisableAlphaBlending();

	thickness.render(scene, buffers, first, total);
	depth.render(scene, buffers, first, total);
	blur.render(scene, depth.getTexture(), thickness.getTexture());
	geometry.render(scene, blur.getTexture());

	scene.end();
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawThickness() {
	drawScaled(thickness.getTexture(), 0.f, 5.f);
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawDepth() {
	drawScaled(depth.getTexture(), -0.9f, -0.03f);
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawDepthBlur() {
	drawScaled(blur.getTexture(), -0.9f, -0.03f);
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawPositions() {
	drawScaled(geometry.getPositionTexture(), 1.0f, 0.5f);
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawNormals() {
	geometry.getNormalTexture().draw(0, 0);
}

//--------------------------------------------------------------------------------
const ofTexture & ofxFluidRenderer::getDepthTexture() const {
	return blur.getTexture();
}

//--------------------------------------------------------------------------------
const ofTexture & ofxFluidRenderer::getPositionTexture() const {
	return geometry.getPositionTexture();
}

//--------------------------------------------------------------------------------
const ofTexture & ofxFluidRenderer::getNormalTexture() const {
	return geometry.getNormalTexture();
}

//--------------------------------------------------------------------------------
void ofxFluidRenderer::drawScaled(const ofTexture & tex, float add, float multiply) {
	ofDisableAlphaBlending();
	if (tex.getTextureData().textureTarget == GL_TEXTURE_2D) {

		if (!scaleProgram.isLoaded()) {
			//scaleProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
			scaleProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentScaleShader);
			scaleProgram.linkProgram();
		}

		scaleProgram.begin();
		scaleProgram.setUniform1f("uAdd", add);
		scaleProgram.setUniform1f("uMultiply", multiply);
		tex.draw(0, 0);
		scaleProgram.end();
	}
	else {

		if (!scaleRectProgram.isLoaded()) {
			//scaleRectProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
			scaleRectProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentScaleRectShader);
			scaleRectProgram.linkProgram();
		}

		scaleRectProgram.begin();
		scaleRectProgram.setUniform1f("uAdd", add);
		scaleRectProgram.setUniform1f("uMultiply", multiply);
		tex.draw(0, 0);
		scaleRectProgram.end();
	}
}
