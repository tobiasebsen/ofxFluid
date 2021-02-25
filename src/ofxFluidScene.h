#pragma once

#include "ofMain.h"

class ofxFluidScene {
public:

	void setScreenSize(int width, int height);

	void setRadius(float radius);
	void setCamera(const ofCamera & camera);
	void setLight(const ofPoint & lightPos, const ofPoint & lightTarget, float lightFov);

	void begin();
	void end();

	void drawQuad(const ofTexture & tex, bool vFlip = false);
	void drawQuad(bool vFlip = false);

	static ofVec4f colorToVec(const ofColor & c);

	int screenWidth;
	int screenHeight;

	ofMatrix4x4 projectionMatrix;
	ofMatrix4x4 modelViewMatrix;
	float fov;
	float clipNear;
	float clipFar;
	bool vFlip;

	ofPoint lightPos;
	ofPoint lightTarget;
	float lightFov;

	float radius;
};