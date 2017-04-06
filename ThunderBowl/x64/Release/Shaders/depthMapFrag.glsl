#version 450

in vec3 Position;

layout(location = 0) out vec4 fragDepth;

void main(void)
{
	//fragDepth = gl_FragCoord.z;
	//fragDepth = 0;
	//gl_FragDepth = 0.5;
	//fragDepth = 0.5;
	
	float distToLight = distance(vec3(5,2,5)*50, Position);
	
	fragDepth = vec4(vec3(distToLight*0.001), 1.0);
}