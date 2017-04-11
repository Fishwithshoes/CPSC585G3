#pragma once
#include "Structs.h"

enum GeoAnchors
{
	GA_CENTER,
	GA_LEFT,
	GA_RIGHT,
	GA_TOP,
	GA_BOTTOM
};

class GeoGenerator
{
public:
	//3D - For World and Particle GameObjects
	static Mesh MakeSphere(float radius, int lattitudes, int longitudes, bool insideOut);
	static Mesh MakePlane(float width, float height, int widthSegs, int heightSegs, bool upsideDown);
	static Mesh MakeCylinder(float startRadius, float endRadius, float height, int segments, bool upright);
	static Mesh MakeBox(float width, float height, float depth, bool insideOut);
	static Mesh MakeThunderbowl(int curveSegments, int revolveSegments, bool insideOut);
	static Mesh MakeParticle();

	//2D - For Overlay GameObjects
	static Mesh MakeCircle(float sweep, int segments, float radius);//sweep from 0-1
	static Mesh MakeRect(float width, float height, GeoAnchors anchor, float inset);
protected:

private:
};