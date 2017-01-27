#version 450

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec2 TexCoord;

//Lightweight PBR Props
uniform float diffuseLevel;
uniform vec3 diffuseColor;
uniform float reflectivity;
uniform vec3 reflectColor;
uniform float roughness;
uniform float curveShape;
uniform float _0degRef;
uniform float _90degRef;
uniform float bumpLevel;
uniform float selfIllumLevel;
uniform vec3 selfIllumColor;

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

//Texture Coords
uniform vec2 tileUV;
uniform vec2 offsetUV;

uniform vec3 cameraPos;
uniform vec4 lightPos;
uniform vec4 lightColor;

out vec4 FragmentColor;

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
const int SHADOW_SAMPLES = 4;//4 is quite high enough, methinks!
const float GLOSSY_STRETCH = 0.5;//Set to 0 i GLOSSY_SAMPLES = 1;
const int GLOSSY_SAMPLES = 8;//Again, 4 should satisfy thee!
//Eat memory, gain perfomance!
const float SHADOW_CONTRIB = 1.0/(SHADOW_SAMPLES*SHADOW_SAMPLES);
const float GLOSSY_CONTRIB = 1.0/(GLOSSY_SAMPLES*GLOSSY_SAMPLES);
const float SHADOW_STRIDE = LIGHT_RADIUS/SHADOW_SAMPLES * 2.0;
const float GLOSSY_STRIDE = GLOSSY_STRETCH/GLOSSY_SAMPLES * 2.0;

void RunSceneCollisions(vec3 ray, vec3 origin);

void TestPlane(vec3 p0, vec3 normal, vec3 ray, vec3 origin);
void TestSphere(vec3 center, float radius, vec3 ray, vec3 origin);
void TestCylinder(vec3 center, float radius, float height, vec3 ray, vec3 origin);
void TestTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 ray, vec3 origin);

void main()
{
	float ambientLevel = 0.2;
	
	//WORLD CALCULATIONS
	vec3 lightDir = normalize(lightPos.xyz-Position);
	float lightDist = length(lightPos.xyz-Position);
	vec3 viewDir = normalize(cameraPos-Position);
	float viewDist = length(cameraPos-Position);
	
	//TILE AND OFFSET UVs
	vec2 _texCoord = TexCoord * tileUV + offsetUV;
	
	//GET MAP TEXTELS
	vec3 diffuseTex = texture2D(diffuseMap, _texCoord).xyz;
	vec3 normalTex = texture2D(normalMap, _texCoord).xyz;
	vec3 roughnessTex = texture2D(roughnessMap, _texCoord).xyz;
	
	//SEE THE NORMALS. BE THE NORMALS. QUICK AND DIRTY VARIANT.
	normalTex -= vec3(0.5, 0.5, 1.0);
	vec3 normalDir = normalize(Normal + normalTex*bumpLevel);
	
	//ENVIRONMENT MAPPING - FLAT IMAGE REFLECTION WITH BOX BLUR. WATCH THAT SAMPLE RATE!
	vec3 red = reflect(-viewDir, normalDir);
	vec3 mirrorTex = vec3(0);
	float totalStride = GLOSSY_STRETCH*roughness*roughnessTex.x;
	
	for(int i = 0; i < GLOSSY_SAMPLES; i++)
	{
		float x = red.x - totalStride + i*GLOSSY_STRIDE;
		float fadeX = 1.0 - abs(i - GLOSSY_SAMPLES*0.5)/(GLOSSY_SAMPLES*0.5);
		for(int j = 0; j < GLOSSY_SAMPLES; j++)
		{
			float y = red.y - totalStride + j*GLOSSY_STRIDE;
			float fadeY = 1.0 - abs(j - GLOSSY_SAMPLES*0.5)/(GLOSSY_SAMPLES*0.5);
			mirrorTex += texture2D(mirrorMap, vec2(x, y)).xyz * GLOSSY_CONTRIB * (fadeX + fadeY);
		}
	}
	
	//ENVIRONMENT MAPPING - CUBEMAP REFLECTION WITH BOX BLUR. WATCH THAT SAMPLE RATE!
	vec3 envTex = vec3(0);
	envTex = texture(envMap, red).xyz;
	
	
	//FRESNEL AND PHYSICAL CONSIDERATIONS
	float focus = clamp(dot(viewDir, normalDir), 0, 1);
	float edge = 1 - focus;
	float _reflectivity = (reflectivity-focus) * _90degRef * pow(edge, 5 - curveShape) + (_0degRef * reflectivity);
	_reflectivity = clamp(_reflectivity, 0, 1);	
	float _diffuseLevel = clamp(diffuseLevel - _reflectivity, 0, 1);
	
	//FINAL PSUEDO-PHONG LIGHTING CONTRIBUTIONS
	_diffuseLevel *= clamp(dot(lightDir, normalDir), 0, 1);
	vec3 diffuse = _diffuseLevel * diffuseColor * diffuseTex;
	vec3 specular = _reflectivity * lightColor.xyz * reflectColor * pow(clamp(dot(viewDir, reflect(-lightDir, normalDir)), 0, 1), (1.04-roughness*roughnessTex.x)*25) * (1-roughness*roughnessTex.x*0.6);
	vec3 reflection = _reflectivity * reflectColor * envTex;
	vec3 selfIllum = selfIllumLevel * selfIllumColor * envTex;
	vec3 rim = rimLevel * rimColor * clamp(dot(normalDir, lightDir), 0, 1) * pow(1 - clamp(dot(viewDir, normalDir), 0, 1), rimPower);
	
	//SHADOWS!! BUT WATCH THAT SAMPLE RATE!
	vec3 ray = lightDir;
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
				vec3 destination = lightPos.xyz;
				destination += -LIGHT_RADIUS*east + i*SHADOW_STRIDE*east;
				destination += -LIGHT_RADIUS*north + j*SHADOW_STRIDE*north;
				
				ray = destination-Position;
				RunSceneCollisions(ray, origin);
				
				if(hit.distance > EPSILON && hit.distance < lightDist)
				{	
					shadow += clamp(1-hit.distance, 0, 1) * SHADOW_CONTRIB;
				}
			}
		}
	}
	diffuse *= 1-shadow;
	specular *= 1-shadow;
	
	//MIX IN AMBIENT COLOR AND SUM CONTRIBUTORS
	diffuse += (1-_diffuseLevel)*envTex*ambientLevel*diffuseColor*diffuseTex;
	vec3 final = diffuse + specular + reflection + selfIllum + rim;
	
	//FOGGY FUGUE
	vec3 fogColor = vec3(0,0,0);	
	float u = viewDist*0.05;
	// final = final * (1-u) + fogColor * u;
	
	FragmentColor = vec4(final, 1.0);
	
	//DEBUG DANCING - COMMENT OUT FOR FINAL COLOR
	// FragmentColor = vec4(Color, 1);
	// FragmentColor = vec4(Normal, 1);
	// FragmentColor = vec4(normalDir, 1);
    // FragmentColor = vec4(TexCoord.x, 0, 0, 1);
	// FragmentColor = vec4(0, TexCoord.y, 0, 1);
	// FragmentColor = vec4(TexCoord.x, TexCoord.y, 0, 1);
	// FragmentColor = vec4(_reflectivity);
	// FragmentColor = vec4(roughness);
	// FragmentColor = vec4(viewDist*0.01);
	// FragmentColor = vec4(hit.distance);
	
	// FragmentColor = vec4(_diffuseLevel);
	// FragmentColor = vec4(_reflectivity);
	// FragmentColor = vec4(diffuse, 1.0);
	// FragmentColor = vec4(specular, 1.0);
	// FragmentColor = vec4(reflection, 1.0);
	// FragmentColor = vec4(selfIllum, 1.0);
	// FragmentColor = vec4(rim, 1.0);
	// FragmentColor = vec4(1-shadow);
	
	// FragmentColor = vec4(diffuseTex, 1.0);
	// FragmentColor = vec4(roughnessTex, 1.0);
	// FragmentColor = vec4(normalTex, 1.0);
	// FragmentColor = vec4(mirrorTex, 1.0);
	// FragmentColor = vec4(envTex, 1.0);
}

void RunSceneCollisions(vec3 ray, vec3 origin)
{
	hit.distance = 1000;
	
	// TestSphere(vec3(0,1,0), 1, ray, origin);
	TestCylinder(vec3(3, 2, 0), 1, 2, ray, origin);
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

