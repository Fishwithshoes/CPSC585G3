#version 450

#extension GL_NV_shadow_samplers_cube : enable

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ShadowCoord;

//Lightweight PBR Props
uniform float diffuseLevel;
uniform vec3 diffuseColor;
uniform float roughness;
uniform float metalness;
uniform int isMetallic;//When OFF=0 metalness simply represents base angle reflectivity
uniform float transparency;
uniform float IOR;
uniform vec3 refractColor;
uniform int isPhysicalTransparency;//When OFF=0 transparency simply represents reverse opacity
uniform float bumpLevel;
uniform float selfIllumLevel;
uniform vec3 selfIllumColor;
uniform float fogLevel;

//Non-physical. Just for giggles
uniform float rimLevel;
uniform vec3 rimColor;
uniform float rimPower;

//Maps
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D mirrorMap;
uniform samplerCube envMap;
uniform sampler2D roughnessMap;
uniform sampler2D metalnessMap;
uniform sampler2D grabPassMap;

//Texture Coords
uniform vec2 tileUV;
uniform vec2 offsetUV;

uniform vec3 cameraPos;
uniform vec3 cameraForward;
uniform vec4 lightColors[100];
uniform vec4 lightPositions[100];
uniform int lightCount;
uniform sampler2D shadowMap;
uniform vec2 screenDims;
uniform int isBloodMoon;

//Post Process
layout(location = 0) out vec4 OutputColor;
layout(location = 1) out vec4 OutputPosition;
layout(location = 2) out vec4 OutputNormal;

//OHH SUCH CHOICES!!
const float EPSILON = 0.003;
const float SHADOW_STRIDE = 0.02;//Global shadow blur multiplier. Set to 0 if SHADOW_SAMPLES = 1
//const int SHADOW_SAMPLES = 4;//4 is quite high enough, methinks!
const int SHADOW_SAMPLES = 0;//4 is quite high enough, methinks!
const float GLOSSY_STRIDE = 0.5;//Set to 0 if GLOSSY_SAMPLES = 1
//const int GLOSSY_SAMPLES = 4;//Again, 4 should satisfy thee!
const int GLOSSY_SAMPLES = 0;//Again, 4 should satisfy thee!
//Eat memory, gain performance!
const float SHADOW_STEP = SHADOW_STRIDE/SHADOW_SAMPLES * 2.0;
const float SHADOW_CONTRIB = 0.6/(SHADOW_SAMPLES*SHADOW_SAMPLES);
const float GLOSSY_STEP = GLOSSY_STRIDE/GLOSSY_SAMPLES * 2.0;
const float GLOSSY_CONTRIB = 1.0/(GLOSSY_SAMPLES*GLOSSY_SAMPLES*GLOSSY_SAMPLES);

void main()
{
	//ENV COLOR
	float ambientLevel = 0.2;
	vec3 dayPos = vec3(1,0,1);
	float t = 1.0;
	t = (dot(normalize(dayPos), cameraForward)+1)*0.5;	
	// vec3 envColor = vec3(0.7, 0.9, 1.0)*(1-t) + vec3(0.7, 0.9, 1.0)*t;
	vec3 envColor = vec3(0.4, 0.4, 1.0)*(1-t) + vec3(1.0,0.5,0.2)*t;
	if(isBloodMoon == 1)
		envColor = vec3(0.8, 0.2, 0.2)*(1-t) + vec3(1.0,0.5,0.2)*t;
	
	if(cameraPos.y < -2.0)
	{
		float v = clamp(0.0 - Position.y*0.0003, 0.0, 1.0);
		vec3 newEnvColor = (vec3(0.0, 1.0, 1.0)*0.4)*0.5 + envColor*0.5;
		envColor = (newEnvColor*(1-v) + vec3(0)*v);
	}

	//WORLD CALCULATIONS
	vec3 lightPos0 = vec3(5,2,5)*600;
	vec3 lightColor0 = vec3(1.0,0.5,0.0)*1.5;	
	vec3 lightDir0 = normalize(lightPos0.xyz-Position);
	float lightDist0 = length(lightPos0.xyz-Position);
	
	vec3 lightPos1 = vec3(-4,1,-4)*650;
	vec3 lightColor1 = vec3(0.8,0.8,1.0)*0.3;
	if(isBloodMoon == 1)
		lightColor1 = vec3(1.0, 0.3, 0.3);
	vec3 lightDir1 = normalize(lightPos1.xyz-Position);
	float lightDist1 = length(lightPos1.xyz-Position);
	
	vec3 viewDir = normalize(cameraPos-Position);
	float viewDist = length(cameraPos-Position);
	
	//TILE AND OFFSET UVs
	vec2 _texCoord = TexCoord * tileUV + offsetUV;
	
	//GET MAP TEXTELS
	vec4 diffuseTex = texture2D(diffuseMap, _texCoord);
	vec3 normalTex = texture2D(normalMap, _texCoord).xyz;
	vec3 roughnessTex = texture2D(roughnessMap, _texCoord).xyz;
	vec3 metalnessTex = texture2D(metalnessMap, _texCoord).xyz;
	vec3 shadowTex = texture2D(shadowMap, _texCoord).xyz;
	vec3 grabPassTex = texture2D(grabPassMap, _texCoord).xyz;
	
	float _roughness = roughness*roughnessTex.x;
	float _metalness = metalness*metalnessTex.x;
	
	//SEE THE NORMALS. BE THE NORMALS. QUICK AND DIRTY VARIANT.
	normalTex -= vec3(0.5, 0.5, 1.0);
	vec3 normalDir = normalize(Normal + normalTex*bumpLevel);
	
	//ENVIRONMENT MAPPING - CUBEMAP REFLECTION WITH BOX BLUR. WATCH THAT SAMPLE RATE!
	vec3 red = reflect(-viewDir, normalDir);
	float glossyStride = GLOSSY_STRIDE*_roughness;
	float glossyStep = GLOSSY_STEP*_roughness;
	float glossyFade = 1.0;
	vec3 envTex = vec3(0);
	
	for(int i = 0; i < GLOSSY_SAMPLES; i++)
	{
		float x = red.x - glossyStride + i*glossyStep;
		for(int j = 0; j < GLOSSY_SAMPLES; j++)
		{
			float y = red.y - glossyStride + j*glossyStep;
			for(int k = 0; k < GLOSSY_SAMPLES; k++)
			{
				float z = red.z - glossyStride + k*glossyStep;
				glossyFade = (1.0 - abs((i+j+z)/3 - GLOSSY_SAMPLES*0.5)/(GLOSSY_SAMPLES*0.5))*2;
				envTex += textureCube(envMap, vec3(x, y, z)).xyz * GLOSSY_CONTRIB * glossyFade;		
			}
		}
	}
	envTex *= envColor;
	
	//FRESNEL AND PHYSICAL CONSIDERATIONS
	float focus = clamp(dot(viewDir, normalDir), 0, 1);
	float edge = 1 - focus;
	float _reflectivity = clamp(((1-focus) * pow(edge, 2.5) + _metalness) * (1-_roughness), 0, 1);
	float _diffuseLevel = clamp(1 - _reflectivity, 0, 1);
	vec3 _diffuseColor = diffuseColor;
	vec3 _reflectColor = vec3(1,1,1);
	float _transparency = transparency * diffuseTex.w;
	if(isMetallic == 1)//Metallic materials should have reflections colored more by diffuse color
	{
		_reflectColor = _reflectColor*(1-_metalness) + diffuseLevel*diffuseColor*diffuseTex.xyz*_metalness;
		_diffuseColor = _diffuseColor*(1-_metalness);
	}
	if(isPhysicalTransparency == 1)//Physical transparency subtracts from diffuse and is subtracted by reflectivity
	{
		_diffuseColor = clamp(_diffuseColor - _transparency, 0.0, 1.0);
		_transparency = clamp(_transparency - _reflectivity, 0.0, 1.0);
	}
	
	//SURFACE SPACE RERACTION
	vec3 refraction = vec3(1,1,1);
	
	vec2 refractCoords = vec2(gl_FragCoord.x/screenDims.x, gl_FragCoord.y/screenDims.y)
		// + normalTex.xz*normalTex.yz*normalTex.xy*bumpLevel*IOR;
		+ normalDir.xz*normalDir.yz*normalDir.xy*IOR;
	
	refraction = vec3(texture2D(grabPassMap, refractCoords)).xyz;
	
	refraction = refraction * refractColor * _transparency;
	
	//FINAL PSUEDO-PHONG LIGHTING CONTRIBUTIONS
	float glossiness = (1-_roughness);
	float specExp = pow(glossiness+1, 2 + glossiness*10);//4096;//(1.04-_roughness)*25;
	float specFade = (1-_roughness) * 5;

	vec3 diffuseCoeff = _diffuseLevel * diffuseLevel * _diffuseColor * diffuseTex.xyz;
	vec3 specCoeff = _reflectivity * _reflectColor;
	
	vec3 diffuse0 = lightColor0 * diffuseCoeff * clamp(dot(lightDir0, normalDir), 0, 1);
	vec3 specular0 = lightColor0 * specCoeff * pow(clamp(dot(viewDir, reflect(-lightDir0, normalDir)), 0, 1), specExp) * specFade;
	
	vec3 diffuse1 = lightColor1 * diffuseCoeff * clamp(dot(lightDir1, normalDir), 0, 1);
	vec3 specular1 = lightColor1 * specCoeff * pow(clamp(dot(viewDir, reflect(-lightDir1, normalDir)), 0, 1), specExp) * specFade;
	
	vec3 reflection = specCoeff * envTex;
	vec3 selfIllum = selfIllumLevel * selfIllumColor * diffuseTex.xyz;
	vec3 rim = rimLevel * rimColor * clamp(dot(normalDir, lightDir0), 0, 1) * pow(1 - clamp(dot(viewDir, normalDir), 0, 1), rimPower);
	
	vec3 diffuse;
	vec3 specular;
	
	for(int i = 0; i < lightCount; i++)
	{
		vec3 dir = normalize(lightPositions[i].xyz - Position);
		float dist = distance(lightPositions[i].xyz, Position);
		float level = clamp((lightPositions[i].w - dist)/lightPositions[i].w, 0, 1) * lightColors[i].w;
		diffuse += lightColors[i].xyz * diffuseCoeff * clamp(dot(dir, normalDir), 0, 1) * level;
		specular += lightColors[i].xyz * specCoeff * pow(clamp(dot(viewDir, reflect(-dir, normalDir)), 0, 1), specExp) * level * specFade;
	}
	
	//SHADOWS!! BUT WATCH THAT SAMPLE RATE!	
	vec3 revisedShadowCoords = ShadowCoord.xyz * 0.5 + 0.5;
	//Distances are in texture space 0.0-1.0 (where 1.0 is ~ 1000m)
	float casterToLightDist = texture2D(shadowMap, revisedShadowCoords.xy).x;
	float fragToLightDist = distance(vec3(5,2,5)*50, Position) * 0.001;//LIGHTPOS
	float fragToCasterDist = fragToLightDist - casterToLightDist;//Estimate only. Error will increase with geometry thickness.	
	float bias = clamp(0.006*dot(Normal, lightDir0), 0.0, 0.006);
	// bias = 0;
	
	float baseSampleStride = 0.02;
	float shadowStride = 	SHADOW_STRIDE	* (fragToCasterDist + baseSampleStride);
	float shadowStep = 	SHADOW_STEP 	* (fragToCasterDist + baseSampleStride);
	float shadowFade = 1.0;
	float shadow = 0;
	
	if(length(Position) < 250)//Maximum reach of shadows
	{
		for(int i = 0; i < SHADOW_SAMPLES; i++)
		{
			float x = revisedShadowCoords.x - shadowStride + i*shadowStep;
			for(int j = 0; j < SHADOW_SAMPLES; j++)
			{
				float y = revisedShadowCoords.y - shadowStride + j*shadowStep;
				casterToLightDist = texture2D(shadowMap, vec2(x, y)).x;
				shadowFade = (1.0 - abs((i+j)*0.5 - SHADOW_SAMPLES*0.5)/(SHADOW_SAMPLES*0.5))*2;
				if(casterToLightDist < fragToLightDist - bias)
					shadow += SHADOW_CONTRIB * shadowFade;
			}
		}
	}
	
	diffuse0 *= 1.0-shadow;
	specular0 *= 1.0-shadow;
	
	diffuse += diffuse0 + diffuse1;
	specular += specular0 + specular1;
	
	//MIX IN AMBIENT COLOR AND SUM CONTRIBUTORS
	diffuse += (1-_diffuseLevel)*(envColor*ambientLevel)*diffuseLevel*diffuseColor*diffuseTex.xyz*(1-_reflectivity);
	vec3 final = diffuse + specular + reflection + refraction + selfIllum + rim;
	
	//FOGGY FUGUE
	float u = clamp(viewDist*0.003*fogLevel, 0, 1);
	final = final * (1-u) + envColor * u;
	
	//SHADOW MAP DEBUG
	// final = vec3(revisedShadowCoords.x, 0, 0);
	// final = vec3(0, revisedShadowCoords.y, 0);
	// final = vec3(revisedShadowCoords.xy, 0);
	
	// if(Position.z > 0)
		// final = vec3(fragToLightDist);
	// else
		// final = vec3(casterToLightDist);
	
	//OUTPUT
	OutputColor = vec4(final, 1.0);
	OutputPosition = vec4(Position*0.0005+0.5, 1.0);
	OutputNormal = vec4(normalDir*0.5+0.5, 1.0);
	
	//DEBUG DANCING - COMMENT OUT FOR FINAL COLOR
	// OutputColor = vec4(Color, 1);
	// OutputColor = vec4(Normal, 1);
	// OutputColor = vec4(normalDir, 1);
    // OutputColor = vec4(TexCoord.x, 0, 0, 1);
	// OutputColor = vec4(0, TexCoord.y, 0, 1);
	// OutputColor = vec4(TexCoord.xy, 0, 1);
	// OutputColor = vec4(gl_FragCoord.x/screenDims.x, gl_FragCoord.y/screenDims.y, 0, 1);
	// OutputColor = vec4(_reflectivity);
	// OutputColor = vec4(vec3(roughness), 1.0);
	// OutputColor = vec4(vec3(metalness), 1.0);
	// OutputColor = vec4(vec3(isMetallic), 1.0);
	// OutputColor = vec4(vec3(transparency), 1.0);
	// OutputColor = vec4(vec3(isPhysicalTransparency), 1.0);
	// OutputColor = vec4(bumpLevel);
	// OutputColor = vec4(fogLevel);
	// OutputColor = vec4(viewDist*0.01);
	
	// OutputColor = vec4(-viewDir, 1);
	// OutputColor = vec4(red, 1);
	
	// OutputColor = vec4(vec3(_diffuseLevel), 1.0);
	// OutputColor = vec4(vec3(_reflectivity), 1.0);
	// OutputColor = vec4(vec3(_metalness), 1.0);
	// OutputColor = vec4(vec3(_roughness), 1.0);
	// OutputColor = vec4(vec3(_transparency), 1.0);
	// OutputColor = vec4(diffuse, 1.0);
	// OutputColor = vec4(specular, 1.0);
	// OutputColor = vec4(reflection, 1.0);
	// OutputColor = vec4(selfIllum, 1.0);
	// OutputColor = vec4(rim, 1.0);
	// OutputColor = vec4(vec3(1.0-shadow), 1.0);
	
	// OutputColor = vec4(diffuseTex, 1.0);
	// OutputColor = vec4(roughnessTex, 1.0);
	// OutputColor = vec4(metalnessTex, 1.0);
	// OutputColor = vec4(normalTex, 1.0);
	// OutputColor = vec4(mirrorTex, 1.0);
	// OutputColor = vec4(envTex, 1.0);
	// OutputColor = vec4(shadowTex, 1.0);
	// OutputColor = vec4(grabPassTex.xyz, 1.0);
}
