#pragma once

#include "ofMain.h"
#include "ofxFluidBuffers.h"
#include "ofxFluidScene.h"

class ofxFluidThickness {
public:
	void allocate(int width, int height);

	void render(ofxFluidScene & scene, const ofxFluidBuffers & buffers, int first, int total);

	const ofTexture & getTexture() const;
	
	ofParameterGroup & getParameters();

private:
	ofFbo thicknessFbo;
	ofShader pointThicknessProgram;
	ofParameter<float> thicknessScale;
	ofParameterGroup params;
};