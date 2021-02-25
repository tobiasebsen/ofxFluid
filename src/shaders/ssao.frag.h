#define STRINGIFY(A) #A
static const char* fragmentCompositeShader = STRINGIFY(

uniform vec4 diffuse;

uniform vec2 noiseScale;
uniform int kernelSize;
uniform float radius;
uniform float bias;

uniform vec3 samples[64];

uniform sampler2D tPosition;
uniform sampler2D tNormal;
uniform sampler2D tNoise;

void main() {
	// sample from depth texture
	// discard fragment if depth is zero (frament not defined)
	vec4 pos = texture2D(tPosition, gl_TexCoord[0].xy);
	if (pos.z == 0.0) discard;
	gl_FragDepth = pos.w;

	vec3 fragPos = pos.xyz;
	vec3 normal = normalize(texture2D(tNormal, gl_TexCoord[0].xy).xyz);
	vec3 randomVec = normalize(texture2D(tNoise, gl_TexCoord[0].xy * noiseScale).xyz);

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	for (int i = 0; i < kernelSize; ++i) {
		// get sample position
		vec3 samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius;

		// project sample position (to sample texture) (to get position on screen/texture)
		vec4 offset = vec4(samplePos, 1.0);
		offset = gl_ProjectionMatrix * offset; // from view to clip-space
		offset.xyz /= offset.w; // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

											 // get sample depth
		float sampleDepth = texture2D(tPosition, offset.xy).z; // get depth value of kernel sample

															 // range check & accumulate
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = 1.0 - (occlusion / float(kernelSize));

	gl_FragColor = diffuse * vec4(vec3(occlusion), 1.0);
}

);