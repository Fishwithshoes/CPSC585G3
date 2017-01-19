#pragma once
#include "Structs.h"

class GeoGenerator
{
public:
	static Mesh MakeSphere(float radius, int lattitudes, int longitudes);
	static Mesh MakePlane(float width, float height, int widthSegs, int heightSegs);
	static Mesh MakeCylinder();
protected:

private:
};
