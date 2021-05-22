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
using namespace glm;

void Instantiate(Shader& _shader, vec3 _position);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale);
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana

//medidas de la pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//Cámara
Camara camera(vec3(0.0f, 5.0f, 10.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TAMAL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en gcrear GLFW y la ventana date un balazo" << std::endl;
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
	Shader segundoShader("Vertex2.vs", "Vertex2.fs");

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

	float triangulos[] = {
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 
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
	unsigned int indicestriangulo[]
	{
		0, 1, 2
	};

	vec3 posicionesCubo[] = {
		vec3(0.0f,  0.0f,  0.0f),
		vec3(2.0f,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f, -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f, -2.0f, -2.5f),
		vec3(1.5f,  2.0f, -2.5f),
		vec3(1.5f,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};


#pragma region SHADER LADRILLOS
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

#pragma region SHADER METAL
	unsigned int VBO2, VAO2, EBO2; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2); 
	//unir o linkear
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	//texturas
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(3);
#pragma endregion

	//crear y cargar nuestras texturas
	unsigned int textura1, textura2;
	//primera textura
	glGenTextures(1, &textura1);
	glBindTexture(GL_TEXTURE_2D, textura1);
	//configurar el comportamiento de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//configuramos el filtrado de la textura en caso que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//cargar la imagen, crear la textura y generar los mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	//cargar nuestra textura
	unsigned char* data = stbi_load("Bricks.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "fallaste en cargar la primera textura" << std::endl;
	}
	stbi_image_free(data);

	//Textura2
	glGenTextures(1, &textura2);
	glBindTexture(GL_TEXTURE_2D, textura2);
	//configurar el comportamiento de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//configuramos el filtrado de la textura en caso que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//cargar nuestra textura
	data = stbi_load("Metal.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "fallaste en cargar la segunda textura" << std::endl;
	}
	stbi_image_free(data);

	nuestroShader.use();
	nuestroShader.setInt("textura1", 0);
	nuestroShader.setInt("textura2", 1);

	segundoShader.use();
	segundoShader.setInt("textura1", 0);
	segundoShader.setInt("textura2", 1);

	//loop para que se pueda visualizar nuestra pantalla
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Renderizado
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //LIMPIEZA DEL BUFFER Z

		//activado  y union de las texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);

		//transformaciones
		/*mat4 transform = mat4(1.0f); //inicializamos con una matriz identidad
		transform = translate(transform, vec3(0.3f, -0.5f, 0.0f));
		transform = rotate(transform, (float)glfwGetTime(), vec3(0.0f, 1.0f, 1.0f));*/

		//nuestroShader.use();
		//MVP
		/*mat4 view = mat4(1.0f);
		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f ); //SE maneja en radianes para mayor exactitud
		view = translate(view, vec3(0.0f, 0.0f, -3.0f));*/

		mat4 projection = perspective(radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 500.0f);
		mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);
		segundoShader.setMat4("projection", projection);
		segundoShader.setMat4("view", view);

		/*unsigned int transformLoc = glGetUniformLocation(nuestroShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));*/

		glBindVertexArray(VAO);
		//glBindVertexArray(VAO2);

		/*for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = mat4(1.0f);
			model = translate(model, posicionesCubo[i]);
			float angulos = (float)glfwGetTime() * (i + 1);
			model = rotate(model, radians(angulos), vec3(1.0f, 0.3f, 0.5f));
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
		}*/
/*
#pragma region CASA
		Instantiate(nuestroShader, vec3(0.0f, 0.0f, 0.0f), vec3(10.0f, 0.5f, 10.0f)); //Cimientos
		Instantiate(nuestroShader, vec3(0.0f, 2.5f, 0.0f), vec3(5.0f, 5.0f, 5.0f)); //Cubo Casa
		Instantiate(nuestroShader, vec3(0.0f, 1.0f, 2.1f), vec3(1.0f, 2.0f, 1.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-2.1f, 2.0f, 0.0f), vec3(1.0f, 2.0f, 3.0f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Ventana izquierda
		Instantiate(nuestroShader, vec3(2.1f, 2.0f, 0.0f), vec3(1.0f, 2.0f, 3.0f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Ventana derecha
		Instantiate(nuestroShader, vec3(0.0f, 4.0f, 2.1f), vec3(3.0f, 1.0f, 1.0f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Ventana frontal
		Instantiate(nuestroShader, vec3(0.0f, 5.0f, 0.0f), vec3(3.53f, 3.53f, 4.98f), 45.0f, vec3(0.0f, 0.0f, 1.0f)); //Techo
		Instantiate(nuestroShader, vec3(-1.59f, 6.25f, 0.0f), vec3(5.0f, 0.5f, 4.98f), 45.0f, vec3(0.0f, 0.0f, 1.0f)); //Techo saliente izq
		Instantiate(nuestroShader, vec3(1.59f, 6.25f, 0.0f), vec3(5.0f, 0.5f, 4.96f), -45.0f, vec3(0.0f, 0.0f, 1.0f)); //Techo saliente izq
#pragma endregion
*/

#pragma region MUROS EXTERIOR
		Instantiate(segundoShader, vec3(0.0f, 0.0f, 0.0f), vec3(300.0f, 1.0f, 300.0f));
		Instantiate(nuestroShader, vec3(17.5f, 20.5f, -60.5f), vec3(25.0f, 40.0f, 25.0f));
		Instantiate(nuestroShader, vec3(-17.5f, 20.5f, -60.5f), vec3(25.0f, 40.0f, 25.0f));
		Instantiate(nuestroShader, vec3(0.0f, 32.75f, -60.7f), vec3(10.0f, 15.0f, 24.0f));
		Instantiate(nuestroShader, vec3(0.0f, 39.0f, -103.0f), vec3(42.0f, 2.0f, 60.0f)); //Techo grandote

		//Instantiate(nuestroShader, vec3(1.5f, 25.2f, -60.6f), vec3(10.0f, 5.0f, 24.0f), -25.0f, vec3(0.0f, 0.0f, 1.0f));
		//Instantiate(nuestroShader, vec3(-1.5f, 25.2f, -60.5f), vec3(10.0f, 5.0f, 24.0f), 25.0f, vec3(0.0f, 0.0f, 1.0f));
#pragma endregion

#pragma region PASARELA EXTERIOR
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

#pragma region MESA
		Instantiate(nuestroShader, vec3(0.0f, 1.5f, -103.0f), vec3(5.0f, 0.2f, 10.0f));
		Instantiate(nuestroShader, vec3(1.75f, 1.0f, -98.75f));
		Instantiate(nuestroShader, vec3(-1.75f, 1.0f, -98.75f));
		Instantiate(nuestroShader, vec3(-1.75f, 1.0f, -107.25f));
		Instantiate(nuestroShader, vec3(1.75f, 1.0f, -107.25f));
#pragma endregion

#pragma region SILLAS
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

#pragma region Paredes
		Instantiate(nuestroShader, vec3(23.0f, 20.05f, -103.0f), vec3(5.0f, 40.0f, 60.0f));
		Instantiate(nuestroShader, vec3(-23.0f, 20.05f, -103.0f), vec3(5.0f, 40.0f, 60.0f));
		Instantiate(nuestroShader, vec3(17.5f, 20.05f, -145.5f), vec3(25.0f, 40.0f, 25.0f));
		Instantiate(nuestroShader, vec3(-17.5f, 20.05f, -145.5f), vec3(25.0f, 40.0f, 25.0f));
		Instantiate(nuestroShader, vec3(0.0f, 20.05f, -135.5f), vec3(10.0f, 40.0f, 5.0f));
#pragma endregion

#pragma region COCINA
		Instantiate(nuestroShader, vec3(-11.0f, 1.0f, -129.0f), vec3(1.0f, 1.0f, 8.0f));
		Instantiate(nuestroShader, vec3(-15.5f, 1.0f, -122.0f), vec3(10.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -132.5f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -125.5f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 2.5f, -122.0f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-20.0f, 2.5f, -122.0f), vec3(1.0f, 2.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.0f, 8.5f, -127.5f), vec3(1.0f, 10.0f, 12.0f));
		Instantiate(nuestroShader, vec3(-16.0f, 8.5f, -122.25f), vec3(9.0f, 10.0f, 1.0f));

		Instantiate(nuestroShader, vec3(-16.0f, 1.0f, -126.25f), vec3(3.0f, 1.0f, 2.0f));
		Instantiate(nuestroShader, vec3(-14.25f, 1.0f, -132.5f), vec3(2.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-19.0f, 1.0f, -132.5f), vec3(2.0f, 1.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-19.0f, 3.0f, -132.75f), vec3(1.0f, 1.0f, 0.5f));
#pragma endregion

#pragma region ESCALERAS y TECHO
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
		Instantiate(nuestroShader, vec3(11.5f, 26.5f, -110.0f), vec3(18.0f, 24.0f, 1.0f));
		Instantiate(nuestroShader, vec3(-11.5f, 26.5f, -110.0f), vec3(18.0f, 24.0f, 1.0f));
		Instantiate(nuestroShader, vec3(0.0f, 29.0f, -110.0f), vec3(5.0f, 18.0f, 1.0f));
		
		Instantiate(nuestroShader, vec3(0.0f, 16.0f, -128.0f), vec3(6.0f, 1.0f, 10.0f));
		Instantiate(nuestroShader, vec3(2.25f, 15.0f, -123.75f));
		Instantiate(nuestroShader, vec3(-2.25f, 15.0f, -123.75f));
		Instantiate(nuestroShader, vec3(1.5f, 16.75f, -132.0f), vec3(2.0f, 0.5f, 1.0f));
		Instantiate(nuestroShader, vec3(-1.5f, 16.75f, -132.0f), vec3(2.0f, 0.5f, 1.0f));
		Instantiate(nuestroShader, vec3(-6.0f, 15.5f, -132.0f), vec3(4.0f, 2.0f, 2.0f));
		Instantiate(nuestroShader, vec3(-6.0f, 17.25f, -132.0f), vec3(0.5f, 1.5f, 0.5f));
		Instantiate(nuestroShader, vec3(11.75f, 18.5f, -132.0f), vec3(8.0f, 8.0f, 2.0f));
		Instantiate(nuestroShader, vec3(0.0f, 24.75f, -128.0f), vec3(6.0f, 0.5f, 10.0f));
		Instantiate(nuestroShader, vec3(2.25f, 20.5f, -123.75f), vec3(0.5f, 8.0f, 0.5f));
		Instantiate(nuestroShader, vec3(-2.25f, 20.5f, -123.75f), vec3(0.5f, 8.0f, 0.5f));
#pragma endregion

#pragma region CELDAS
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

		//PARED MEDIA
		Instantiate(nuestroShader, vec3(15.0f, 22.0f, -90.75f), vec3(11.0f, 15.0f, 0.5f));

		//PUERTITAS
		Instantiate(nuestroShader, vec3(7.84f, 16.5f, -80.28f), vec3(3.0f, 5.0f, 0.5f), -25.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(8.25f, 16.5f, -99.8f), vec3(3.0f, 5.0f, 0.5f), -50.0f, vec3(0.0f, 1.0f, 0.0f));

#pragma endregion

		
		mat4 transform2 = mat4(1.0f);
		transform2 = translate(transform2, vec3(-0.3f, 0.5f, 0.0f));
		transform2 = rotate(transform2, ((float)glfwGetTime() * -1) * 5, vec3(1.0f, 1.0f, 1.0f));
		transform2 = scale(transform2, vec3(0.5, 0.5, 0.5));

		//segundoShader.use();

		unsigned int transformLoc2 = glGetUniformLocation(segundoShader.ID, "transform");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, value_ptr(transform2));

		float colorverde = sin(glfwGetTime()) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(segundoShader.ID, "nuestroColor");
		glUniform4f(vertexColorLocation, 0.0f, colorverde, 0.0f, 1.0f);
		
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	/*glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);*/
	glfwTerminate();
	return 0;
}
//Instancía un cubo, sin rotación.
void Instantiate(Shader& _shader, vec3 _position)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
}

//Instanciar con una escala
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con la rotación indicada 
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con escala y rotación indicadas 
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
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