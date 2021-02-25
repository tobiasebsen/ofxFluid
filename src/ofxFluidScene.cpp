#include "ofxFluidScene.h"

void ofxFluidScene::setScreenSize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
}

void ofxFluidScene::setRadius(float radius) {
	this->radius = radius;
}

void ofxFluidScene::setCamera(const ofCamera & camera) {
	projectionMatrix = camera.getProjectionMatrix();
	modelViewMatrix = camera.getModelViewMatrix();
	fov = camera.getFov();
	clipNear = camera.getNearClip();
	clipFar = camera.getFarClip();
	vFlip = camera.isVFlipped();
}

void ofxFluidScene::setLight(const ofPoint & lightPos, const ofPoint & lightTarget, float lightFov) {
	this->lightPos = lightPos;
	this->lightTarget = lightTarget;
	this->lightFov = lightFov;
}

void ofxFluidScene::begin() {
	ofPushView();
	ofSetOrientation(OF_ORIENTATION_DEFAULT, vFlip);
	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix(projectionMatrix);
	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix(modelViewMatrix);
}

void ofxFluidScene::end() {
	ofPopView();
}

void ofxFluidScene::drawQuad(const ofTexture & tex, bool vFlip) {
	tex.bind();
	drawQuad(this->vFlip);
	tex.unbind();
}

void ofxFluidScene::drawQuad(bool vFlip) {

	static float vertices[][2] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
	static float texCoords[][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };
	static float texCoordsFlipped[][2] = { { 0, 1 },{ 1, 1 },{ 1, 0 },{ 0, 0 } };
	static GLuint indices[] = { 0, 1, 2, 2, 3, 0 };

	glColor3f(1.0f, 1.0f, 1.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, vFlip ? texCoordsFlipped : texCoords);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

ofVec4f ofxFluidScene::colorToVec(const ofColor & c) {
	return ofVec4f(c.r, c.g, c.b, c.a) / 255.f;
}
