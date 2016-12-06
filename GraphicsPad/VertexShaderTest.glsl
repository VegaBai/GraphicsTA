#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

out vec3 theColor;

uniform mat4 mvp;	// model to view matrix

void main()
{
	vec4 v = vec4(position, 1.0);
 	gl_Position = mvp * v;
	theColor = vertexColor;

}