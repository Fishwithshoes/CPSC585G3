#version 450

in vec3 Position;
in vec3 Color;
in vec2 TexCoord;

//Post Process
uniform vec2 screenDims;

uniform sampler2D colorBufferMap;
uniform sampler2D positionBufferMap;

out vec4 FragmentColor;

const int SAMPLES = 4;
const float SAMPLE_CONTRIB = 1.0/(SAMPLES*SAMPLES);

void main()
{
	vec4 colorSample = texture2D(colorBufferMap, TexCoord);
	vec4 positionSample = texture2D(positionBufferMap, TexCoord);
	
	vec4 colorSamples[];

	FragmentColor = vec4(colorSample);
	
	// FragmentColor = vec4(Color, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.xy, 0, 1);
	
	// FragmentColor = vec4(screenDims.xy, 0, 1);	
	// FragmentColor = vec4(colorSample);
	// FragmentColor = vec4(positionSample);
}