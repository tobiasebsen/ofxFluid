#pragma once

#include "ofMain.h"
#include "ofxFluidBuffers.h"
#include "ofxFluidScene.h"

class ofxFluidBlur {
public:
	void allocate(int width, int height);

	void render(ofxFluidScene & scene, const ofTexture & inputTex, const ofTexture & thicknessTex);

	const ofTexture & getTexture() const;

	ofParameterGroup & getParameters();

private:
	ofFbo depthBlurFbo;
	ofShader depthBlurProgram;

	ofParameter<float> blurFalloff;
	ofParameterGroup params;
};