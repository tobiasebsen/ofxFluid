#include "ofxFluidThickness.h"
#include "shaders/thickness.vert.h"
#include "shaders/thickness.frag.h"

//-----------------------------------------------------------------------------------------------------
void ofxFluidThickness::allocate(int width, int height) {

	ofFbo::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.useDepth = true;
	settings.internalformat = GL_LUMINANCE32F_ARB;
	settings.textureTarget = GL_TEXTURE_2D;
	thicknessFbo.allocate(settings);

	if (!pointThicknessProgram.isLoaded()) {
		pointThicknessProgram.setupShaderFromSource(GL_VERTEX_SHADER, vertexPointDepthShader);
		pointThicknessProgram.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentPointThicknessShader);
		pointThicknessProgram.linkProgram();
	}
}

void ofxFluidThickness::render(ofxFluidScene & scene, const ofxFluidBuffers & buffers, int first, int total) {

	if (!thicknessFbo.isAllocated() || thicknessFbo.getWidth() != scene.screenWidth || thicknessFbo.getHeight() != scene.screenHeight)
		allocate(scene.screenWidth, scene.screenHeight);

	thicknessFbo.begin(false);

	ofClear(0);

	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);

	const float screenWidth = thicknessFbo.getWidth();
	const float screenAspect = screenWidth / thicknessFbo.getHeight();;
	//const float fov = camera.getFov() * DEG_TO_RAD;

	pointThicknessProgram.begin();
	pointThicknessProgram.setUniform1f("pointRadius", thicknessScale * scene.radius);
	pointThicknessProgram.setUniform1f("pointScale", screenWidth / screenAspect * (1.0f / (tanf(DEG_TO_RAD * scene.fov*0.5f))));

	buffers.draw(first, total);

	pointThicknessProgram.end();

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	thicknessFbo.end();
}

const ofTexture & ofxFluidThickness::getTexture() const {
	return thicknessFbo.getTexture();
}

ofParameterGroup & ofxFluidThickness::getParameters() {
	if (params.size() == 0) {
		params.setName("THICKNESS");
		params.add(thicknessScale.set("Thickness scale", 4.f, 0.f, 10.f));
	}
	return params;
}
