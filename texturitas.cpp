#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp> //Operaciones avanzadas
#include <glm/gtc/matrix_transform.hpp> //Transformaciones matriciales
#include <glm/gtc/type_ptr.hpp> //Transformación de tipos de datos

#include <shader/shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void processInput(GLFWwindow* window); //acciones para nuestra ventana

//medidas de la pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

using namespace glm;

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

	//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}

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
		/*-1.0f, 1.0f, 0.0f, // top left
		-1.0f, 0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f, // bottom right
		0.5f,  1.0f, 0.0f, // top right*/
		//Cordenadas de frente
		0.5f, 0.5f, 0.0f, //0 - Arriba - Derecha
		-0.5f, 0.5f, 0.0f, //1 - Arriba - Izquierda
		-0.5f, -0.5f, 0.0f, //2 - Abajo - Izquierda
		0.5f, -0.5f, 0.0f, //3 - Abajo - Derecha

		//Cordenadas de atrás
		0.5f, 0.5f, -1.0f, //4 - Arriba - Derecha
		-0.5f, 0.5f, -1.0f, //5 - Arriba - Izquierda
		-0.5f, -0.5f, -1.0f, //6 - Abajo - Izquierda
		0.5f, -0.5f, -1.0f //7 - Abajo - Derecha
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
		1
	};

	//---------------primer vertex-----------------------

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
	//colores
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	//texturas
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//--------------- segundo vertex------------
	unsigned int VBO2, VAO2, EBO2; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	//unir o linkear
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangulos), triangulos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestriangulo), indicestriangulo, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
	unsigned char* data = stbi_load("Alam.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fallo al cargar la primera textura" << std::endl;
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
	data = stbi_load("Background.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fallo al cargar la segunda textura" << std::endl;
	}
	stbi_image_free(data);

	nuestroShader.use();
	nuestroShader.setInt("textura1", 0);
	nuestroShader.setInt("textura2", 1);

	glBindVertexArray(VAO2);
	//loop para que se pueda visualizar nuestra pantalla
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Renderizado
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//activado  y union de las texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);

		mat4 transform = mat4(1.0f);
		//transform = translate(transform, vec3(0.0f, 0.0f, 0.0f));
		transform = rotate(transform, (float)glfwGetTime(), vec3(0.0f, 1.0f, 1.0f));
		//transform = scale(transform, vec3(3.5f, 3.0f, 3.0f));

		nuestroShader.use();

		unsigned int transformLoc = glGetUniformLocation(nuestroShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

		mat4 transform2 = mat4(1.0f);
		transform2 = translate(transform2, vec3(3.0f, 0.0f, 0.0f));
		//transform2 = rotate(transform2, (float)glfwGetTime(), vec3(0.0f, 1.0f, -1.0f));
		//transform2 = scale(transform2, vec3(0.5f, 1.0f, 1.0f));

		segundoShader.use();

		unsigned int transformLoc2 = glGetUniformLocation(segundoShader.ID, "transform2");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, value_ptr(transform2));
		
		float colorverde = sin(glfwGetTime()) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(segundoShader.ID, "nuestroColor");
		glUniform4f(vertexColorLocation, 0.0f, colorverde, 0.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, 10, GL_UNSIGNED_INT, 0);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//nuestra primera configuracion de la camara
	glViewport(0, 0, width, height);
}