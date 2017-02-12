#version 450

in layout (location = 0) vec3 VertexPosition;
in layout (location = 1) vec3 VertexColor;
in layout (location = 2) vec3 VertexNormal;
in layout (location = 3) vec2 VertexTexCoord;

out vec3 Position;
out vec3 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(VertexPosition, 1.0);
	
	Position = VertexPosition;
	Color = VertexColor;
	TexCoord = VertexTexCoord;
}