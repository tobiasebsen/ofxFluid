#pragma once

#include "ofxFluidCompositor.h"

class ofxFluidPhong : public ofxFluidCompositor {
public:
	void init();
	void composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer);
	ofParameterGroup & getParameters();

private:
	ofShader phongProgram;
	ofParameter<ofColor> ambientColor;
	ofParameter<ofColor> diffuseColor;
	ofParameter<float> shininess;
	ofParameterGroup params;
};