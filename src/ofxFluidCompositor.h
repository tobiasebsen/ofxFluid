#pragma once

#include "ofMain.h"
#include "ofxFluidRenderer.h"

class ofxFluidCompositor {
public:
	virtual void composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer) = 0;
	virtual ofParameterGroup & getParameters() = 0;
};
