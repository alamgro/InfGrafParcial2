#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp> //operaciones avanzadas
#include <glm/gtc/matrix_transform.hpp> //transformaciones matriciales
#include <glm/gtc/type_ptr.hpp> //transformaciones de tipos de datos

#include <shader/shader.h>
#include <camara/camara.h>

#include <iostream>
#include <vector>


/*
Tenemos luz:
	-Ambiental: luz homogénea, que se va reflejar en un ángulo de 90° que va a iluminar todos los objetos de nuestro shader
	-Especular: es un punto de luz concentrado dentro de la figura. Entre más grande sea el valor, se concentrará más en un punto
	-combinada: Para crear efectos mejores
*/
using namespace glm;
using namespace std;

void Instantiate(Shader& _shader, vec3 _position);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale);
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis);

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana
unsigned int LoadTexture(const char* path);
unsigned int LoadCubeMap(vector<string> faces);

//medidas de la pantalla
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

//Número de vértices en la figura
const unsigned int cubeVertexNum = 100;

//camara
Camara camera(glm::vec3(0.0f, 5.0f, 30.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Mis texturas
unsigned int texBricks, texMetal, texGrass, texWood, texWhiteWood, texWool, texWool2, texStone, texStoneGray;

//Punto donde estará posicionada nuestra luz
vec3 posLuz(0.0f, 2.0f, 0.0f);

float tempCoord = 0.0f;

int main() {
	//inicializar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Buenas tardes", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en crear GLFW y la ventana" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//decirle a glfw que va a recibir señales de mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); //comprobacion del buffer z

	Shader nuestroShader("Cubo.vs", "Cubo.fs");
	Shader luzShader("Luz.vs", "Luz.fs");
	Shader skyboxShader("Skybox.vs", "Skybox.fs");

	float vertices[]{
		//posiciones		//cómo afecta
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int indices[]
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		14, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	float verticesSkybox[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//unir o linkear
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//luces
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Texturas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//segundo shader
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Skybox Shader
	unsigned int sVAO, sVBO;
	glGenVertexArrays(1, &sVAO);

	glGenBuffers(1, &sVBO);
	glBindVertexArray(sVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSkybox), &verticesSkybox, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<string> faces
	{
		("skybox/right.jpg"),
		("skybox/left.jpg"),
		("skybox/top.jpg"),
		("skybox/bottom.jpg"),
		("skybox/front.jpg"),
		("skybox/back.jpg")
	};

	//crear la textura
	unsigned int cubemapTexture = LoadCubeMap(faces);
	
	//Configurar el shader del skybox
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	unsigned int diffuseMap = LoadTexture("Box.png");
	unsigned int specularMap = LoadTexture("Box_Specular.png");

	//Mis texturas
	texMetal = LoadTexture("Metal.png");
	texGrass = LoadTexture("Grass.jpg");
	texWood = LoadTexture("Wood.jpg");
	texWhiteWood = LoadTexture("whiteWood.jpeg");
	texWool = LoadTexture("Wool.jpg");
	texWool2 = LoadTexture("Wool2.jpg");
	texStone = LoadTexture("Stone.jpg");
	texStoneGray = LoadTexture("StoneGray.jpg");

	nuestroShader.use();
	nuestroShader.setInt("material.diffuse", 0);
	nuestroShader.setInt("material.specular", 1);

	while (!glfwWindowShouldClose(window))
	{
		//calculo para el mouse para que pueda captar los movimeintos en tiempo real
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Renderizado
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpieza del buffer z

		//Propiedades de la luz
		nuestroShader.use();
		//nuestroShader.setVec3("light.position", posLuz);
		nuestroShader.setVec3("light.position", camera.Position);
		nuestroShader.setVec3("light.direction", camera.Front);
		nuestroShader.setFloat("light.cutOff", cos(radians(10.0f)));
		nuestroShader.setFloat("light.outerCutOff", cos(radians(32.0f)));
		nuestroShader.setVec3("viewPos", camera.Position);

		nuestroShader.setVec3("light.ambient", vec3(0.1f));
		nuestroShader.setVec3("light.diffuse", 0.9f, 0.9f, 0.9f);
		nuestroShader.setVec3("light.specular", vec3(0.3f));
		nuestroShader.setFloat("light.constant", 1.0f);
		nuestroShader.setFloat("light.linear", 0.1f);
		nuestroShader.setFloat("light.quadratic", 0.0032f);

		nuestroShader.setFloat("material.shininess", 32.0f);

		//posLuz = vec3(posLuz.x, posLuz.y, 2.0f - (glfwGetTime() * 0.25f));
		/*float radio = 3.0f;
		float multipVelocidad = 1.2f;
		posLuz = vec3((radio * cos(glfwGetTime() * multipVelocidad)), posLuz.y, posLuz.z);*/

		//nuestroShader.setVec3("lightPos", posLuz);
		//nuestroShader.setVec3("viewPos", camera.Position);


		//Propiedades avanzadas de la luz
		/*vec3 lightColor(1.0f);
		lightColor.x = sin(glfwGetTime() * 2.21f);
		lightColor.y = sin(glfwGetTime() * 1.33f);
		lightColor.z = sin(glfwGetTime() * 3.18f);
		vec3 diffuseColor = lightColor * vec3(0.5f);
		vec3 ambientColor = diffuseColor * vec3(0.2f);*/

		//mvp
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 300.0f);
		glm::mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		/*glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);*/

		glBindTexture(GL_TEXTURE_2D, texStone);

#pragma region MUROS EXTERIOR
		glBindTexture(GL_TEXTURE_2D, texGrass); //Poner textura por defecto
		Instantiate(nuestroShader, vec3(0.0f, 0.0f, -100.0f), vec3(300.0f, 1.0f, 300.0f)); //Pasto

		glBindTexture(GL_TEXTURE_2D, texStoneGray);
		//Pilares Gigantosos
		Instantiate(nuestroShader, vec3(17.5f, 25.5f, -60.5f), vec3(25.0f, 50.0f, 25.0f));
		Instantiate(nuestroShader, vec3(-17.5f, 25.5f, -60.5f), vec3(25.0f, 50.0f, 25.0f));
		Instantiate(nuestroShader, vec3(17.5f, 25.5f, -145.5f), vec3(25.0f, 50.0f, 25.0f));
		Instantiate(nuestroShader, vec3(-17.5f, 25.5f, -145.5f), vec3(25.0f, 50.0f, 25.0f));

		Instantiate(nuestroShader, vec3(0.0f, 39.0f, -103.0f), vec3(42.0f, 2.0f, 60.0f)); //Techo grandote
		Instantiate(nuestroShader, vec3(0.0f, 32.75f, -60.7f), vec3(10.0f, 15.0f, 24.0f));
		Instantiate(nuestroShader, vec3(0.0f, 20.05f, -135.5f), vec3(10.0f, 40.0f, 5.0f));

		Instantiate(nuestroShader, vec3(0.0f, 0.05f, -103.0f), vec3(42.0f, 1.0f, 60.0f)); //Piso interior
#pragma endregion

#pragma region PASARELA EXTERIOR
		glBindTexture(GL_TEXTURE_2D, texStone);
		Instantiate(nuestroShader, vec3(-8.0f, 18.75f, -18.0f), vec3(6.0f, 4.0f, 60.0f));
		Instantiate(nuestroShader, vec3(8.0f, 18.75f, -18.0f), vec3(6.0f, 4.0f, 60.0f));

		Instantiate(nuestroShader, vec3(-8.0f, 8.75f, 4.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-8.0f, 8.75f, -7.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-8.0f, 8.75f, -18.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-8.0f, 8.75f, -29.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-8.0f, 8.75f, -40.0f), vec3(5.0f, 18.0f, 5.0f));

		Instantiate(nuestroShader, vec3(8.0f, 8.75f, 4.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(8.0f, 8.75f, -7.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(8.0f, 8.75f, -18.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(8.0f, 8.75f, -29.0f), vec3(5.0f, 18.0f, 5.0f));
		Instantiate(nuestroShader, vec3(8.0f, 8.75f, -40.0f), vec3(5.0f, 18.0f, 5.0f));
#pragma endregion

#pragma region PAREDES
		Instantiate(nuestroShader, vec3(23.0f, 20.05f, -103.0f), vec3(5.0f, 40.0f, 60.0f));
		Instantiate(nuestroShader, vec3(-23.0f, 20.05f, -103.0f), vec3(5.0f, 40.0f, 60.0f));

#pragma endregion

#pragma region MESA
		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(0.0f, 1.5f, -103.0f), vec3(5.0f, 0.2f, 10.0f));
		Instantiate(nuestroShader, vec3(1.75f, 1.0f, -98.75f));
		Instantiate(nuestroShader, vec3(-1.75f, 1.0f, -98.75f));
		Instantiate(nuestroShader, vec3(-1.75f, 1.0f, -107.25f));
		Instantiate(nuestroShader, vec3(1.75f, 1.0f, -107.25f));
#pragma endregion

#pragma region SILLAS
		glBindTexture(GL_TEXTURE_2D, texWhiteWood);
		Instantiate(nuestroShader, vec3(-3.5f, 0.75f, -100.0f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(-3.75f, 1.25f, -100.0f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(-3.5f, 0.75f, -103.0f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(-3.75f, 1.25f, -103.0f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(-3.5f, 0.75f, -106.0f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(-3.75f, 1.25f, -106.0f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(3.5f, 0.75f, -100.25f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(3.75f, 1.25f, -100.25f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(3.5f, 0.75f, -103.25f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(3.75f, 1.25f, -103.25f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(3.5f, 0.75f, -106.0f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(3.75f, 1.25f, -106.0f), vec3(0.1f, 1.5f, 0.5f));

		Instantiate(nuestroShader, vec3(0.0f, 0.75f, -109.0f), vec3(0.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(0.0f, 1.25f, -109.25f), vec3(0.5f, 1.5f, 0.1f));

		Instantiate(nuestroShader, vec3(0.0f, 1.0f, -131.75f), vec3(1.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(0.0f, 2.0f, -132.25f), vec3(1.0f, 3.0f, 0.1f));
		Instantiate(nuestroShader, vec3(0.0f, 0.75f, -131.0f), vec3(1.0f, 0.5f, 0.5f));

#pragma endregion

#pragma region COCINA
		Instantiate(nuestroShader, vec3(-11.0f, 1.0f, -129.0f), vec3(1.0f, 1.0f, 8.0f));
		Instantiate(nuestroShader, vec3(-15.5f, 1.0f, -122.0f), vec3(10.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -132.5f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -125.5f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -122.0f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-20.0f, 2.5f, -122.0f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 8.5f, -127.5f), vec3(1.0f, 10.0f, 12.0f));
		Instantiate(nuestroShader, vec3(-16.0f, 8.5f, -122.0f), vec3(9.0f, 10.0f, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(-16.0f, 1.0f, -126.25f), vec3(3.0f, 1.0f, 2.0f));
		Instantiate(nuestroShader, vec3(-14.25f, 1.0f, -132.5f), vec3(2.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-19.0f, 1.0f, -132.5f), vec3(2.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-19.0f, 3.0f, -132.75f), vec3(1.0f, 1.0f, 0.5f));
#pragma endregion

#pragma region ESCALERAS y TECHO
		glBindTexture(GL_TEXTURE_2D, texStoneGray);
		Instantiate(nuestroShader, vec3(-18.0f, 1.0f, -80.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 2.0f, -81.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 3.0f, -82.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 4.0f, -83.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 5.0f, -84.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 6.0f, -85.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 7.0f, -86.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 8.0f, -87.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 9.0f, -88.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 10.0f, -89.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 11.0f, -90.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 12.0f, -91.25f), vec3(5.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-18.0f, 13.0f, -92.25f), vec3(5.0f, 1.0f, 1.0f));


		Instantiate(nuestroShader, vec3(-18.0f, 14.0f, -113.25f), vec3(5.0f, 1.0f, 41.0f));
		Instantiate(nuestroShader, vec3(2.5f, 14.0f, -103.0f), vec3(36.0f, 1.0f, 60.0f));
#pragma endregion

#pragma region CUARTO GRANDE
		glBindTexture(GL_TEXTURE_2D, texStone);
		Instantiate(nuestroShader, vec3(11.5f, 26.5f, -110.0f), vec3(18.0f, 24.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.5f, 26.5f, -110.0f), vec3(18.0f, 24.0f, 1.0f));
		Instantiate(nuestroShader, vec3(0.0f, 29.0f, -110.0f), vec3(5.0f, 18.0f, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texWool);
		Instantiate(nuestroShader, vec3(0.0f, 16.0f, -128.0f), vec3(6.0f, 1.0f, 10.0f));
		glBindTexture(GL_TEXTURE_2D, texWool2);
		Instantiate(nuestroShader, vec3(1.5f, 16.75f, -132.0f), vec3(2.0f, 0.5f, 1.0f));
		Instantiate(nuestroShader, vec3(-1.5f, 16.75f, -132.0f), vec3(2.0f, 0.5f, 1.0f));
		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(2.25f, 15.0f, -123.75f));
		Instantiate(nuestroShader, vec3(-2.25f, 15.0f, -123.75f));

		Instantiate(nuestroShader, vec3(-6.0f, 17.25f, -132.0f), vec3(0.5f, 1.5f, 0.5f));
		Instantiate(nuestroShader, vec3(0.0f, 24.75f, -128.0f), vec3(6.0f, 0.5f, 10.0f));
		Instantiate(nuestroShader, vec3(2.25f, 20.5f, -123.75f), vec3(0.5f, 8.0f, 0.5f)); //Palo de la cama 1
		Instantiate(nuestroShader, vec3(-2.25f, 20.5f, -123.75f), vec3(0.5f, 8.0f, 0.5f)); //Palo de la cama 2
		glBindTexture(GL_TEXTURE_2D, texWhiteWood);
		Instantiate(nuestroShader, vec3(-6.0f, 15.5f, -132.0f), vec3(4.0f, 2.0f, 2.0f));
		Instantiate(nuestroShader, vec3(11.75f, 18.5f, -132.0f), vec3(8.0f, 8.0f, 2.0f));

#pragma endregion

#pragma region CELDAS
		glBindTexture(GL_TEXTURE_2D, texMetal);
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -73.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -74.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -75.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -76.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -77.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -78.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -79.75f), vec3(0.5f, 15.0f, 0.5f));

		Instantiate(nuestroShader, vec3(9.25f, 24.5f, -80.75f), vec3(0.5f, 10.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 24.5f, -81.75f), vec3(0.5f, 10.0f, 0.5f));

		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -82.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -83.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -84.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -85.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -86.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -87.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -88.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -89.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -90.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -91.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -92.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -93.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -94.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -95.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -96.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -97.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -98.75f), vec3(0.5f, 15.0f, 0.5f));

		Instantiate(nuestroShader, vec3(9.25f, 24.5f, -99.75f), vec3(0.5f, 10.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 24.5f, -100.75f), vec3(0.5f, 10.0f, 0.5f));

		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -101.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -102.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -103.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -104.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -105.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -106.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -107.75f), vec3(0.5f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.25f, 22.0f, -108.75f), vec3(0.5f, 15.0f, 0.5f));

		//HORIZONTALES
		Instantiate(nuestroShader, vec3(9.31f, 19.25f, -91.25f), vec3(0.5f, 0.5f, 37.0f));
		Instantiate(nuestroShader, vec3(9.31f, 24.25f, -91.25f), vec3(0.5f, 0.5f, 37.0f));

		//PUERTITAS
		Instantiate(nuestroShader, vec3(7.84f, 16.5f, -80.28f), vec3(3.0f, 5.0f, 0.5f), -25.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(8.25f, 16.5f, -99.8f), vec3(3.0f, 5.0f, 0.5f), -50.0f, vec3(0.0f, 1.0f, 0.0f));

		//PARED MEDIA
		glBindTexture(GL_TEXTURE_2D, texStone);
		Instantiate(nuestroShader, vec3(15.0f, 22.0f, -90.75f), vec3(11.0f, 15.0f, 0.5f));

#pragma endregion


		//#pragma region ESMERALDA
		//		//Propiedades de los materiales
		//		/*nuestroShader.setVec3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
		//		nuestroShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		//		nuestroShader.setVec3("material.specular", 0.633f, 0.727811f, 0.633f);*/
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
		//
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		//		nuestroShader.setMat4("model", model);
		//
		//		
		//#pragma endregion

		//#pragma region JADE
		//		nuestroShader.setVec3("material.ambient", 0.135f, 0.2225f, 0.1575f);
		//		nuestroShader.setVec3("material.diffuse", 0.54f, 0.89f, 0.63f);
		//		nuestroShader.setVec3("material.specular", 0.316228f, 0.316228f, 0.316228f);
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));//Propiedades de los materiales
		//		nuestroShader.setMat4("model", model);
		//		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		//#pragma endregion
		//
		//#pragma region OBSIDIANA
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		//		nuestroShader.setVec3("material.ambient", 0.05375f, 0.05f, 0.06625f);
		//		nuestroShader.setVec3("material.diffuse", 0.18275f, 0.17f, 0.22525f);
		//		nuestroShader.setVec3("material.specular", 0.332741f, 0.328634f, 0.346435f);
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.3f);
		//		nuestroShader.setMat4("model", model);
		//		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		//#pragma endregion
		//
		//#pragma region PERLA
		//		nuestroShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
		//		nuestroShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
		//		nuestroShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.088f);
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		//		nuestroShader.setMat4("model", model);
		//		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		//#pragma endregion
		//
		//#pragma region RUBY
		//		nuestroShader.setVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
		//		nuestroShader.setVec3("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		//		nuestroShader.setVec3("material.specular", 0.727811f, 0.626959f, 0.626959f);
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		//		nuestroShader.setMat4("model", model);
		//		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		//#pragma endregion
		//
		//#pragma region TURQUESA
		//		nuestroShader.setVec3("material.ambient", 0.1f, 0.18725f, 0.1745f);
		//		nuestroShader.setVec3("material.diffuse", 0.396f, 0.74151f, 0.69102f);
		//		nuestroShader.setVec3("material.specular", 0.297254f, 0.30829f, 0.306678f);
		//		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
		//		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		//		nuestroShader.setMat4("model", model);
		//		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		//#pragma endregion

		//Configurar punto de luz
		//luzShader.use();
		//luzShader.setMat4("projection", projection);
		//luzShader.setMat4("view", view);
		//model = mat4(1.0f);
		//model = translate(model, posLuz);
		//model = scale(model, vec3(0.2f));
		////model = translate(model, vec3(posLuz.x, posLuz.y, -glfwGetTime()));
		//model = rotate(model, 45.0f, vec3(0.0f, 1.0f, 0.0f));
		//luzShader.setMat4("model", model);

		//renderizar luz
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//Dibujamos el skybox
		// Función de profundidad encargada del cálculo entre el usuario y la profundidad del skybox para generación de infinidad
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = mat4(mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(sVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &sVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &sVBO);
	glfwTerminate();
	return 0;
}

//Instancía un cubo, sin rotación.
void Instantiate(Shader& _shader, vec3 _position)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instanciar con una escala
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con la rotación indicada 
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con escala y rotación indicadas 
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//nuestra primera configuracion de la camara
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int LoadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {
		std::cout << "Error en la textura: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

unsigned int LoadCubeMap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cout << "fallo en cargar las texturas de nuestro skybox, la textura que fallo es: " << faces[i] << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
