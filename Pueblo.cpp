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

using namespace glm;
using namespace std;

void Instantiate(Shader& _shader, vec3 _position);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale);
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis);

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana
unsigned int LoadTexture(const char* path); //Cargar textura
unsigned int LoadCubeMap(vector<string> faces);

//medidas de la pantalla
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

//Cámara
Camara camera(vec3(0.0f, 5.0f, -20.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//crear nuestras texturas
unsigned int texMetal, texOldMetal, texWoodenWindow, texDirt, texStone, texStone2, texBox, texWantedSign, texDoor;
unsigned int texWood, texWood2, texWood3, texWood4, texWood5, texWhiteWood;

int main() {
	float tempCoord = 0.0f;
	//inicializar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MUY BUENAS TARDES", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en gcrear GLFW y la ventana" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//DECIRLE a glfw que va recibir señales del mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); //Esto es para evitar la sobreposición de las figuras

	Shader nuestroShader("Texturas.vs", "Texturas.fs");
	Shader skyboxShader("Skybox.vs", "Skybox.fs");

	float vertices[]{
		//posiciones			//colores			//texturas
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

#pragma region SHADER 1
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texturas
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion

	//Skybox Shader
	unsigned int sVAO, sVBO;
	glGenVertexArrays(1, &sVAO);

	glGenBuffers(1, &sVBO);
	glBindVertexArray(sVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sVAO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSkybox), &verticesSkybox, GL_STATIC_DRAW);
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


#pragma region CARGA DE TEXTURAS
	texWood = LoadTexture("Wood.jpg"); //Palos
	texWood2 = LoadTexture("Wood2.jpg"); //Estructuras
	texWood3 = LoadTexture("Wood3.jpg"); //Estructuras 
	texWood4 = LoadTexture("Wood4.jpg"); //Estructuras 
	texWood5 = LoadTexture("Wood5.jpg"); //Casona 
	texWhiteWood = LoadTexture("WhiteWood.jpeg"); //Palos

	texWantedSign = LoadTexture("WantedSign.jpg");
	texDirt = LoadTexture("Dirt2.jpg");
	texMetal = LoadTexture("Metal.png");
	texOldMetal = LoadTexture("OldMetal.jpg");
	texStone = LoadTexture("Stone.jpg");
	texStone2 = LoadTexture("StoneGray.jpg");
	texBox = LoadTexture("Box.png");
	texWoodenWindow = LoadTexture("WoodenWindow.jpg");
	texDoor = LoadTexture("Door1.jpg");
#pragma endregion

	nuestroShader.setInt("textura1", 0);
	nuestroShader.use();

	//loop para que se pueda visualizar nuestra pantalla
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Renderizado
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //LIMPIEZA DEL BUFFER Z

		nuestroShader.use();

		mat4 projection = perspective(radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
		mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);

#pragma region SUELO
		glBindTexture(GL_TEXTURE_2D, texDirt); //Poner textura por defecto
		Instantiate(nuestroShader, vec3(0.0f, 0.0f, 5.0f), vec3(50.0f, 1.0f, 60.0f)); //Tierra
#pragma endregion

#pragma region BOXES
		glBindTexture(GL_TEXTURE_2D, texBox);
		Instantiate(nuestroShader, vec3(12.0f, 2.0f, -6.25f)); 
		Instantiate(nuestroShader, vec3(12.0f, 1.0f, -7.0f)); 
		Instantiate(nuestroShader, vec3(8.25f, 1.25f, 14.75f)); 
		Instantiate(nuestroShader, vec3(7.0f, 1.25f, 14.75f)); 
		Instantiate(nuestroShader, vec3(7.75f, 2.25f, 15.0f)); 
		Instantiate(nuestroShader, vec3(8.75f, 1.25f, -18.25f)); 
		Instantiate(nuestroShader, vec3(-9.25f, 1.0f, -18.25f)); 
		Instantiate(nuestroShader, vec3(-12.25f, 1.25f, -5.75f)); 
		Instantiate(nuestroShader, vec3(-16.0f, 1.0f, -7.5f)); 
		Instantiate(nuestroShader, vec3(12.0f, 1.0f, -5.75f)); 
		Instantiate(nuestroShader, vec3(10.75f, 1.0f, -6.0f)); 
		Instantiate(nuestroShader, vec3(6.0f, 1.0f, -12.5f)); 
		Instantiate(nuestroShader, vec3(4.0f, 1.0f, -23.0f)); 
		Instantiate(nuestroShader, vec3(-7.75f, 1.0f, -20.75f)); 
		Instantiate(nuestroShader, vec3(-3.0f, 1.0f, -8.0f)); 
		Instantiate(nuestroShader, vec3(6.0f, 1.0f, -4.0f)); 
		Instantiate(nuestroShader, vec3(-5.5f, 1.0f, 7.75f)); 

#pragma endregion

#pragma region Postes
		glBindTexture(GL_TEXTURE_2D, texWood4);
		Instantiate(nuestroShader, vec3(-5.5f, 2.5f, -23.5f), vec3(3.0f, 0.25f, 0.25f));
		Instantiate(nuestroShader, vec3(-4.5f, 1.5f, -23.5f), vec3(0.25f, 1.75f, 0.25f));
		Instantiate(nuestroShader, vec3(-6.5f, 1.5f, -23.5f), vec3(0.25f, 1.75f, 0.25f));

		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -8.75f), vec3(0.25f, 0.25f, 3.0f));
		Instantiate(nuestroShader, vec3(-9.0f, 1.5f, -9.75f), vec3(0.25f, 1.75f, 0.25f));
		Instantiate(nuestroShader, vec3(-9.0f, 1.5f, -7.75f), vec3(0.25f, 1.75f, 0.25f));

		Instantiate(nuestroShader, vec3(-5.75f, 2.5f, -2.5f), vec3(0.25f, 0.25f, 3.0f));
		Instantiate(nuestroShader, vec3(-5.75f, 1.5f, -3.5f), vec3(0.25f, 1.75f, 0.25f));
		Instantiate(nuestroShader, vec3(-5.75f, 1.5f, -1.5f), vec3(0.25f, 1.75f, 0.25f));

		Instantiate(nuestroShader, vec3(5.75f, 2.5f, -1.75f), vec3(0.25f, 0.25f, 3.0f));
		Instantiate(nuestroShader, vec3(5.75f, 1.5f, -2.75f), vec3(0.25f, 1.75f, 0.25f));
		Instantiate(nuestroShader, vec3(5.75f, 1.5f, -0.75f), vec3(0.25f, 1.75f, 0.25f));

#pragma endregion

#pragma region VALLA FRONTAL
		glBindTexture(GL_TEXTURE_2D, texWood);
		//Parte izquierda
		tempCoord = -21.0f;
		for (int i = 0; i < 9; i++) 
		{
			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, -22.0f), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}
		//Parte derecha
		tempCoord = 5.0f;
		for (int i = 0; i < 9; i++)
		{
			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, -22.0f), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}

		//Entrada principal
		Instantiate(nuestroShader, vec3(-3.0f, 3.5f, -22.0f), vec3(0.4f, 6.0f, 0.4f));
		Instantiate(nuestroShader, vec3(3.0f, 3.5f, -22.0f), vec3(0.4f, 6.0f, 0.4f));
		Instantiate(nuestroShader, vec3(0.0f, 6.75f, -22.0f), vec3(7.5f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(-1.0f, 6.25f, -22.0f), vec3(0.1f, 0.5f, 0.1f)); //Palito 1
		Instantiate(nuestroShader, vec3(1.0f, 6.25f, -22.0f), vec3(0.1f, 0.5f, 0.1f)); //Palito 2
		
		//Tablas horizontales
		Instantiate(nuestroShader, vec3(12.25f, 2.25f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(12.25f, 1.5f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(12.25f, 0.75f, -22.15f), vec3(18.0f, 0.2f, 0.1f));

		Instantiate(nuestroShader, vec3(-12.25f, 2.25f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(-12.25f, 1.5f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(-12.25f, 0.75f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
		

		glBindTexture(GL_TEXTURE_2D, texOldMetal);
		Instantiate(nuestroShader, vec3(0.0f, 5.5f, -22.0f), vec3(4.0f, 1.0f, 0.1f)); //Letrero


#pragma endregion

#pragma region VALLA DERECHA
		glBindTexture(GL_TEXTURE_2D, texWood);
		tempCoord = -20.0f;
		for (int i = 0; i < 27; i++)
		{
			Instantiate(nuestroShader, vec3(21.0f, 1.5f, tempCoord), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}

		Instantiate(nuestroShader, vec3(21.15f, 2.25f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
		Instantiate(nuestroShader, vec3(21.15f, 1.5f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
		Instantiate(nuestroShader, vec3(21.15f, 0.75f, 5.0f), vec3(0.1f, 0.2f, 54.0f));

#pragma endregion

#pragma region VALLA IZQUIERDA
		tempCoord = -20.0f;
		for (int i = 0; i < 27; i++)
		{
			Instantiate(nuestroShader, vec3(-21.0f, 1.5f, tempCoord), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}

		Instantiate(nuestroShader, vec3(-21.15f, 2.25f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
		Instantiate(nuestroShader, vec3(-21.15f, 1.5f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
		Instantiate(nuestroShader, vec3(-21.15f, 0.75f, 5.0f), vec3(0.1f, 0.2f, 54.0f));

#pragma endregion

#pragma region VALLA TRASERA
		tempCoord = -19.0f;
		for (int i = 0; i < 20; i++)
		{
			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, 32.0f), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}

		Instantiate(nuestroShader, vec3(0.0f, 2.25f, 32.15f), vec3(42.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(0.0f, 1.5f, 32.15f), vec3(42.0f, 0.2f, 0.1f));
		Instantiate(nuestroShader, vec3(0.0f, 0.75f, 32.15f), vec3(42.0f, 0.2f, 0.1f));

#pragma endregion

#pragma region MOTEL
		glBindTexture(GL_TEXTURE_2D, texWood2);
		Instantiate(nuestroShader, vec3(-15.0f, 8.5f, -14.0f), vec3(10.0f, 16.0f, 12.0f)); 
		Instantiate(nuestroShader, vec3(-9.0f, 4.5f, -13.75f), vec3(3.0f, 0.2f, 8.0f), -20.0f, vec3(0.0f, 0.0f, 1.0f)); //Techito

		glBindTexture(GL_TEXTURE_2D, texDoor);
		Instantiate(nuestroShader, vec3(-10.0f, 2.0f, -13.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-10.0f, 2.0f, -15.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta

		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -14.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -10.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -18.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));

		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -18.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -14.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -10.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));

		glBindTexture(GL_TEXTURE_2D, texOldMetal);
		Instantiate(nuestroShader, vec3(-10.0f, 17.25f, -14.0f), vec3(0.2f, 3.0f, 8.0f));

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -11.0f), vec3(0.25f, 4.0f, 0.25f)); 
		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -17.0f), vec3(0.25f, 4.0f, 0.25f));

#pragma endregion

#pragma region SHERIFF COMISARIA
		glBindTexture(GL_TEXTURE_2D, texWood4); 
		Instantiate(nuestroShader, vec3(13.5f, 0.5f, -14.0f), vec3(13.0f, 0.5f, 12.0f)); //Piso
		//techito y postes
		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -18.5f), vec3(0.2f, 4.25f, 0.2f));
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -9.5f), vec3(0.2f, 4.25f, 0.2f));
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -12.0f), vec3(0.2f, 4.25f, 0.2f));
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -16.0f), vec3(0.2f, 4.25f, 0.2f));
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -17.25f), vec3(0.1f, 0.4f, 3.0f));
		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -10.75f), vec3(0.1f, 0.4f, 3.0f));
		Instantiate(nuestroShader, vec3(8.5f, 5.5f, -14.0f), vec3(3.0f, 0.2f, 9.9f), 30.0f, vec3(0.0f, 0.0f, 1.0f)); //Techito
		Instantiate(nuestroShader, vec3(14.0f, 7.5f, -14.0f), vec3(9.5f, 0.5f, 10.5f)); //Techote

		glBindTexture(GL_TEXTURE_2D, texWood3);
		Instantiate(nuestroShader, vec3(14.0f, 4.0f, -14.0f), vec3(9.0f, 7.0f, 10.0f)); //Cubote casa

		glBindTexture(GL_TEXTURE_2D, texDoor);
		Instantiate(nuestroShader, vec3(9.5f, 2.25f, -14.0f), vec3(0.1f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta

		glBindTexture(GL_TEXTURE_2D, texMetal);
		Instantiate(nuestroShader, vec3(9.75f, 7.5f, -14.0f), vec3(0.25f, 4.0f, 4.0f), 45.0f, vec3(1.0f, 0.0f, 0.0f));

		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
		Instantiate(nuestroShader, vec3(9.5f, 3.25f, -17.0f), vec3(0.1f, 2.0f, 1.5f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Ventana
		Instantiate(nuestroShader, vec3(9.5f, 3.25f, -11.0f), vec3(0.1f, 2.0f, 1.5f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Ventana

#pragma endregion

#pragma region CARCEL
		glBindTexture(GL_TEXTURE_2D, texWood4);
		Instantiate(nuestroShader, vec3(13.5f, 0.5f, 1.0f), vec3(13.0f, 0.5f, 10.0f)); //Piso

		glBindTexture(GL_TEXTURE_2D, texStone);
		//Paredes
		Instantiate(nuestroShader, vec3(18.5f, 4.75f, 1.0f), vec3(0.5f, 8.0f, 8.0f)); 
		Instantiate(nuestroShader, vec3(9.0f, 4.75f, 2.5f), vec3(0.5f, 8.0f, 5.0f)); 
		Instantiate(nuestroShader, vec3(13.75f, 4.75f, 4.75f), vec3(9.0f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.0f, 4.75f, -2.5f), vec3(0.5f, 8.0f, 1.0f));
		Instantiate(nuestroShader, vec3(13.75f, 4.75f, -2.75f), vec3(9.0f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.0f, 1.25f, -1.0f), vec3(0.5f, 1.0f, 2.0f));
		Instantiate(nuestroShader, vec3(9.0f, 6.75f, -1.0f), vec3(0.5f, 4.0f, 2.0f));

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(13.75f, 9.0f, 1.0f), vec3(10.0f, 0.5f, 8.0f)); //Techo
		//Cosito lateral con techo
		Instantiate(nuestroShader, vec3(13.75f, 7.0f, 7.25f), vec3(9.0f, 0.25f, 5.0f), 20.0f, vec3(1.0f, 0.0f, 0.0f)); //Techito
		Instantiate(nuestroShader, vec3(9.75f, 3.5f, 8.75f), vec3(0.25f, 6.0f, 0.25f));
		Instantiate(nuestroShader, vec3(18.0f, 3.5f, 8.75f), vec3(0.25f, 6.0f, 0.25f));
		Instantiate(nuestroShader, vec3(9.75f, 2.5f, 6.75f), vec3(0.2f, 0.25f, 4.0f));
		Instantiate(nuestroShader, vec3(18.0f, 2.5f, 6.75f), vec3(0.2f, 0.25f, 4.0f));

		glBindTexture(GL_TEXTURE_2D, texOldMetal);
		Instantiate(nuestroShader, vec3(9.0f, 10.25f, 1.0f), vec3(0.5f, 2.0f, 5.0f));

		glBindTexture(GL_TEXTURE_2D, texMetal);
		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -1.0f), vec3(0.2f, 3.0f, 0.2f));
		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -1.5f), vec3(0.2f, 3.0f, 0.2f));
		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -0.5f), vec3(0.2f, 3.0f, 0.2f));

		glBindTexture(GL_TEXTURE_2D, texDoor);
		Instantiate(nuestroShader, vec3(8.75f, 2.25f, 2.5f), vec3(0.1f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));


#pragma endregion

#pragma region CANTINA
		glBindTexture(GL_TEXTURE_2D, texWood4);
		Instantiate(nuestroShader, vec3(-13.5f, 0.5f, 1.5f), vec3(13.0f, 0.5f, 15.0f)); //Piso

		glBindTexture(GL_TEXTURE_2D, texWhiteWood);
		//Paredes
		Instantiate(nuestroShader, vec3(-18.0f, 4.75f, 1.5f), vec3(0.5f, 8.0f, 13.5f));
		Instantiate(nuestroShader, vec3(-8.5f, 4.75f, 5.75f), vec3(0.5f, 8.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-13.25f, 4.75f, 8.0f), vec3(9.0f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-13.25f, 4.75f, -5.0f), vec3(9.0f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-8.5f, 4.75f, -2.75f), vec3(0.5f, 8.0f, 5.0f));
		Instantiate(nuestroShader, vec3(-8.5f, 6.75f, 1.5f), vec3(0.5f, 4.0f, 3.5f));
		Instantiate(nuestroShader, vec3(-13.25f, 8.0f, 1.5f), vec3(9.0f, 0.5f, 12.5f)); //Techo
		
		glBindTexture(GL_TEXTURE_2D, texWood2);
		Instantiate(nuestroShader, vec3(-9.25f, 2.5f, 2.5f), vec3(0.1f, 2.0f, 2.0f), 35.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-9.25f, 2.5f, 0.5f), vec3(0.1f, 2.0f, 2.0f), -35.0f, vec3(0.0f, 1.0f, 0.0f));

		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, 5.75f), vec3(0.1f, 2.5f, 2.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, -2.75f), vec3(0.1f, 2.5f, 2.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(-5.75f, 8.25f, 7.0f), vec3(5.0f, 0.5f, 0.5f));
		Instantiate(nuestroShader, vec3(-7.0f, 7.5f, 7.0f), vec3(0.25f, 1.0f, 0.25f));
		Instantiate(nuestroShader, vec3(-4.5f, 7.5f, 7.0f), vec3(0.25f, 1.0f, 0.25f));

		glBindTexture(GL_TEXTURE_2D, texOldMetal);
		Instantiate(nuestroShader, vec3(-5.75f, 6.5f, 7.0f), vec3(3.5f, 2.0f, 0.1f));

#pragma endregion

#pragma region LA CASONA
		glBindTexture(GL_TEXTURE_2D, texWood4);
		Instantiate(nuestroShader, vec3(0.0f, 0.5f, 20.0f), vec3(24.0f, 0.5f, 18.0f)); //Piso

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(0.0f, 16.0f, 22.0f), vec3(22.0f, 0.5f, 14.0f)); //TEcho
		Instantiate(nuestroShader, vec3(0.0f, 17.75f, 22.0f), vec3(16.0f, 3.0f, 10.0f)); //Techo

		//Tablones
		Instantiate(nuestroShader, vec3(0.0f, 9.0f, 13.75f), vec3(20.0f, 0.5f, 4.0f)); //Piso 2
		Instantiate(nuestroShader, vec3(3.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-3.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-9.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.5f, 2.0f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
		Instantiate(nuestroShader, vec3(9.5f, 3.5f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
		Instantiate(nuestroShader, vec3(-9.5f, 2.0f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
		Instantiate(nuestroShader, vec3(-9.5f, 3.5f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
		Instantiate(nuestroShader, vec3(-2.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(2.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(6.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(9.5f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(-6.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(-9.5f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(0.0f, 9.75f, 12.5f), vec3(20.0f, 0.25f, 0.2f));
		Instantiate(nuestroShader, vec3(0.0f, 10.5f, 12.5f), vec3(20.0f, 0.25f, 0.2f));
		Instantiate(nuestroShader, vec3(9.5f, 10.0f, 14.0f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(-9.5f, 10.0f, 14.0f), vec3(0.25f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(9.5f, 10.5f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
		Instantiate(nuestroShader, vec3(9.5f, 9.75f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
		Instantiate(nuestroShader, vec3(-9.5f, 10.5f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
		Instantiate(nuestroShader, vec3(-9.5f, 9.75f, 14.0f), vec3(0.2f, 0.25f, 4.0f));

		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
		//Ventanas
		Instantiate(nuestroShader, vec3(6.0f, 3.5f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
		Instantiate(nuestroShader, vec3(-6.0f, 3.5f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
		Instantiate(nuestroShader, vec3(6.0f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
		Instantiate(nuestroShader, vec3(0.25f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
		Instantiate(nuestroShader, vec3(-6.0f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));

		glBindTexture(GL_TEXTURE_2D, texMetal);
		Instantiate(nuestroShader, vec3(0.0f, 20.75f, 17.5f), vec3(6.0f, 3.0f, 0.5f)); //Letrero

		glBindTexture(GL_TEXTURE_2D, texWood2);
		Instantiate(nuestroShader, vec3(1.25f, 3.25f, 16.5f), vec3(2.0f, 3.0f, 0.1f), 35.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-1.25f, 3.25f, 16.5f), vec3(2.0f, 3.0f, 0.1f), -35.0f, vec3(0.0f, 1.0f, 0.0f));

		glBindTexture(GL_TEXTURE_2D, texWood5);
		//Paredes
		Instantiate(nuestroShader, vec3(6.0f, 8.25f, 16.0f), vec3(8.0f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(0.0f, 10.75f, 16.0f), vec3(4.0f, 10.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-6.0f, 8.25f, 16.0f), vec3(8.0f, 15.0f, 0.5f));
		Instantiate(nuestroShader, vec3(9.75f, 8.25f, 22.25f), vec3(0.5f, 15.0f, 12.0f));
		Instantiate(nuestroShader, vec3(-9.75f, 8.25f, 22.25f), vec3(0.5f, 15.0f, 12.0f));
		Instantiate(nuestroShader, vec3(-0.0f, 8.25f, 28.0f), vec3(19.0f, 15.0f, 0.5f));

#pragma endregion
		
#pragma region POZO
		glBindTexture(GL_TEXTURE_2D, texStone2);
		Instantiate(nuestroShader, vec3(1.0f, 1.25f, -6.75f), vec3(0.25f, 1.5f, 2.25f));
		Instantiate(nuestroShader, vec3(-1.0f, 1.25f, -6.75f), vec3(0.25f, 1.5f, 2.25f));
		Instantiate(nuestroShader, vec3(0.0f, 1.25f, -7.75f), vec3(1.75f, 1.5f, 0.25f));
		Instantiate(nuestroShader, vec3(0.0f, 1.25f, -5.75f), vec3(1.75f, 1.5f, 0.25f));

		Instantiate(nuestroShader, vec3(-0.75f, 4.25f, -6.75f), vec3(2.35f, 0.25f, 3.0f), 45.0f, vec3(0.0f, 0.0f, 1.0f));
		Instantiate(nuestroShader, vec3(0.75f, 4.25f, -6.75f), vec3(2.0f, 0.25f, 3.0f), -45.0f, vec3(0.0f, 0.0f, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texWood);
		Instantiate(nuestroShader, vec3(1.0f, 3.0f, -5.75f), vec3(0.25f, 2.0f, 0.25f));
		Instantiate(nuestroShader, vec3(-1.0f, 3.0f, -5.75f), vec3(0.25f, 2.0f, 0.25f));
		Instantiate(nuestroShader, vec3(-1.0f, 3.0f, -7.75f), vec3(0.25f, 2.0f, 0.25f));
		Instantiate(nuestroShader, vec3(1.0f, 3.0f, -7.75f), vec3(0.25f, 2.0f, 0.25f));

		Instantiate(nuestroShader, vec3(0.0f, 3.75f, -6.75f), vec3(2.5f, 0.25f, 0.25f));
		Instantiate(nuestroShader, vec3(0.0f, 2.75f, -6.75f), vec3(0.1f, 2.0f, 0.1f));
		Instantiate(nuestroShader, vec3(0.0f, 2.75f, -6.75f), vec3(0.1f, 2.0f, 0.1f));


		glBindTexture(GL_TEXTURE_2D, texStone);
		Instantiate(nuestroShader, vec3(0.0f, 0.5f, -6.75f), vec3(4.0f, 0.25f, 4.0f));

#pragma endregion

#pragma region POSTER WANTED
		glBindTexture(GL_TEXTURE_2D, texWantedSign);
		Instantiate(nuestroShader, vec3(-10.0f, 3.0f, -18.25f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
		Instantiate(nuestroShader, vec3(3.0f, 3.0f, -22.25f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
		Instantiate(nuestroShader, vec3(10.75f, 3.0f, -19.0f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
		Instantiate(nuestroShader, vec3(8.75f, 3.0f, -2.5f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
		Instantiate(nuestroShader, vec3(-3.5f, 3.0f, 12.25f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, -1.0f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
		
#pragma endregion

		//Dibujamos el skybox
		// Función de profundidad encargada del cálculo entre el usuario y la profundidad del skybox para generación de infinidad
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		skyboxShader.use();
		view = mat4(mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(sVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
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
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
}

//Instanciar con una escala
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con la rotación indicada 
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con escala y rotación indicadas 
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);
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

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
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
		}
		else
		{
			cout << "fallo en cargar las texturas de nuestro skybox, la textura que fallo es: " << faces[i] << endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}