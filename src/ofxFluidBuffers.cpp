#include "ofxFluidBuffers.h"

void ofxFluidBuffers::allocate(size_t numParticles) {
	this->numParticles = numParticles;
	
	positionsObj.allocate(sizeof(float) * 4 * numParticles, GL_DYNAMIC_DRAW);

	for (int i = 0; i < 3; i++) {
		anisotropyObj[i].allocate(sizeof(float) * 4 * numParticles, GL_DYNAMIC_DRAW);
	}
}

void ofxFluidBuffers::update() {
	positionsVbo.setVertexBuffer(positionsObj, 3, sizeof(float) * 4, 0);
	for (int i = 0; i < 3; i++) {
		anisotropyVbo[i].setVertexBuffer(anisotropyObj[i], 3, sizeof(float) * 4);
	}
}

void ofxFluidBuffers::draw(int first, int total) const {
	positionsVbo.draw(GL_POINTS, first, total);
}

void ofxFluidBuffers::draw(int first) const {
	draw(first, numParticles);
}

size_t ofxFluidBuffers::getNumParticles() const {
	return numParticles;
}

GLuint ofxFluidBuffers::getPositionsBuffer() const {
	return positionsObj.getId();
}

GLuint ofxFluidBuffers::getAnisotropyBuffer(int index) const {
	return anisotropyObj[index].getId();
}

GLuint ofxFluidBuffers::getAnisotropyAttrib(int index) const {
	return anisotropyVbo[index].getVertId();
}