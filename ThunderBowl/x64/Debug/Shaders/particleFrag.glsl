#version 450

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

uniform vec4 color;
uniform sampler2D mainTexture;

uniform vec3 cameraPos;

//Post Process
uniform vec2 screenDims;

uniform sampler2D colorBufferMap;
uniform sampler2D positionBufferMap;

uniform writeonly image2D colorBuffer;
uniform writeonly image2D positionBuffer;

out vec4 FragmentColor;

void main()
{
	//WORLD CALCULATIONS
	vec3 viewDir = normalize(cameraPos-Position);
	float viewDist = length(cameraPos-Position);
	
	vec4 mainTex = texture2D(mainTexture, TexCoord);
	
	vec4 final = color * mainTex;
	
	//OUTPUT
	vec2 screenSampleCoords = vec2(gl_FragCoord.x/screenDims.x, gl_FragCoord.y/screenDims.y);
	vec4 colorBufferTex = texture2D(colorBufferMap, screenSampleCoords);
	vec4 positionBufferTex = texture2D(positionBufferMap, screenSampleCoords);
	
	if(viewDist < positionBufferTex.w)
	{
		//TODO This storage may need to be updated to incorporate alpha blending
		imageStore(colorBuffer, ivec2(gl_FragCoord.xy), vec4(final));
		imageStore(positionBuffer, ivec2(gl_FragCoord.xy), vec4(Position, viewDist));
	}
	
	FragmentColor = vec4(final);
	
	// FragmentColor = vec4(Color.xyz, 1);
	// FragmentColor = vec4(Normal, 1);	
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
	// FragmentColor = vec4(cameraPos, 1);
	// FragmentColor = vec4(viewDist);
}