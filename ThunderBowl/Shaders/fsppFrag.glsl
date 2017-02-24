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
	float focalDist = 20;
	float depthBlur = 0.06;
	float blurMinSize = 0.0;
	float blurMaxSize = 1.0;
	
	float bloomThreshold = 1.8;
	float bloomStrength = 1.55;
	
	float ambientOcclusionLevel = 0.10;
	
	vec3 colorCorrection = vec3(1.0, 1.0, 1.0);

	vec4 colorSample = texture2D(colorBufferMap, TexCoord);
	vec4 positionSample = texture2D(positionBufferMap, TexCoord);
	vec4 depthSample = texture(depthBufferMap, TexCoord);
	float dist = positionSample.w * 1000;//Convert from clamped to real distance
	
	vec4 colorSamples[SAMPLES*SAMPLES];
	vec4 positionSamples[SAMPLES*SAMPLES];
	
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
			
			//Bloom and SSAO can share samples
			x = TexCoord.x -SAMPLE_STRIDE + i*SAMPLE_STEP;
			y = TexCoord.y -SAMPLE_STRIDE + j*SAMPLE_STEP;
			colorSamples[index] = texture2D(colorBufferMap, vec2(x, y));
			positionSamples[index] = texture2D(positionBufferMap, vec2(x, y));
			index++;
		}
	}
	
	//PERFORM BLOOM AND SSAO WITH COLLECTED SAMPLES
	index = 0;
	float occlusionAmt = 0;
	for(int i = 0; i < SAMPLES; i++)
	{
		for(int j = 0; j < SAMPLES; j++)
		{
			//Gotta love dat bloom!
			vec4 currentColor = colorSamples[index];
			vec4 currentPos = positionSamples[index];
			if(currentColor.x + currentColor.y + currentColor.z > bloomThreshold)
				final += currentColor * SAMPLE_CONTRIB * bloomStrength;
				
			//Accumulate reverse dot products against all other samples to estimage occlusion
			occlusionAmt += (1-clamp(dot(positionSample.xyz, currentPos.xyz), 0, 1)) * SAMPLE_CONTRIB;
			
			index++;
		}
	}
	//Apply that wondrous SSAO
	final.xyz -= vec3(1) * occlusionAmt * ambientOcclusionLevel;
	
	//Put in that color correction. Easy peasy...
	final.xyz *= colorCorrection;

	// FragmentColor = vec4(final.xyz, 1.0);//Allow all effects
	FragmentColor = vec4(colorSample.xyz, 1.0);//Turn off all effects
	
	// FragmentColor = vec4(Color, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.xy, 0, 1);
	
	// FragmentColor = vec4(screenDims.xy, 0, 1);	
	// FragmentColor = vec4(colorSample.xyz, 1.0);
	// FragmentColor = vec4(positionSample.xyz, 1.0);
	// FragmentColor = vec4(vec3(depthSample.x), 1.0);
	// FragmentColor = vec4(vec3(positionSample.w), 1.0);
	// FragmentColor = vec4(vec3(positionSample.w), 1.0);
}