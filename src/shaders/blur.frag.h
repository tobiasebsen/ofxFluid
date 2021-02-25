#define STRINGIFY(A) #A
static const char* fragmentBlurDepthShader =
"#extension GL_ARB_texture_rectangle : enable\n"
STRINGIFY(

uniform sampler2DRect depthTex;
uniform sampler2D thicknessTex;
uniform float blurRadiusWorld;
uniform float blurScale;
uniform float blurFalloff;
uniform vec2 invTexScale;

uniform bool debug;

float sqr(float x) { return x*x; }

void main() {
	// eye-space depth of center sample
	float depth = texture2DRect(depthTex, gl_FragCoord.xy).x;
	float thickness = texture2D(thicknessTex, gl_TexCoord[0].xy).x;

	// hack: ENABLE_SIMPLE_FLUID
	//thickness = 0.0f;

	if (debug) {
		// do not blur
		gl_FragColor.x = depth;
		return;
	}

	// threshold on thickness to create nice smooth silhouettes
	if (depth == 0.0)//|| thickness < 0.02f)
	{
		gl_FragColor.x = 0.0;
		return;
	}

	/*
	float dzdx = dFdx(depth);
	float dzdy = dFdy(depth);

	// handle edge case
	if (max(abs(dzdx), abs(dzdy)) > 0.05)
	{
	dzdx = 0.0;
	dzdy = 0.0;

	gl_FragColor.x = depth;
	return;
	}
	*/

	float blurDepthFalloff = blurFalloff*mix(4.0, 1.0, thickness)/blurRadiusWorld*0.0375;	// these constants are just a re-scaling from some known good values

	float maxBlurRadius = 5.0;
	//float taps = min(maxBlurRadius, blurScale * (blurRadiusWorld / -depth));
	//vec2 blurRadius = min(mix(0.25, 2.0/blurFalloff, thickness) * blurScale * (blurRadiusWorld / -depth) / taps, 0.15)*invTexScale;

	//discontinuities between different tap counts are visible. to avoid this we 
	//use fractional contributions between #taps = ceil(radius) and floor(radius) 
	float radius = min(maxBlurRadius, blurScale * (blurRadiusWorld / -depth));
	float radiusInv = 1.0 / radius;
	float taps = ceil(radius);
	float frac = taps - radius;

	float sum = 0.0;
	float wsum = 0.0;
	float count = 0.0;

	for (float y = -taps; y <= taps; y += 1.0) {
		for (float x = -taps; x <= taps; x += 1.0) {
			vec2 offset = vec2(x, y);

			float sample = texture2DRect(depthTex, gl_FragCoord.xy + offset).x;

			if (sample < -10000.0*0.5)
				continue;

			// spatial domain
			float r1 = length(vec2(x, y))*radiusInv;
			float w = exp(-(r1*r1));

			//float expectedDepth = depth + dot(vec2(dzdx, dzdy), offset);

			// range domain (based on depth difference)
			float r2 = (sample - depth) * blurDepthFalloff;
			float g = exp(-(r2*r2));

			//fractional radius contributions
			float wBoundary = step(radius, max(abs(x), abs(y)));
			float wFrac = 1.0 - wBoundary*frac;

			sum += sample * w * g * wFrac;
			wsum += w * g * wFrac;
			count += g * wFrac;
		}
	}

	if (wsum > 0.0) {
		sum /= wsum;
	}

	float blend = count / sqr(2.0*radius + 1.0);
	gl_FragColor.x = mix(depth, sum, blend);
}

);