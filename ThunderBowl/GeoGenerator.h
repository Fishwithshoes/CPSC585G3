#pragma once
#include "Structs.h"

class GeoGenerator
{
public:
	//3D - For World and Particle GameObjects
	static Mesh MakeSphere(float radius, int lattitudes, int longitudes, bool insideOut);
	static Mesh MakePlane(float width, float height, int widthSegs, int heightSegs);
	static Mesh MakeCylinder(float radius, float height, int segments);
	static Mesh MakeBox(float width, float height, float depth, bool insideOut);

	//2D - For Overlay GameObjects
	static Mesh MakeCircle(float sweep, int segments, float radius);//sweep from 0-1
protected:

private:
};