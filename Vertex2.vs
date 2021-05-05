#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform2;

void main()
{
	gl_Position = transform2 * vec4(aPos, 1.0);
}