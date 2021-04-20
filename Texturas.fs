#version 330 core
out vec4 FragColor;

in vec3 miColor;
in vec2 miTextura;

//Ejemplo de textura
uniform sampler2D textura1;
uniform sampler2D textura2;

void main()
{
	FragColor = mix(texture(textura1, miTextura), textura(textura2, miTextura), 0.3f);
}