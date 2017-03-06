#version 450

in vec3 Position;
in vec3 Color;
in vec2 TexCoord;

//Post Process
uniform vec2 screenDims;

uniform sampler2D colorBufferMap;
uniform sampler2D positionBufferMap;
uniform sampler2D depthBufferMap;

out vec4 FragmentColor;

const int SAMPLES = 8;
const float SAMPLE_CONTRIB = 1.0/(SAMPLES*SAMPLES);
const float SAMPLE_STRIDE = 0.01;
const float SAMPLE_STEP = SAMPLE_STRIDE/SAMPLES*2;

void main()
{
	//TODO Make these uniforms as desired
	float focalDist = 15;
	float depthBlur = 0.06;
	float blurMinSize = 0.0;
	float blurMaxSize = 1.0;
	
	float bloomThreshold = 1.8;
	float bloomStrength = 1.55;
	
	float ambientOcclusionLevel = 0.0;
	
	vec3 colorCorrection = vec3(1.0, 1.0, 1.0);

	vec4 colorSample = texture2D(colorBufferMap, TexCoord);
	vec4 positionSample = texture2D(positionBufferMap, TexCoord);
	vec4 depthSample = texture(depthBufferMap, TexCoord);
	float dist = positionSample.w * 1000;//Convert from clamped to real distance
	
	vec4 final = vec4(0,0,0,0);
	
	//GET IMAGE SAMPLES AND PERFORM DEPTH OF FIELD	
	float finalBlur = blurMinSize;
	if(dist < focalDist)
		finalBlur = clamp(abs(dist - focalDist)*depthBlur, blurMinSize, blurMaxSize);
	
	int index = 0;
	for(int i = 0; i < SAMPLES; i++)
	{
		for(int j = 0; j < SAMPLES; j++)
		{
			//DOF is sampled at a different radius based on dist from focus			
			float x = TexCoord.x -SAMPLE_STRIDE*finalBlur + i*SAMPLE_STEP*finalBlur;
			float y = TexCoord.y -SAMPLE_STRIDE*finalBlur + j*SAMPLE_STEP*finalBlur;
			
			final += texture2D(colorBufferMap, vec2(x, y)) * SAMPLE_CONTRIB;
		}
	}
	
	//PERFORM BLOOM AND SSAO WITH SHARED SAMPLES
	index = 0;
	float occlusionAmt = 0;
	for(int i = 0; i < SAMPLES; i++)
	{
		for(int j = 0; j < SAMPLES; j++)
		{
			//Bloom and SSAO can share samples
			float x = TexCoord.x -SAMPLE_STRIDE + i*SAMPLE_STEP;
			float y = TexCoord.y -SAMPLE_STRIDE + j*SAMPLE_STEP;
			
			vec4 currentColor = texture2D(colorBufferMap, vec2(x, y));
			vec4 currentPos = texture2D(positionBufferMap, vec2(x, y));
		
			//Gotta love dat bloom!
			if(currentColor.x + currentColor.y + currentColor.z > bloomThreshold)
				final += currentColor * SAMPLE_CONTRIB * bloomStrength;
				
			//Accumulate reverse dot products against all other samples to estimage occlusion
			float dot = dot(positionSample.xyz, currentPos.xyz);
			if(dot > 0.0)
				occlusionAmt += clamp((0.5-dot), 0, 1) * SAMPLE_CONTRIB;
		}
	}
	//Apply that wondrous SSAO
	final.xyz -= vec3(1) * occlusionAmt * ambientOcclusionLevel;
	
	//Put in that color correction. Easy peasy...
	final.xyz *= colorCorrection;

	FragmentColor = vec4(final.xyz, 1.0);//Allow all effects
	// FragmentColor = vec4(colorSample.xyz, 1.0);//Turn off all effects
	
	// FragmentColor = vec4(Color, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.xy, 0, 1);
	
	// FragmentColor = vec4(vec3(1 - occlusionAmt*ambientOcclusionLevel), 1.0);
	
	// FragmentColor = vec4(screenDims.xy, 0, 1);	
	// FragmentColor = vec4(colorSample.xyz, 1.0);
	// FragmentColor = vec4(positionSample.xyz, 1.0);
	// FragmentColor = vec4(vec3(depthSample.x), 1.0);
	// FragmentColor = vec4(vec3(positionSample.w), 1.0);
}