#version 330 core
out vec4 FragColor;

//Mapas de luces, áreas que reciben luz
//Mapas difusos, áreas que no reciben

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	//luz ambiental
	//float ambientStrength = 0.1;
	vec3 ambiental = light.ambient * texture( material.diffuse, TexCoords).rgb;

	//luz difusa
	vec3 difusa = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(difusa, lightDir), 0.0);
	vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb;

	//luz especular
	//float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectorDir = reflect(-lightDir, difusa);
	float spec = pow(max(dot(viewDir, reflectorDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture( material.specular, TexCoords).rgb;

	//Spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	//Atenuación
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambiental *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//convoluciones 
	vec3 result = ambiental + diffuse + specular;
	FragColor = vec4(result, 1.0);

}