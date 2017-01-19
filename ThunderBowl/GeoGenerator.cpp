#include "GeoGenerator.h"
#include "CommonIncludes.h"
#include "GameObject.h"

Mesh GeoGenerator::MakeSphere(float radius, int lattitudes, int longitudes)
{
	Mesh result;

	//Top point
	vec3 pole = vec3(0, 1, 0);
	result.positions.push_back(pole*radius);
	result.normals.push_back(pole);
	result.texcoords.push_back(vec2(0.5, 1.0));

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
			result.normals.push_back(pole);
			result.texcoords.push_back(vec2(0 + (float)i / longitudes, 1.0 - (float)i / lattitudes));

			pole = yaw * pole;

			if (j == longitudes - 1)
			{
				result.positions.push_back(pole*radius);
				result.normals.push_back(pole);
				result.texcoords.push_back(vec2(1.0, 1.0 - (float)i / lattitudes));
			}
		}
	}

	//Bottom point
	result.positions.push_back(vec3(0,-radius,0));
	result.normals.push_back(vec3(0,-1,0));
	result.texcoords.push_back(vec2(0.5, 0.0));

	//Indicies - top cap
	for (int i = 0; i < longitudes; i++)
	{
		result.indicies.push_back(0);
		result.indicies.push_back(i + 1);
		result.indicies.push_back(i + 2);
	}

	//Indicies - rings
	for (int i = 0; i < lattitudes - 1; i++)
	{
		for(int j = 0; j < longitudes; j++)
		{
			result.indicies.push_back((i)*(longitudes)+j+1);
			result.indicies.push_back((i+1)*(longitudes)+j+1);
			result.indicies.push_back((i+1)*(longitudes)+j+2);

			result.indicies.push_back((i)*(longitudes)+j + 1);
			result.indicies.push_back((i + 1)*(longitudes)+j + 2);
			result.indicies.push_back((i)*(longitudes)+j + 2);
			
		}
	}

	//Indicies - bottom cap
	for (int i = 0; i < longitudes; i++)
	{
		result.indicies.push_back(result.positions.size()-1);
		result.indicies.push_back((lattitudes - 1) * (longitudes + 1) + i + 2);
		result.indicies.push_back((lattitudes - 1) * (longitudes + 1) + i + 1);
	}

	result.colors = result.normals;
	result.elementCount = result.positions.size();

	return result;
}

Mesh GeoGenerator::MakePlane(float width, float height, int widthSegs, int heightSegs)
{
	Mesh result;

	//Prep
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
			result.normals.push_back(vec3(0,1,0));
			result.texcoords.push_back(vec2(0+(float)i/widthSegs, 1-(float)j/heightSegs));
		}
	}

	//Indicies
	for(int i = 0; i < widthSegs; i++)
	{
		for(int j = 0; j < heightSegs; j++)
		{
			result.indicies.push_back(i*heightVerts + j);
			result.indicies.push_back(i*heightVerts + j + 1);
			result.indicies.push_back((i + 1)*heightVerts + j + 1);

			result.indicies.push_back(i*heightVerts + j);
			result.indicies.push_back((i + 1)*heightVerts + j + 1);
			result.indicies.push_back((i + 1)*heightVerts + j);
		}
	}

	result.elementCount = result.positions.size();
	return result;
}
