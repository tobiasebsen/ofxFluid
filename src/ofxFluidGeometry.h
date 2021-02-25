#pragma once

#include "ofMain.h"
#include "ofxFluidScene.h"

class ofxFluidGeometry {
public:

	void allocate(int width, int height);

	void render(ofxFluidScene & scene, const ofTexture & inputTex);

	const ofTexture & getPositionTexture() const;
	const ofTexture & getNormalTexture() const;

private:
	ofFbo geometryFbo;
	ofShader geometryProgram;
};