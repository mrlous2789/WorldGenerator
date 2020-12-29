
#version 400 core

layout(location = 0) in vec3 vertexPosition_modelspace;


//out vec3 fragColor;

void main()
{	
	//fragColor = color;
	gl_Position.xyz = vertexPosition_modelspace;
	gl_Position.w = 1.0;
}
