#version 330 core
out vec4 FragColor;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	//luz ambiental
	//float ambientStrength = 0.1;
	vec3 ambiental = light.ambient * material.ambient;

	//luz difusa
	vec3 difusa = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(difusa, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//luz especular
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectorDir = reflect(-lightDir, difusa);
	float spec = pow(max(dot(viewDir, reflectorDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	//convoluciones 
	vec3 result = ambiental + diffuse + specular;
	FragColor = vec4(result, 1.0);

}