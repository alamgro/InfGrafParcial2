#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	//luz ambiental
	float ambientStrength = 0.1;
	vec3 ambiental = ambientStrength * lightColor;

	//luz difusa
	vec3 difusa = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(difusa, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//luz especular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectorDir = reflect(-lightDir, difusa);
	float spec = pow(max(dot(viewDir, reflectorDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	//convoluciones 
	vec3 result = (ambiental + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);

}