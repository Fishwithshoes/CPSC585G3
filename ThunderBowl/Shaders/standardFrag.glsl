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

//Texture Coords
uniform vec2 tileUV;
uniform vec2 offsetUV;

uniform vec3 cameraPos;
uniform vec3 cameraForward;
uniform vec4 lightPos;
uniform vec4 lightColor;
uniform sampler2D shadowMap;

//Post Process
layout(location = 0) out vec4 OutputColor;
layout(location = 1) out vec4 OutputPosition;

struct Hit
{
	float distance;
	vec3 position;
	vec3 normal;
};
Hit hit;

//OHH SUCH CHOICES!!
const float EPSILON = 0.003;
const float LIGHT_RADIUS = 1;//Set to 0 if SHADOW_SAMPLES = 1
const int SHADOW_SAMPLES = 2;//4 is quite high enough, methinks!
const float GLOSSY_STRETCH = 0.5;//Set to 0 i GLOSSY_SAMPLES = 1;
const int GLOSSY_SAMPLES = 4;//Again, 4 should satisfy thee!
//Eat memory, gain performance!
const float SHADOW_CONTRIB = 1.0/(SHADOW_SAMPLES*SHADOW_SAMPLES);
const float GLOSSY_CONTRIB = 1.0/(GLOSSY_SAMPLES*GLOSSY_SAMPLES*GLOSSY_SAMPLES);
const float SHADOW_STRIDE = LIGHT_RADIUS/SHADOW_SAMPLES * 2.0;
const float GLOSSY_STRIDE = GLOSSY_STRETCH/GLOSSY_SAMPLES * 2.0;

void RunSceneCollisions(vec3 ray, vec3 origin);

void TestPlane(vec3 p0, vec3 normal, vec3 ray, vec3 origin);
void TestSphere(vec3 center, float radius, vec3 ray, vec3 origin);
void TestCylinder(vec3 center, float radius, float height, vec3 ray, vec3 origin);
void TestTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 ray, vec3 origin);

void main()
{
	//ENV COLOR
	float ambientLevel = 0.2;
	vec3 dayPos = vec3(1,0,1);
	float t = 1.0;
	t = (dot(normalize(dayPos), cameraForward)+1)*0.5;	
	// vec3 envColor = vec3(0.7, 0.9, 1.0)*(1-t) + vec3(0.7, 0.9, 1.0)*t;
	vec3 envColor = vec3(0.4, 0.4, 1.0)*(1-t) + vec3(1.0,0.5,0.2)*t;

	//WORLD CALCULATIONS
	vec3 lightPos0 = vec3(5,2,5)*600;
	vec3 lightColor0 = vec3(1.0,0.5,0.0)*2.2;	
	vec3 lightDir0 = normalize(lightPos0.xyz-Position);
	float lightDist0 = length(lightPos0.xyz-Position);
	
	vec3 lightPos1 = vec3(-4,1,-4)*650;
	vec3 lightColor1 = vec3(0.8,0.8,1.0)*0.7;	
	vec3 lightDir1 = normalize(lightPos1.xyz-Position);
	float lightDist1 = length(lightPos1.xyz-Position);
	
	vec3 viewDir = normalize(cameraPos-Position);
	float viewDist = length(cameraPos-Position);
	
	//TILE AND OFFSET UVs
	vec2 _texCoord = TexCoord * tileUV + offsetUV;
	
	//GET MAP TEXTELS
	vec3 diffuseTex = texture2D(diffuseMap, _texCoord).xyz;
	vec3 normalTex = texture2D(normalMap, _texCoord).xyz;
	vec3 roughnessTex = texture2D(roughnessMap, _texCoord).xyz;
	vec3 metalnessTex = texture2D(metalnessMap, _texCoord).xyz;
	vec3 shadowTex = texture2D(shadowMap, _texCoord).xyz;
	
	float _roughness = roughness*roughnessTex.x;
	float _metalness = metalness*metalnessTex.x;
	
	//SEE THE NORMALS. BE THE NORMALS. QUICK AND DIRTY VARIANT.
	normalTex -= vec3(0.5, 0.5, 1.0);
	vec3 normalDir = normalize(Normal + normalTex*bumpLevel);
	
	//ENVIRONMENT MAPPING - CUBEMAP REFLECTION WITH BOX BLUR. WATCH THAT SAMPLE RATE!
	vec3 red = reflect(-viewDir, normalDir);
	float totalStride = GLOSSY_STRETCH*_roughness;
	float strideIncrement = GLOSSY_STRIDE*_roughness;
	float fade = 1;
	vec3 envTex = vec3(0);
	
	for(int i = 0; i < GLOSSY_SAMPLES; i++)
	{
		float x = red.x - totalStride + i*strideIncrement;
		for(int j = 0; j < GLOSSY_SAMPLES; j++)
		{
			float y = red.y - totalStride + j*strideIncrement;
			for(int k = 0; k < GLOSSY_SAMPLES; k++)
			{
				float z = red.z - totalStride + k*strideIncrement;
				float fade = (1.0 - abs((i+j+z)/3 - GLOSSY_SAMPLES*0.5)/(GLOSSY_SAMPLES*0.5))*2;
				envTex += textureCube(envMap, vec3(x, y, z)).xyz * GLOSSY_CONTRIB * fade;		
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
	if(isMetallic == 1)//Metallic materials should have reflections colored more by diffuse color
	{
		_reflectColor = _reflectColor*(1-_metalness) + diffuseLevel*diffuseColor*diffuseTex*_metalness;
		_diffuseColor = _diffuseColor*(1-_metalness);
	}
	
	//FINAL PSUEDO-PHONG LIGHTING CONTRIBUTIONS
	float glossiness = (1-_roughness);
	float specExp = pow(glossiness+1, 2 + glossiness*10);//4096;//(1.04-_roughness)*25;
	float specFade = (1-_roughness) * 5;
	
	vec3 diffuse = _diffuseLevel * lightColor0 * diffuseLevel * _diffuseColor * diffuseTex * clamp(dot(lightDir0, normalDir), 0, 1);
	vec3 specular = _reflectivity * lightColor0 * _reflectColor * pow(clamp(dot(viewDir, reflect(-lightDir0, normalDir)), 0, 1), specExp) * specFade;
	
	diffuse += _diffuseLevel * lightColor1 * diffuseLevel * _diffuseColor * diffuseTex * clamp(dot(lightDir1, normalDir), 0, 1);
	specular += _reflectivity * lightColor1 * _reflectColor * pow(clamp(dot(viewDir, reflect(-lightDir1, normalDir)), 0, 1), specExp) * specFade;
	
	vec3 reflection = _reflectivity * _reflectColor * envTex;
	vec3 selfIllum = selfIllumLevel * selfIllumColor * diffuseTex;
	vec3 rim = rimLevel * rimColor * clamp(dot(normalDir, lightDir0), 0, 1) * pow(1 - clamp(dot(viewDir, normalDir), 0, 1), rimPower);
	
	//SHADOWS!! BUT WATCH THAT SAMPLE RATE!
	vec3 ray = lightDir0;
	vec3 origin = Position;
	
	float shadow = 0;
	vec3 east = normalize(cross(lightPos.xyz-Position, normalDir));
	vec3 north = normalize(cross(lightPos.xyz-Position, east));
	//Simple floating-plane shadows, use the processed normals for increased sample jitter
	if(dot(ray, normalDir) > EPSILON)//Don't trace fragments pointing away from light
	{
		for(int i = 0; i < SHADOW_SAMPLES; i++)
		{
			for(int j = 0; j < SHADOW_SAMPLES; j++)
			{
				vec3 destination = lightPos0;
				destination += -LIGHT_RADIUS*east + i*SHADOW_STRIDE*east;
				destination += -LIGHT_RADIUS*north + j*SHADOW_STRIDE*north;
				
				ray = destination-Position;
				RunSceneCollisions(ray, origin);
				
				if(hit.distance > EPSILON && hit.distance < lightDist0)
				{	
					shadow += clamp(1-hit.distance, 0, 1) * SHADOW_CONTRIB;
				}
			}
		}
	}
	
	// if(texture2D(shadowMap, ShadowCoord.xy*0.001).x < ShadowCoord.z*0.001)
		// shadow = 1.0;
		
	vec2 revisedShadowCoords = vec2(ShadowCoord.x, ShadowCoord.y)*0.0015 + vec2(0.2, 0.0);
		
	if(texture2D(shadowMap, revisedShadowCoords).x < ShadowCoord.z*0.001)
		shadow = 1.0;
	
	// diffuse *= 1-shadow;
	// specular *= 1-shadow;
	
	//MIX IN AMBIENT COLOR AND SUM CONTRIBUTORS
	diffuse += (1-_diffuseLevel)*(envColor*ambientLevel)*diffuseLevel*diffuseColor*diffuseTex*(1-_reflectivity);
	vec3 final = diffuse + specular + reflection + selfIllum + rim;
	
	//FOGGY FUGUE
	float u = clamp(viewDist*0.01*fogLevel, 0, 1);
	final = final * (1-u) + envColor * u;
	
	// final = vec3(ShadowCoord.x, 0, 0) * 0.001;
	// final = vec3(0, ShadowCoord.y, 0) * 0.001;
	// final = vec3(ShadowCoord.xy, 0) * 0.002;
	// final = vec3(ShadowCoord.z) * 0.001;
	
	// final = vec3(texture2D(shadowMap, revisedShadowCoords).x);
	
	//OUTPUT
	OutputColor = vec4(final, 1.0);
	OutputPosition = vec4(Normal, viewDist*0.001);
	
	//DEBUG DANCING - COMMENT OUT FOR FINAL COLOR
	// OutputColor = vec4(Color, 1);
	// OutputColor = vec4(Normal, 1);
	// OutputColor = vec4(normalDir, 1);
    // OutputColor = vec4(TexCoord.x, 0, 0, 1);
	// OutputColor = vec4(0, TexCoord.y, 0, 1);
	// OutputColor = vec4(TexCoord.xy, 0, 1);
	// OutputColor = vec4(gl_FragCoord.x/screenDims.x, gl_FragCoord.y/screenDims.y, 0, 1);
	// OutputColor = vec4(_reflectivity);
	// OutputColor = vec4(roughness);
	// OutputColor = vec4(metalness);
	// OutputColor = vec4(isMetallic);
	// OutputColor = vec4(bumpLevel);
	// OutputColor = vec4(fogLevel);
	// OutputColor = vec4(viewDist*0.01);
	// OutputColor = vec4(hit.distance);
	
	// OutputColor = vec4(-viewDir, 1);
	// OutputColor = vec4(red, 1);
	
	// OutputColor = vec4(_diffuseLevel);
	// OutputColor = vec4(_reflectivity);
	// OutputColor = vec4(diffuse, 1.0);
	// OutputColor = vec4(specular, 1.0);
	// OutputColor = vec4(reflection, 1.0);
	// OutputColor = vec4(selfIllum, 1.0);
	// OutputColor = vec4(rim, 1.0);
	// OutputColor = vec4(1-shadow);
	
	// OutputColor = vec4(diffuseTex, 1.0);
	// OutputColor = vec4(roughnessTex, 1.0);
	// OutputColor = vec4(metalnessTex, 1.0);
	// OutputColor = vec4(normalTex, 1.0);
	// OutputColor = vec4(mirrorTex, 1.0);
	// OutputColor = vec4(envTex, 1.0);
	// OutputColor = vec4(shadowTex, 1.0);
	
	// OutputColor = vec4(colorBufferTex);
	// OutputColor = vec4(positionBufferTex);
}

void RunSceneCollisions(vec3 ray, vec3 origin)
{
	hit.distance = 1000;
	
	// TestSphere(vec3(0,1,0), 1, ray, origin);
	// TestCylinder(vec3(3, 2, 0), 1, 2, ray, origin);
}

void TestPlane(vec3 p0, vec3 normal, vec3 ray, vec3 origin)
{
	if(dot(ray,normal) != 0)
	{
		float d = dot((p0 - origin), normal)/dot(ray,normal);

		if(d < hit.distance && d > EPSILON)
		{
			hit.distance = d;
			//hit.position = origin + hit.distance*ray;
			//hit.normal = normal;
		}
	}
}

void TestSphere(vec3 center, float radius, vec3 ray, vec3 origin)
{
	float det = pow(dot(ray, (origin-center)),2) - pow(length(origin-center),2) + pow(radius,2);

	if(det > 0)//Two collisions
	{
		//Find smaller scalar (nearest collision)
		det = sqrt(det);
		float head = -dot(ray,(origin-center));
		float d = head + det;

		if(head - det < d)
			d = head - det;

		if(d < hit.distance && d > EPSILON)
		{
			hit.distance = d;
			// hit.position = origin + hit.distance*ray;
			// hit.normal = normalize(hit.position-center);
		}
	}
}

void TestCylinder(vec3 center, float radius, float height, vec3 ray, vec3 origin)
{
	//Cap
	vec3 up = vec3(0,1,0);
	center.y += height*0.5;
	if(dot(ray,up) < 0)
	{
		float d = dot((center - origin), up/dot(ray,up));		
		vec3 pos = origin + d*ray;		
		if(distance(pos, center) < radius)
		{
			if(d < hit.distance && d > EPSILON)
			{
				hit.distance = d;
				//hit.position = origin + hit.distance*ray;
				//hit.normal = up;
			}
		}
	}
	
	//Body
	center.y -= height*0.5;
	float a = pow(ray.x,2) + pow(ray.z,2);
	float b = 2*(origin.x-center.x)*ray.x + 2*(origin.z-center.z)*ray.z;
	float c = pow(origin.x-center.x,2) + pow(origin.z-center.z,2) - pow(radius,2);
	float det = b*b - 4*a*c;

	if(det > 0)//Two collisions
	{
		//Find smaller scalar (nearest collision), which is greater than zero
		det = sqrt(det);
		float head = -b;
		float d;
		float d1 = (head + det)/(2*a);
		float d2 = (head - det)/(2*a);

		d = min(d1,d2);
		if(d < 0)
			d = max(d1,d2);
			
		vec3 pos = origin + d*ray;
		
		if(pos.y > center.y - height*0.5 && pos.y < center.y + height*0.5)
		{			
			//Ensure distance is less that previous and > 0
			if(d < hit.distance && d > EPSILON)
			{			
				hit.distance = d;
				//hit.position = origin + hit.distance*ray;
				//hit.normal = normalize(vec3(hit.position.x-center.x, 0, hit.position.z-center.z));
			}		
		}
	}
	
	//Bottom
	vec3 down = vec3(0,-1,0);
	center.y -= height*0.5;
	if(dot(ray,down) < 0)
	{
		float d = dot((center - origin), down/dot(ray,down));		
		vec3 pos = origin + d*ray;		
		if(distance(pos, center) < radius)
		{
			if(d < hit.distance && d > EPSILON)
			{
				hit.distance = d;
				//hit.position = origin + hit.distance*ray;
				//hit.normal = down;
			}
		}
	}
}

void TestTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 ray, vec3 origin)
{
	vec3 normal = normalize(cross(p2-p1, p3-p1));
	if(dot(ray, normal) != 0)
	{		
		vec3 p1p2 = p2 - p1;
		vec3 p1p3 = p3 - p1;
		vec3 pvec = cross(ray, p1p3);
		
		float invDet = 1/ dot(p1p2, pvec);
		
		vec3 tvec = origin - p1;
		float u = dot(tvec, pvec) * invDet;
		
		vec3 qvec = cross(tvec, p1p2);
		float v = dot(ray, qvec) * invDet;
		
		//If u,v > 0 and u+v < 1, get distance d
		if(u >= 0 && v >= 0 && u+v <= 1)
		{
			float d = dot(p1p3, qvec) * invDet;
			
			if(d < hit.distance && d > EPSILON)
			{
				hit.distance = d;
				//hit.position = origin + d*ray;
				//hit.normal = normal;
				//hit.material = mat;
			}
		}
	}
}

