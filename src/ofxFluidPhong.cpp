#include "ofxFluidPhong.h"
#include "shaders\pass.vert.h"
#include "shaders\phong.frag.h"

void ofxFluidPhong::init() {

	if (!phongProgram.isLoaded()) {
		phongProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
		phongProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentCompositeShader);
		phongProgram.linkProgram();
	}
}

void ofxFluidPhong::composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer) {

	if (!phongProgram.isLoaded())
		init();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	phongProgram.begin();

	ofVec3f lightDir = scene.lightTarget - scene.lightPos;
	lightDir.normalize();
	phongProgram.setUniform3f("lightDir", lightDir);
	phongProgram.setUniform4f("ambientColor", ofxFluidScene::colorToVec(ambientColor));
	phongProgram.setUniform4f("diffuseColor", ofxFluidScene::colorToVec(diffuseColor));
	phongProgram.setUniform1f("shininess", shininess);

	phongProgram.setUniformTexture("tPosition", renderer.getPositionTexture(), 0);
	phongProgram.setUniformTexture("tNormal", renderer.getNormalTexture(), 1);

	scene.begin();
	scene.drawQuad(renderer.getPositionTexture());
	scene.end();

	phongProgram.end();

	glDisable(GL_DEPTH_TEST);
}

ofParameterGroup & ofxFluidPhong::getParameters() {
	if (params.size() == 0) {
		params.setName("PHONG");
		params.add(ambientColor.set("Ambient", ofColor::black));
		params.add(diffuseColor.set("Diffuse", ofColor::red));
		params.add(shininess.set("Shininess", 32, 2, 256));
	}
	return params;
}
