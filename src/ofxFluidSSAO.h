#pragma once

#include "ofMain.h"
#include "ofxFluidCompositor.h"

class ofxFluidSSAO : public ofxFluidCompositor {
public:
	void init();
	void composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer);
	ofParameterGroup & getParameters();

private:
	std::vector<ofVec3f> ssaoKernel;
	ofTexture noiseTex;
	ofShader ssaoProgram;

	ofParameter<ofColor> diffuse;
	ofParameter<float> radius;
	ofParameter<float> bias;
	ofParameterGroup params;
};