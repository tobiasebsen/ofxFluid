#include "ofxFluidSSAO.h"
#include "shaders\pass.vert.h"
#include "shaders\ssao.frag.h"

void ofxFluidSSAO::init() {

	ofFloatPixels ssaoNoise;
	ssaoNoise.allocate(4, 4, 4);
	for (unsigned int i = 0; i < 16; i++) {
		ssaoNoise.setColor(i, ofFloatColor(ofRandomf(), ofRandomf(), 1.0f));
	}
	noiseTex.allocate(4, 4, GL_RGBA32F);
	noiseTex.loadData(ssaoNoise);

	ssaoKernel.clear();
	for (unsigned int i = 0; i < 64; ++i) {
		ofVec3f sample(ofRandomf(), ofRandomf(), ofRandomf());
		sample = sample.normalized();
		sample *= ofRandomf();
		float scale = float(i) / 64.0;
		scale = ofLerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	if (!ssaoProgram.isLoaded()) {
		ssaoProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPassThroughShader);
		ssaoProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentCompositeShader);
		ssaoProgram.linkProgram();
	}
}

void ofxFluidSSAO::composite(ofxFluidScene & scene, const ofxFluidRenderer & renderer) {

	if (!ssaoProgram.isLoaded())
		init();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	ssaoProgram.begin();

	ssaoProgram.setUniform4f("diffuse", ofxFluidScene::colorToVec(diffuse));

	ssaoProgram.setUniform2f("noiseScale", scene.screenWidth / 4.f, scene.screenHeight / 4.f);
	ssaoProgram.setUniform1i("kernelSize", 64);
	ssaoProgram.setUniform1f("radius",radius);
	ssaoProgram.setUniform1f("bias", bias);
	ssaoProgram.setUniform3fv("samples", ssaoKernel.data()->getPtr(), ssaoKernel.size());

	ssaoProgram.setUniformTexture("tPosition", renderer.getPositionTexture(), 0);
	ssaoProgram.setUniformTexture("tNormal", renderer.getNormalTexture(), 1);
	ssaoProgram.setUniformTexture("tNoise", noiseTex, 2);

	scene.begin();
	scene.drawQuad(renderer.getPositionTexture());
	scene.end();

	ssaoProgram.end();

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

}

ofParameterGroup & ofxFluidSSAO::getParameters() {
	if (params.size() == 0) {
		params.setName("SSAO");
		params.add(radius.set("Radius", 0.5, 0, 1));
		params.add(bias.set("Bias", 0.025, 0, 0.1));
		params.add(diffuse.set("Diffuse", ofColor::white));
	}
	return params;
}
