
#version 400 core

layout(location = 0) in vec3 vertexPosition_modelspace;
uniform mat4 mvp;

//out vec3 fragColor;

void main()
{	
	//fragColor = color;
	gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0);
	gl_Position.w = 1.0;
}
