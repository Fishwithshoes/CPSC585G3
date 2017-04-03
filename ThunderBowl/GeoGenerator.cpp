#include "GeoGenerator.h"
#include "CommonIncludes.h"
#include "GameObject.h"

//3D - For World and Particle GameObjects
Mesh GeoGenerator::MakeSphere(float radius, int lattitudes, int longitudes, bool insideOut)
{
	Mesh result;
	float flip = insideOut ? -1 : 1;

	//Top point
	vec3 pole = vec3(0, 1, 0);
	result.positions.push_back(pole*radius);
	result.normals.push_back(pole*flip);
	result.texcoords.push_back(vec2(0.5, 1.0)*flip);

	//Prep
	float pitchTheta = Mathf::PI / (lattitudes+1);
	float yawTheta = 2 * Mathf::PI / (longitudes);

	mat3 pitch = mat3(
		1, 0, 0,
		0, cos(pitchTheta), -sin(pitchTheta),
		0, sin(pitchTheta), cos(pitchTheta));

	mat3 yaw = mat3(
		cos(yawTheta), 0, sin(yawTheta),
		0, 1, 0,
		-sin(yawTheta), 0, cos(yawTheta));

	//Main construction
	for (int i = 0; i < lattitudes; i++)
	{
		pole = pitch * pole;

		for (int j = 0; j < longitudes; j++)
		{
			result.positions.push_back(pole*radius);
			result.normals.push_back(pole*flip);
			result.texcoords.push_back(vec2(0 + (float)j / longitudes, 1.0 - (float)i / lattitudes)*flip);

			pole = yaw * pole;

			if (j == longitudes - 1)
			{
				result.positions.push_back(pole*radius);
				result.normals.push_back(pole*flip);
				result.texcoords.push_back(vec2(1.0, 1.0 - (float)i / lattitudes)*flip);
			}
		}
	}

	//Bottom point
	result.positions.push_back(vec3(0,-radius,0));
	result.normals.push_back(vec3(0,-1,0)*flip);
	result.texcoords.push_back(vec2(0.5, 0.0)*flip);

	//GENERATE INDICIES
	if (insideOut)
	{
		//Indicies - top cap
		for (int i = 0; i < longitudes; i++)
		{
			result.indices.push_back(0);
			result.indices.push_back(i + 2);
			result.indices.push_back(i + 1);
		}

		//Indicies - rings
		for (int i = 0; i < lattitudes - 1; i++)
		{
			for (int j = 0; j < longitudes; j++)
			{
				result.indices.push_back((i)*(longitudes + 1) + j + 1);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 2);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 1);

				result.indices.push_back((i)*(longitudes + 1) + j + 1);
				result.indices.push_back((i)*(longitudes + 1) + j + 2);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 2);
			}
		}

		//Indicies - bottom cap
		for (int i = 0; i < longitudes; i++)
		{
			result.indices.push_back(result.positions.size() - 1);
			result.indices.push_back((lattitudes - 1) * (longitudes + 1) + i + 1);
			result.indices.push_back((lattitudes - 1) * (longitudes + 1) + i + 2);
		}
	}
	else
	{
		//Indicies - top cap
		for (int i = 0; i < longitudes; i++)
		{
			result.indices.push_back(0);
			result.indices.push_back(i + 1);
			result.indices.push_back(i + 2);
		}

		//Indicies - rings
		for (int i = 0; i < lattitudes - 1; i++)
		{
			for (int j = 0; j < longitudes; j++)
			{
				result.indices.push_back((i)*(longitudes + 1) + j + 1);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 1);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 2);

				result.indices.push_back((i)*(longitudes + 1) + j + 1);
				result.indices.push_back((i + 1)*(longitudes + 1) + j + 2);
				result.indices.push_back((i)*(longitudes + 1) + j + 2);
			}
		}

		//Indices - bottom cap
		for (int i = 0; i < longitudes; i++)
		{
			result.indices.push_back(result.positions.size() - 1);
			result.indices.push_back((lattitudes - 1) * (longitudes + 1) + i + 2);
			result.indices.push_back((lattitudes - 1) * (longitudes + 1) + i + 1);
		}
	}

	result.colors = result.normals;
	result.elementCount = result.positions.size();

	return result;
}

Mesh GeoGenerator::MakePlane(float width, float height, int widthSegs, int heightSegs, bool upsideDown)
{
	Mesh result;

	//Prep
	float flip = upsideDown ? -1.0 : 1.0;

	float westStart = -width*0.5;
	float northStart = height*0.5;
	
	float widthStep = width/widthSegs;
	float heightStep = height/heightSegs;
	
	int widthVerts = widthSegs + 1;
	int heightVerts = heightSegs + 1;
	
	//Main construction
	for(int i = 0; i < widthVerts; i++)
	{
		for(int j = 0; j < heightVerts; j++)
		{
			result.positions.push_back(vec3(westStart+widthStep*i, 0, northStart-heightStep*j));
			result.colors.push_back(vec3((float)i/widthSegs,1,(float)j/heightSegs));
			result.normals.push_back(flip * vec3(0,1,0));
			result.texcoords.push_back(vec2(0+(float)i/widthSegs, 1-(float)j/heightSegs));
		}
	}
	
	//Indices
	for(int i = 0; i < widthSegs; i++)
	{
		for(int j = 0; j < heightSegs; j++)
		{

			if (!upsideDown)
			{
				result.indices.push_back(i*heightVerts + j);
				result.indices.push_back(i*heightVerts + j + 1);
				result.indices.push_back((i + 1)*heightVerts + j + 1);

				result.indices.push_back(i*heightVerts + j);
				result.indices.push_back((i + 1)*heightVerts + j + 1);
				result.indices.push_back((i + 1)*heightVerts + j);
			}
			else
			{
				result.indices.push_back(i*heightVerts + j);
				result.indices.push_back((i + 1)*heightVerts + j + 1);
				result.indices.push_back(i*heightVerts + j + 1);

				result.indices.push_back(i*heightVerts + j);
				result.indices.push_back((i + 1)*heightVerts + j);
				result.indices.push_back((i + 1)*heightVerts + j + 1);
			}
		}
	}
	
	result.elementCount = result.positions.size();
	return result;
}

Mesh GeoGenerator::MakeCylinder(float startRadius, float endRadius, float height, int segments, bool upright)
{
	Mesh result;

	//Prep
	vec3 pole = vec3(0, height*0.5, -1);
	float theta = 2 * Mathf::PI / segments;

	mat3 yaw = mat3(
		cos(theta), 0, sin(theta),
		0, 1, 0,
		-sin(theta), 0, cos(theta));

	//Top point
	result.positions.push_back(vec3(0, height*0.5, 0));
	result.colors.push_back(vec3(0, height*0.5, 0));
	result.normals.push_back(vec3(0, 1, 0));
	result.texcoords.push_back(vec2(0.5, 0.5));

	//Bottom point
	result.positions.push_back(vec3(0, -height*0.5, 0));
	result.colors.push_back(vec3(0, -height*0.5, 0));
	result.normals.push_back(vec3(0, -1, 0));
	result.texcoords.push_back(vec2(0.5, 0.5));

	//Top
	for (int i = 0; i < segments * 2 + 2; i++)
	{
		result.positions.push_back(vec3(pole.x*endRadius, pole.y, pole.z*endRadius));
		result.colors.push_back(pole);
		if (i < segments + 1)
		{
			result.normals.push_back(vec3(0, 1, 0));
			result.texcoords.push_back(vec2(pole.x*0.5+0.5, pole.z*0.5+0.5));
		}
		else
		{
			result.normals.push_back(vec3(pole.x, 0, pole.z));
			result.texcoords.push_back(vec2((float)(i - segments - 1) / (segments), 1));
		}
		pole = yaw * pole;
	}
	pole = vec3(0, -height*0.5, -1);

	//Bottom
	for (int i = 0; i < segments*2+2; i++)
	{
		result.positions.push_back(vec3(pole.x*startRadius, pole.y, pole.z*startRadius));
		result.colors.push_back(pole);
		if (i < segments + 1)
		{
			result.normals.push_back(vec3(0, -1, 0));
			result.texcoords.push_back(vec2(pole.x*0.5 + 0.5, pole.z*0.5 + 0.5));
		}
		else
		{
			result.normals.push_back(vec3(pole.x, 0, pole.z));
			result.texcoords.push_back(vec2((float)(i - segments - 1) / (segments), 0));
		}
		pole = yaw * pole;
	}

	if (!upright)
	{
		float theta = Mathf::PI * 0.5;

		mat3 roll = mat3(
			cos(theta), sin(theta), 0,
			-sin(theta), cos(theta), 0,
			0,0,1);

		for (int i = 0; i < result.positions.size(); i++)
		{
			result.positions[i] = roll * result.positions[i];
			result.normals[i] = roll * result.normals[i];
		}
	}

	//Indices
	for (int i = 0; i < segments; i++)
	{
		//Top
		result.indices.push_back(i + 2);
		result.indices.push_back(i + 3);
		result.indices.push_back(0);

		//Body
		result.indices.push_back(i + 2 + segments + 1);
		result.indices.push_back(i + 2 + segments*3 + 3);
		result.indices.push_back(i + 2 + segments*3 + 3 + 1);
		
		result.indices.push_back(i + 2 + segments + 1 + 1);
		result.indices.push_back(i + 2 + segments + 1);
		result.indices.push_back(i + 2 + segments * 3 + 3 + 1);
		
		//Bottom
		result.indices.push_back(i + 2 + segments*2 + 2 + 1);
		result.indices.push_back(i + 2 + segments*2 + 2);
		result.indices.push_back(1);
	}

	result.elementCount = result.positions.size();
	return result;
}

Mesh GeoGenerator::MakeBox(float width, float height, float depth, bool insideOut)
{
	Mesh result;

	//Prep
	float flip = insideOut ? -1 : 1;
	vec3 _0 = vec3(-width*0.5, height*0.5, depth*0.5);
	vec3 _1 = vec3(-width*0.5, height*0.5, -depth*0.5);
	vec3 _2 = vec3(width*0.5, height*0.5, -depth*0.5);
	vec3 _3 = vec3(width*0.5, height*0.5, depth*0.5);

	vec3 _4 = vec3(-width*0.5, -height*0.5, depth*0.5);
	vec3 _5 = vec3(-width*0.5, -height*0.5, -depth*0.5);
	vec3 _6 = vec3(width*0.5, -height*0.5, -depth*0.5);
	vec3 _7 = vec3(width*0.5, -height*0.5, depth*0.5);

	//Positions
	//Top
	result.positions.push_back(_0);
	result.positions.push_back(_1);
	result.positions.push_back(_2);
	result.positions.push_back(_3);
	
	//Front
	result.positions.push_back(_1);
	result.positions.push_back(_5);
	result.positions.push_back(_6);
	result.positions.push_back(_2);
	
	//Right
	result.positions.push_back(_2);
	result.positions.push_back(_6);
	result.positions.push_back(_7);
	result.positions.push_back(_3);
	
	//Back
	result.positions.push_back(_3);
	result.positions.push_back(_7);
	result.positions.push_back(_4);
	result.positions.push_back(_0);
	
	//Left
	result.positions.push_back(_0);
	result.positions.push_back(_4);
	result.positions.push_back(_5);
	result.positions.push_back(_1);
	
	//bottom
	result.positions.push_back(_5);
	result.positions.push_back(_4);
	result.positions.push_back(_7);
	result.positions.push_back(_6);
	
	//Colors
	for (int i = 0; i < 24; i++)
		result.colors.push_back(vec3(Random::rangef(0, 1), Random::rangef(0, 1), Random::rangef(0, 1)));
	
	//Normals
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(0, 1, 0)*flip);
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(0, 0, -1)*flip);
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(1, 0, 0)*flip);
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(0, 0, 1)*flip);
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(-1, 0, 0)*flip);
	for (int i = 0; i < 4; i++)
		result.normals.push_back(vec3(0, -1, 0)*flip);
	
	//Texcoords
	for (int i = 0; i < 6; i++)
	{
		result.texcoords.push_back(vec2(0, 1)*flip);
		result.texcoords.push_back(vec2(0, 0)*flip);
		result.texcoords.push_back(vec2(1, 0)*flip);
		result.texcoords.push_back(vec2(1, 1)*flip);
	}
	
	//Indices
	if (insideOut)
	{
		for (int i = 0; i < 6; i++)
		{
			result.indices.push_back((GLuint)(0 + i * 4));
			result.indices.push_back((GLuint)(2 + i * 4));
			result.indices.push_back((GLuint)(1 + i * 4));			
			result.indices.push_back((GLuint)(0 + i * 4));
			result.indices.push_back((GLuint)(3 + i * 4));
			result.indices.push_back((GLuint)(2 + i * 4));			
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			result.indices.push_back((GLuint)(0 + i * 4));
			result.indices.push_back((GLuint)(1 + i * 4));
			result.indices.push_back((GLuint)(2 + i * 4));
			result.indices.push_back((GLuint)(0 + i * 4));
			result.indices.push_back((GLuint)(2 + i * 4));
			result.indices.push_back((GLuint)(3 + i * 4));
		}
	}
	
	result.elementCount = result.positions.size();
	return result;
}

Mesh GeoGenerator::MakeParticle()
{
	Mesh result;

	//Main construction
	result.positions.push_back(vec3(-1.0, 1.0, 0.0));
	result.colors.push_back(vec3(1.0, 0.0, 0.0));
	result.normals.push_back(vec3(0.0, 0.0, 1.0));
	result.texcoords.push_back(vec2(1.0, 1.0));

	result.positions.push_back(vec3(1.0, 1.0, 0.0));
	result.colors.push_back(vec3(0.0, 1.0, 0.0));
	result.normals.push_back(vec3(0.0, 0.0, 1.0));
	result.texcoords.push_back(vec2(0.0, 1.0));

	result.positions.push_back(vec3(1.0, -1.0, 0.0));
	result.colors.push_back(vec3(0.0, 0.0, 1.0));
	result.normals.push_back(vec3(0.0, 0.0, 1.0));
	result.texcoords.push_back(vec2(0.0, 0.0));

	result.positions.push_back(vec3(-1.0, -1.0, 0.0));
	result.colors.push_back(vec3(1.0, 1.0, 0.0));
	result.normals.push_back(vec3(0.0, 0.0, 1.0));
	result.texcoords.push_back(vec2(1.0, 0.0));

	//Indicies
	result.indices.push_back(0);
	result.indices.push_back(1);
	result.indices.push_back(3);

	result.indices.push_back(1);
	result.indices.push_back(2);
	result.indices.push_back(3);

	result.elementCount = result.positions.size();
	return result;
}

Mesh GeoGenerator::MakeThunderbowl(int curveSegments, int revolveSegments, bool insideOut)
{
	//Control Points
	vector<vec3> cPoints = {};
	cPoints.push_back(vec3(6, 0, 0));
	cPoints.push_back(vec3(6, 8, 0));
	cPoints.push_back(vec3(8, 9, 0));

	cPoints.push_back(vec3(12, 6, 0));
	cPoints.push_back(vec3(20, 4, 0));

	cPoints.push_back(vec3(30, 8, 0));
	cPoints.push_back(vec3(38, 16, 0));
	
	cPoints.push_back(vec3(40, 22, 0));
	cPoints.push_back(vec3(32, 16, 0));
	
	cPoints.push_back(vec3(16, 4, 0));
	cPoints.push_back(vec3(6, 0, 0));

	int strides = cPoints.size() / 2;

	for (int i = 0; i < cPoints.size(); i++)
	{
		cPoints[i].x *= 3;
		cPoints[i].y *= 2;
		cPoints[i].z *= 3;
	}

	//Prep
	Mesh result;
	float flip = insideOut ? -1.0 : 1.0;
	float theta = 2 * Mathf::PI / revolveSegments;

	mat3 yaw = mat3(
		cos(theta), 0, sin(theta),
		0, 1, 0,
		-sin(theta), 0, cos(theta));

	//Main construction
	vector<vec3> curvePoints = {};

	for (int i = 0; i < strides; i++)
	{
		for (int j = 0; j < curveSegments; j++)
		{
			float u = (float)j / (curveSegments-1);
			//[(1-u)+u]^2
			//(1-u)^2 + 2u(1-u) + u^2
			vec3 point = (1 - u)*(1 - u)*cPoints[i*2] + 2 * u*(1 - u)*cPoints[i*2+1] + u*u*cPoints[i*2+2];
			curvePoints.push_back(point);
		}
	}

	for (int i = 0; i < revolveSegments+1; i++)
	{
		for (int j = 0; j < curvePoints.size(); j++)
		{
			result.positions.push_back(curvePoints[j]);
			result.colors.push_back(vec3(1, 0, 0));
			result.normals.push_back(normalize(flip*vec3(curvePoints[j])));
			result.texcoords.push_back(vec2((float)i/revolveSegments, (float)j/curvePoints.size()));

			curvePoints[j] = yaw * curvePoints[j];
		}
	}

	//Indices
	int quadCount = (curvePoints.size() - 1)*(revolveSegments + 1) -revolveSegments;
	for (int i = 0; i < quadCount; i++)
	{
		if (insideOut)
		{
			result.indices.push_back(i);
			result.indices.push_back(i + curvePoints.size());
			result.indices.push_back(i + 1);

			result.indices.push_back(i + curvePoints.size());
			result.indices.push_back(i + curvePoints.size() + 1);
			result.indices.push_back(i + 1);
		}
		else
		{
			result.indices.push_back(i);
			result.indices.push_back(i + 1);
			result.indices.push_back(i + curvePoints.size());

			result.indices.push_back(i + curvePoints.size());
			result.indices.push_back(i + 1);
			result.indices.push_back(i + curvePoints.size() + 1);
		}
	}

	result.elementCount = result.positions.size();

	cout << "MINE " << result.indices.size() << endl;

	return result;
}

//2D - For Overlay GameObjects
Mesh GeoGenerator::MakeCircle(float sweep, int segments, float radius)
{
	Mesh result;

	//Prep
	float step = -2 * Mathf::PI * sweep / segments;
	vec3 pole = vec3(0, 1, 0);

	mat3 roll = mat3(
		cos(step), -sin(step), 0,
		sin(step), cos(step), 0,
		0, 0, 1);

	//Main construction
	for (int i = 0; i < segments+1; i++)
	{
		result.positions.push_back(pole*radius);
		result.colors.push_back(pole);
		result.normals.push_back(vec3(0, 0, -1));
		result.texcoords.push_back(vec2(pole.x+1, pole.y+1) * (float)0.5);
		pole = roll * pole;
	}
	result.positions.push_back(vec3(0));
	result.colors.push_back(vec3(0));
	result.normals.push_back(vec3(0, 0, -1));
	result.texcoords.push_back(vec2(0.5, 0.5));

	//Indices
	for (int i = 0; i < segments; i++)
	{
		result.indices.push_back(i);
		result.indices.push_back(i+1);
		result.indices.push_back(segments+1);
	}

	result.elementCount = result.positions.size();
	return result;
}

Mesh GeoGenerator::MakeRect(float width, float height, GeoAnchors anchor)
{
	Mesh result;

	//Main construction
	result.positions.push_back(vec3(-width*0.5, height*0.5, 0));
	result.positions.push_back(vec3(width*0.5, height*0.5, 0));
	result.positions.push_back(vec3(width*0.5, -height*0.5, 0));
	result.positions.push_back(vec3(-width*0.5, -height*0.5, 0));
	
	for (int i = 0; i < result.positions.size(); i++)
	{
		switch (anchor)
		{
		case GA_CENTER:
			//No effect
			break;
		case GA_LEFT:
			result.positions[i] += vec3(width*0.5, 0, 0);
			break;
		case GA_RIGHT:
			result.positions[i] -= vec3(width*0.5, 0, 0);
			break;
		case GA_TOP:
			result.positions[i] -= vec3(0, height*0.5, 0);
			break;
		case GA_BOTTOM:
			result.positions[i] += vec3(0, height*0.5, 0);
			break;
		default:
			cout << "Illegal GeoAnchor used in GeoGenerator::MakeRect!" << endl;
			break;
		}
	}
	
	result.colors.push_back(vec3(0, 1, 0));
	result.colors.push_back(vec3(1, 1, 0));
	result.colors.push_back(vec3(1, 0, 0));
	result.colors.push_back(vec3(0, 0, 0));

	result.normals.push_back(vec3(0, 0, -1));
	result.normals.push_back(vec3(0, 0, -1));
	result.normals.push_back(vec3(0, 0, -1));
	result.normals.push_back(vec3(0, 0, -1));

	result.texcoords.push_back(vec2(0, 1));
	result.texcoords.push_back(vec2(1, 1));
	result.texcoords.push_back(vec2(1, 0));
	result.texcoords.push_back(vec2(0, 0));

	//Indices
	result.indices.push_back(0);
	result.indices.push_back(3);
	result.indices.push_back(1);

	result.indices.push_back(1);
	result.indices.push_back(3);
	result.indices.push_back(2);

	result.elementCount = result.positions.size();
	return result;
}