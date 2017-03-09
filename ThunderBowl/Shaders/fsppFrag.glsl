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
uniform sampler2D depthBufferMap;

out vec4 FragmentColor;

const int SAMPLES = 0;
const float SAMPLE_CONTRIB = 1.0/(SAMPLES*SAMPLES);
const float SAMPLE_STRIDE = 0.01;
const float SAMPLE_STEP = SAMPLE_STRIDE/SAMPLES*2;

const float EPSILON = 0.003;

void main()
{
	//TODO Make these uniforms as desired
	float focalDist = 13;
	float depthBlur = 0.06;
	float blurMinSize = 0.1;
	float blurMaxSize = 1.0;
	
	float farDist = 100;
	float farBlur = 0.006;
	float blurFarSize = 0.25;
	
	float bloomThreshold = 1.8;
	float bloomStrength = 1.25;
	
	float ambientOcclusionLevel = 0.25;
	
	vec3 colorCorrection = vec3(1.0, 1.0, 1.0);

	vec4 colorSample = texture2D(colorBufferMap, TexCoord);
	vec4 positionSample = texture2D(positionBufferMap, TexCoord)*2000.0 - vec4(1000.0);
	vec4 normalSample = texture2D(normalBufferMap, TexCoord)*2.0 - vec4(1.0);
	vec4 depthSample = texture(depthBufferMap, TexCoord);
	float dist = distance(positionSample.xyz, cameraPos);
	
	vec4 final = vec4(0,0,0,0);
	
	//GET IMAGE SAMPLES AND PERFORM DEPTH OF FIELD	
	float finalBlur = blurMinSize;
	if(dist < focalDist)
		finalBlur = clamp(abs(dist - focalDist)*depthBlur, blurMinSize, blurMaxSize);
	
	if(dist > farDist)
		finalBlur = clamp(abs(dist - farDist)*farBlur, blurMinSize, blurFarSize);
	
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
			vec4 currentPos = texture2D(positionBufferMap, vec2(x, y))*2000.0 - vec4(1000.0);
			vec4 currentNormal = texture2D(normalBufferMap, vec2(x, y))*2.0 - vec4(1.0);
		
			//Gotta love dat bloom!
			if(currentColor.x + currentColor.y + currentColor.z > bloomThreshold)
				final += currentColor * SAMPLE_CONTRIB * bloomStrength;
			
			if(dist < 200)//Avoid AO on distant objects
			{
				//Compare nearby normals and positions to estimate occlusion			
				// float dist = distance(positionSample.xyz, currentPos.xyz);
				// float distFalloff = clamp(3.0-dist, 0.0, 1.0);
				// float angle = dot(normalSample.xyz, currentNormal.xyz);
				// float angleFalloff = 1.0-abs(angle);					
				// occlusionAmt += distFalloff * angleFalloff * SAMPLE_CONTRIB;
				
				//Compare using a hemisphere to avoid shadowing with intersected geometry
				vec3 otherNormal = normalize(currentPos.xyz - positionSample.xyz);
				float dist = distance(positionSample.xyz, currentPos.xyz);
				float distFalloff = clamp((10.0-dist)*0.1, 0.0, 1.0);
				float angle = dot(normalSample.xyz, otherNormal);
				float angleFalloff = clamp(angle, 0.0, 1.0);
				// occlusionAmt += distFalloff * SAMPLE_CONTRIB;
				occlusionAmt += distFalloff * angleFalloff * SAMPLE_CONTRIB;
			}
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
	// float c = 0.1;
	// final.xyz = vec3(clamp(final.x-(final.x-0.5)*m-c,0,1),clamp(final.y-(final.y-0.5)*m-c,0,1),clamp(final.z-(final.z-0.5)*m-c,0,1));
	
	// FragmentColor = vec4(final.xyz, 1.0);//Allow all effects
	FragmentColor = vec4(colorSample.xyz, 1.0);//Turn off all effects
	
	// FragmentColor = vec4(Color, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.xy, 0, 1);
	
	// FragmentColor = vec4(vec3(1 - occlusionAmt*ambientOcclusionLevel), 1.0);
	
	// FragmentColor = vec4(screenDims.xy, 0, 1);	
	// FragmentColor = vec4(colorSample.xyz, 1.0);
	// FragmentColor = vec4(abs(positionSample.xyz*0.001), 1.0);
	// FragmentColor = vec4(0.0, positionSample.y, 0.0, 1.0);
	// FragmentColor = vec4(abs(normalSample.xyz), 1.0);
	// FragmentColor = vec4(vec3(depthSample.x), 1.0);
	// FragmentColor = vec4(vec3(dist)*0.001, 1.0);
}