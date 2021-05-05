#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

//out vec3 miColor;
out vec2 miTextura;

uniform mat4 transform;

void main() 
{
	//gl_Position = vec4(aPos, 1.0f);
	gl_Position = transform * vec4(aPos, 1.0f);
	//miColor = aColor;
	miTextura = vec2(aTexCoord.x, aTexCoord.y);
}