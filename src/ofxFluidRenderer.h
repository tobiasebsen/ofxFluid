#pragma once

#include "ofMain.h"
#include "ofxFluidBuffers.h"
#include "ofxFluidScene.h"
#include "ofxFluidThickness.h"
#include "ofxFluidDepth.h"
#include "ofxFluidBlur.h"
#include "ofxFluidGeometry.h"

class ofxFluidRenderer {
public:
	
	void render(ofxFluidScene & scene, const ofxFluidBuffers & buffers, int first, int total);

	void drawThickness();
	void drawDepth();
	void drawDepthBlur();
	void drawPositions();
	void drawNormals();

	const ofTexture & getDepthTexture() const;
	const ofTexture & getPositionTexture() const;
	const ofTexture & getNormalTexture() const;

	bool isEnabled() const;
	const ofParameterGroup & getParameters();

private:

	void drawScaled(const ofTexture & tex, float add, float multiply);


	ofParameter<bool> enable = { "Enable", true };
	ofParameterGroup params;

	ofxFluidThickness thickness;
	ofxFluidDepth depth;
	ofxFluidBlur blur;
	ofxFluidGeometry geometry;

	ofShader scaleProgram;
	ofShader scaleRectProgram;
};