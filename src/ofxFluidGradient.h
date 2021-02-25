#pragma once

#include "ofMain.h"
#include "ofxFluidCompositor.h"

class ofxFluidGradient : public ofxFluidCompositor {
public:
	void init();
	void composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer);
	ofParameterGroup & getParameters();

private:
	ofShader gradientProgram;
	ofParameter<float> depthNear;
	ofParameter<float> depthFar;
	ofParameter<ofColor> colorNear;
	ofParameter<ofColor> colorFar;
	ofParameterGroup params;
};