#version 450

in vec3 Position;
in vec3 Color;
in vec2 TexCoord;

//Post Process
uniform vec2 screenDims;
uniform vec3 cameraPos;

uniform sampler2D colorBufferMap;
uniform sampler2D positionBufferMap;
uniform sampler2D normalBufferMap;
uniform sampler2D previousBufferMap;
uniform sampler2D depthBufferMap;

out vec4 FragmentColor;

const int DOF_SAMPLES = 12;//Setting this to 0 will render black
const float DOF_SAMPLE_CONTRIB = 1.0/(DOF_SAMPLES*DOF_SAMPLES);
const float DOF_STRIDE = 0.01;
const float DOF_FILLER = 1.0/DOF_STRIDE;
const float DOF_STEP = DOF_STRIDE/DOF_SAMPLES*2;
const float CURRENT_WEIGHT = 0.67;
const float PREVIOUS_WEIGHT = 1.0 - CURRENT_WEIGHT;

const int BLOOM_SAMPLES = 16;
const float BLOOM_SAMPLE_CONTRIB = 1.0/(BLOOM_SAMPLES*BLOOM_SAMPLES);
const float BLOOM_STRIDE = 0.05;
const float BLOOM_FILLER = 1.0/BLOOM_STRIDE;
const float BLOOM_STEP = BLOOM_STRIDE/BLOOM_SAMPLES*2;

const int AO_SAMPLES = 8;
const float AO_SAMPLE_CONTRIB = 1.0/(AO_SAMPLES*AO_SAMPLES);
const float AO_STRIDE = 0.01;
const float AO_FILLER = 1.0/AO_STRIDE;
const float AO_STEP = AO_STRIDE/AO_SAMPLES*2;

const float EPSILON = 0.003;

void main()
{
	//TODO Make these uniforms as desired
	float focalDist = 10;
	float depthBlur = 0.06;
	float blurMinSize = 0.0;
	float blurMaxSize = 1.0;
	
	float farDist = 200;
	float farBlur = 0.006;
	float blurFarSize = 0.25;
	
	float bloomThreshold = 1.8;
	float bloomStrength = 6.25;
	
	float ambientOcclusionLevel = 0.5;
	
	vec3 colorCorrection = vec3(1.0, 1.0, 1.0);

	vec4 colorSample = texture2D(colorBufferMap, TexCoord);
	vec4 positionSample = texture2D(positionBufferMap, TexCoord)*2000.0 - vec4(1000.0);
	vec4 normalSample = texture2D(normalBufferMap, TexCoord)*2.0 - vec4(1.0);
	vec4 previousSample = texture2D(previousBufferMap, TexCoord);
	vec4 depthSample = texture(depthBufferMap, TexCoord);
	float dist = distance(positionSample.xyz, cameraPos);
	
	vec4 final = vec4(0,0,0,0);
	
	//GET IMAGE COLOR SAMPLES AND PERFORM DEPTH OF FIELD	
	float finalBlur = blurMinSize;
	if(dist < focalDist)
		finalBlur = clamp(abs(dist - focalDist)*depthBlur, blurMinSize, blurMaxSize);
	
	if(dist > farDist)
		finalBlur = clamp(abs(dist - farDist)*farBlur, blurMinSize, blurFarSize);
	
	int index = 0;
	for(int i = 0; i < DOF_SAMPLES; i++)
	{
		for(int j = 0; j < DOF_SAMPLES; j++)
		{
			//DOF is sampled at a different radius based on dist from focus			
			float x = TexCoord.x -DOF_STRIDE*finalBlur + i*DOF_STEP*finalBlur;
			float y = TexCoord.y -DOF_STRIDE*finalBlur + j*DOF_STEP*finalBlur;
			
			// float fade = clamp((DOF_STRIDE - distance(vec2(x,y), TexCoord))*DOF_FILLER, 0, 1);
			
			vec4 current = texture2D(colorBufferMap, vec2(x, y)) * CURRENT_WEIGHT * DOF_SAMPLE_CONTRIB;
			vec4 previous = texture2D(previousBufferMap, vec2(x, y)) * PREVIOUS_WEIGHT * DOF_SAMPLE_CONTRIB;
			
			final += current + previous;
		}
	}
	
	//PERFORM BLOOM WITH COLOR SAMPLES
	index = 0;
	float finalBloomStride = clamp(BLOOM_STRIDE - dist*0.003, 0.015, BLOOM_STRIDE);
	float finalBloomFiller = 1.0/finalBloomStride;
	float finalBloomStep = finalBloomStride/BLOOM_SAMPLES*2.0;
	
	for(int i = 0; i < BLOOM_SAMPLES; i++)
	{
		for(int j = 0; j < BLOOM_SAMPLES; j++)
		{
			float x = TexCoord.x -finalBloomStride + i*finalBloomStep;
			float y = TexCoord.y -finalBloomStride + j*finalBloomStep;
			
			vec4 currentColor = texture2D(colorBufferMap, vec2(x, y));
			
			float fade = clamp((finalBloomStride - distance(vec2(x,y), TexCoord))*finalBloomFiller, 0, 1);
			
			//Gotta love dat bloom!
			if(currentColor.x + currentColor.y + currentColor.z > bloomThreshold)
				final += currentColor * BLOOM_SAMPLE_CONTRIB * bloomStrength * fade;
		}
	}
	
	//PERFORM SSAO WITH POS AND NORMAL SAMPLES
	float occlusionAmt = 0;
	for(int i = 0; i < AO_SAMPLES; i++)
	{
		for(int j = 0; j < AO_SAMPLES; j++)
		{
			float x = TexCoord.x -AO_STRIDE + i*AO_STEP;
			float y = TexCoord.y -AO_STRIDE + j*AO_STEP;
		
			vec4 currentPos = texture2D(positionBufferMap, vec2(x, y))*2000.0 - vec4(1000.0);
			vec4 currentNormal = texture2D(normalBufferMap, vec2(x, y))*2.0 - vec4(1.0);
			
			// float fade = clamp((AO_STRIDE - distance(vec2(x,y), TexCoord))*AO_FILLER, 0, 1);
		
			// if(dist < 100)//Avoid AO on distant objects
			// {
				//Compare nearby normals and positions to estimate occlusion			
				// float dist = distance(positionSample.xyz, currentPos.xyz);
				// float distFalloff = clamp(3.0-dist, 0.0, 1.0);
				// float angle = dot(normalSample.xyz, currentNormal.xyz);
				// float angleFalloff = 1.0-abs(angle);					
				// occlusionAmt += distFalloff * angleFalloff * AO_SAMPLE_CONTRIB;
				
				//Compare using a hemisphere to avoid shadowing with intersected geometry
				vec3 otherNormal = normalize(currentPos.xyz - positionSample.xyz);
				float dist = distance(positionSample.xyz, currentPos.xyz);
				float distFalloff = clamp((4.0-dist)*0.25, 0.0, 1.0);
				float angle = dot(normalSample.xyz, otherNormal);
				float angleFalloff = clamp(angle, 0.0, 1.0);
				// occlusionAmt += distFalloff * AO_SAMPLE_CONTRIB;
				occlusionAmt += distFalloff * angleFalloff * AO_SAMPLE_CONTRIB;
			// }
		}
	}
	
	//Apply that wondrous SSAO
	final.xyz -= vec3(1) * occlusionAmt * ambientOcclusionLevel;
	
	//Put in that color correction. Easy peasy...
	final.xyz *= colorCorrection;
	
	//B/W anyone?
	// final.xyz = vec3(final.x*0.213 + final.y*0.715 + final.z*0.072);
	
	//Sepia
	// float lum = final.x*0.299 + final.y*0.587 + final.z*0.114;
	// final.xyz = vec3(lum+0.4, lum+0.2, lum);
	
	//Juicy
	// float m = 0.6;
	// final.xyz = vec3(clamp(final.x+(final.x-0.5)*m,0,1),clamp(final.y+(final.y-0.5)*m,0,1),clamp(final.z+(final.z-0.5)*m,0,1));
	
	//Drab
	// float m = 0.4;
	// float c = 0.1;//Cuts component down
	// final.xyz = vec3(clamp(final.x-(final.x-0.5)*m-c,0,1),clamp(final.y-(final.y-0.5)*m-c,0,1),clamp(final.z-(final.z-0.5)*m-c,0,1));
	
	FragmentColor = vec4(final.xyz, 1.0);//Allow all effects
	// FragmentColor = vec4(colorSample.xyz, 1.0);//Turn off all effects. This does NOT shut off sampling.
	
	// FragmentColor = vec4(Color, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.xy, 0, 1);
	
	// FragmentColor = vec4(vec3(1 - occlusionAmt*ambientOcclusionLevel), 1.0);
	
	// FragmentColor = vec4(screenDims.xy, 0, 1);	
	// FragmentColor = vec4(colorSample.xyz, 1.0);
	// FragmentColor = vec4(colorSample.x, previousSample.y, 0.0, 1.0);
	// FragmentColor = vec4(abs(positionSample.xyz*0.001), 1.0);
	// FragmentColor = vec4(0.0, positionSample.y, 0.0, 1.0);
	// FragmentColor = vec4(abs(normalSample.xyz), 1.0);
	// FragmentColor = vec4(vec3(depthSample.x), 1.0);
	// FragmentColor = vec4(vec3(dist)*0.001, 1.0);
}