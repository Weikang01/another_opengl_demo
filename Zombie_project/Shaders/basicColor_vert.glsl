#version 440 core
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in vec2 vertexCoord;

out vec2 vert_texCoord;
out vec4 vert_color;

uniform mat4 P;

void main()
{
	gl_Position.xy = (P * vec4(vertexPosition, 0.f, 1.f)).xy;
	gl_Position.z = 0.;
	gl_Position.w = 1.;
	vert_texCoord = vertexCoord;
	vert_color = vertexColor;
}
