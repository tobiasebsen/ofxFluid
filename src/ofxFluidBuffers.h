#pragma once

#include "ofMain.h"

class ofxFluidBuffers {
public:
	void allocate(size_t numParticles);

	void update();

	void draw(int first, int total) const;
	void draw(int first = 0) const;

	size_t getNumParticles() const;
	GLuint getPositionsBuffer() const;
	GLuint getAnisotropyBuffer(int index) const;
	GLuint getAnisotropyAttrib(int index) const;

private:
	size_t numParticles;
	ofBufferObject positionsObj;
	ofBufferObject anisotropyObj[3];
	ofVbo positionsVbo;
	ofVbo anisotropyVbo[3];
};