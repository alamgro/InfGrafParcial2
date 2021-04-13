// InfGraf1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/*
* Inicializar, comprobar, liberar
*/

void FrameBuffer_size_callback(GLFWwindow* _window, int _width, int _height); //Aquí se almacena nuestra ventana, lienzo
void ProcessInput(GLFWwindow* _window);

//Medidas de la pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* VertexShaderSource = "#version 330 core \n"
	"layout (location = 0) in vec3 aPos; \n"
	"layout (location = 1) in vec3 aColor; \n"
	"out vec3 ourColor; \n"
	"void main()\n"
	"{\n"
		"gl_Position = vec4(aPos, 1.0f); \n"
		"ourColor = aColor; \n"
	"}\n\0";

const char* FragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 ourColor;\n"
	"void main()\n" "{\n"
	" FragColor = vec4(ourColor, 1.0f);\n" "}\n\0";

int main() {
	//inicializar GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//esto
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Atole", NULL, NULL);
	if (window == NULL) {
		std::cout << "Fallo en crear GLFW y la venta, ni pepe." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBuffer_size_callback);

	//Comprobar que GLAD se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "No se está ejecutando GLAD" << std::endl;
		return -1;
	}

#pragma region CHECAR ERRORES EN VERTEX
	//Construir y compilar todo el programa
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //Creamos el vertexShader
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL); //Asignamos los atributos
	glCompileShader(vertexShader); //Compilar
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error al compilar nuestro Vertex\n" << infoLog << std::endl;
	}
#pragma endregion

#pragma region CHECAR ERRORES EN FRAGMENT
	//Construir y compilar todo el programa
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //Creamos el vertexShader
	glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL); //Asignamos los atributos
	glCompileShader(fragmentShader); //Compilar

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error al compilar nuestro fragment.\n" << infoLog << std::endl;
	}
#pragma endregion

#pragma region LINKEAR VERTEX Y SHADER
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Comprobar que los linkeos estén bien
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error en los linkeos del programa.\n" << infoLog << std::endl;
	}
#pragma endregion

#pragma region LIBERACION DE GL
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion
	/*//Iniciales nombre (AGRO)
	float vertices[] {
		//Letra A
		-1.0f, 1.0f, 0.0f, 0.8, 0.0f, 0.3f, //0
		-1.0f, 0.0f, 0.0f, 0.0, 0.6f, 0.0f,//1
		-0.9f, 0.0f, 0.0f, 0.5, 0.1f, 0.8f, //2
		-0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.2f, //3
		-0.9f, 0.9f, 0.0f, 0.4f, 0.0f, 0.1f, //4
		-0.6f, 0.9f, 0.0f, 0.0f, 0.2f, 0.8f, //5
		-0.6f, 1.0f, 0.0f, 0.6f, 0.0f, 0.2f, //6
		-0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, //7
		-0.5f, 0.0f, 0.0f, 0.1f, 0.0f, 0.2f, //8
		-0.5f, 1.0f, 0.0f, 0.3f, 0.3f, 0.3f, //9
		-0.9f, 0.5f, 0.0f, 0.9f, 0.9f, 0.4f, //10
		-0.9f, 0.4f, 0.0f, 0.0f, 0.3f, 0.0f, //11
		-0.6f, 0.4f, 0.0f, 0.3f, 0.0f, 0.9f, //12
		-0.6f, 0.5f, 0.0f, 0.0f, 0.1f, 0.3f, //13

		//Letra G
		-0.5f, 1.0f, 0.0f, 0.0, 0.8f, 0.0f, //14
		-0.5f, 0.0f, 0.0f, 0.9, 0.0f, 0.6f, //15
		-0.4f, 0.0f, 0.0f, 0.0, 0.5f, 0.5f, //16
		-0.4f, 1.0f, 0.0f, 0.1, 0.2f, 0.3f, //17
		-0.4f, 0.9f, 0.0f, 0.5, 0.2f, 0.0f, //18
		0.0f, 0.9f, 0.0f, 0.4, 0.6f, 0.7f, //19
		0.0f, 1.0f, 0.0f, 0.2, 0.0f, 0.1f, //20
		-0.4f, 0.1f, 0.0f, 0.9, 0.9f, 0.9f, //21
		0.0f, 0.0f, 0.0f, 0.2, 0.1f, 0.0f, //22
		0.0f, 0.1f, 0.0f, 0.0, 0.5f, 0.6f, //23
		-0.1f, 0.1f, 0.0f, 0.0, 0.0f, 0.8f, //24
		-0.1f, 0.4f, 0.0f, 0.0, 0.2f, 0.1f, //25
		0.0f, 0.4f, 0.0f, 0.1, 0.8f, 0.0f, //26
		0.0f, 0.5f, 0.0f, 0.3, 0.0f, 0.7f, //27
		-0.1f, 0.5f, 0.0f, 0.3, 0.3f, 0.1f, //28
		-0.3f, 0.5f, 0.0f, 0.0, 0.7f, 0.0f, //29
		
		//Letra R
		0.0f, 1.0f, 0.0f, 0.3, 0.0f, 0.1f, //30
		0.0f, 0.0f, 0.0f, 0.0, 0.8f, 0.2f, //31
		0.1f, 0.0f, 0.0f, 0.8, 0.2f, 0.3f, //32
		0.1f, 1.0f, 0.0f, 0.1, 0.8f, 0.8f, //33
		0.1f, 0.85f, 0.0f, 0.0, 0.9f, 0.3f, //34
		0.5f, 0.7f, 0.0f, 0.2, 0.0f, 0.2f, //35
		0.1f, 0.5f, 0.0f, 0.9, 0.9f, 0.9f, //36
		0.1f, 0.35f, 0.0f, 0.4, 0.0f, 0.2f, //37
		0.5f, 0.0f, 0.0f, 0.0, 0.6f, 0.4f, //38

		//Letra O
		0.5f, 1.0f, 0.0f, 0.0, 0.0f, 0.3f, //39
		0.5f, 0.0f, 0.0f, 0.5, 0.0f, 0.2f, //40
		0.6f, 0.0f, 0.0f, 0.7, 0.2f, 0.0f, //41
		0.6f, 1.0f, 0.0f, 0.5, 0.0f, 0.8f, //42
		0.6f, 0.9f, 0.0f, 0.1, 0.5f, 0.2f, //43
		0.9f, 0.9f, 0.0f, 0.2, 0.6f, 0.0f, //44
		0.9f, 1.0f, 0.0f, 0.0, 0.9f, 0.0f, //45
		0.9f, 0.0f, 0.0f, 0.0, 0.0f, 0.5f, //46
		1.0f, 0.0f, 0.0f, 0.8, 0.2f, 0.0f, //47
		1.0f, 1.0f, 0.0f, 0.0, 0.8f, 0.1f, //48
		0.6f, 0.1f, 0.0f, 0.9, 0.9f, 0.0f, //49
		0.9f, 0.1f, 0.0f, 0.9, 0.9f, 0.9f, //50
	};

	unsigned int indices[]{
		//Letra A
		0, 1, 2,
		0, 2, 3,
		3, 4, 5,
		3, 5, 6,
		6, 7, 8,
		6, 8, 9,
		10, 11, 12,
		10, 12, 13,

		//Letra G
		14, 15, 16,
		14, 16, 17,
		17, 18, 19,
		17, 19, 20,
		21, 16, 22,
		21, 22, 23,
		25, 24, 23,
		25, 23, 26,
		28, 25, 26,
		28, 26, 27,
		29, 25, 28,

		//Letra R
		30, 31, 32,
		30, 32, 33,
		33, 34, 35,
		36, 37, 35,
		36, 37, 38,

		//Letra O
		39, 40, 41,
		39, 41, 42,
		42, 43, 44,
		42, 44, 45,
		45, 46, 47,
		45, 47, 48,
		49, 41, 46,
		49, 46, 50,
		
		/*0, 3, 2,
		0, 2, 1
	};
	*/

	/*
	float vertices[]{
		0.1f, 0.5f, 0.0f, 1.0, 0.0f, 0.0f, //0
		0.1f, -0.5f, 0.0f, 0.0, 0.0f, 0.0f, //1
		0.2f, -0.5f, 0.0f, 0.0, 1.0f, 0.0f, //2
		0.2f, 0.5f, 0.0f, 1.0, 0.0f, 0.0f, //3
		0.2f, 0.4f, 0.0f, 0.0, 0.0f, 0.0f, //4
		0.6f, 0.4f, 0.0f, 0.0, 1.0f, 0.0f, //5
		0.6f, 0.5f, 0.0f, 0.0, 0.0f, 0.0f, //6
		0.2f, 0.2f, 0.0f, 0.0, 1.0f, 0.0f, //7
		0.5f, 0.2f, 0.0f, 0.0, 0.0f, 0.0f, //8
		0.5f, 0.1f, 0.0f, 1.0, 0.0f, 0.0f, //9
		0.2f, 0.1f, 0.0f, 0.0, 0.0f, 0.0f, //10
	};
	
	unsigned int indices[]{
		0, 1, 3,
		3, 1, 2,
		3, 4, 6,
		4, 5, 6,
		7, 10, 8,
		10, 9, 8,
	};
	*/

	float vertices[] {
		0.0f, 1.0f, 0.0f, 0.3, 0.0f, 0.1f, //Arriba - Medio
		-0.5f, 0.5f, 0.0f, 1.0, 0.0f, 7.0f, //Arribita - Izquierda
		-0.5f, -0.5f, 0.0f, 0.4, 0.0f, 0.9f, //Abajito - Izquierda
		0.0f, -1.0f, 0.0f, 0.0, 0.2f, 0.0f, //Abajo - Medio
		0.5f, -0.5f, 0.0f, 0.0, 0.2f, 0.5f, //Abajito - Derecha
		0.5f, 0.5f, 0.0f, 0.3, 0.3f, 0.9f, //Arribita - Derecha
	};

	unsigned int indices[] {
		0, 1, 5,
		1, 2, 4,
		1, 4, 5,
		2, 3, 4,
	};

	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object, Extended Buffer Object
	glGenVertexArrays(1, &VAO); //Generar arreglo de vertex
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	//Unir o linkear
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Vincular la característica
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Indicar que son posiciones
	//glBindVertexArray(VAO);

	glUseProgram(shaderProgram);
	//glBindVertexArray(VAO); //Esto va en el bucle para colores fijos, afuera 
	float timeValue = glfwGetTime();
	float senalVerde = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, senalVerde, 0.0f, 1.0f);

	//Loop para que se pueda visualizar nuestra pantalla
	while (!glfwWindowShouldClose(window)) {
		ProcessInput(window);

		//Renderizado
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*
		//Dibujar primer cuadro
		glUseProgram(shaderProgram);
		//glBindVertexArray(VAO); //Esto va en el bucle para colores fijos, afuera 
		float timeValue = glfwGetTime();
		float senalVerde = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, senalVerde, 0.0f, 1.0f);
		*/
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		glDrawElements(GL_TRIANGLES, 55, GL_UNSIGNED_INT, 0); //El segundo parámetro dice cuántos elementos tiene

		//Detectar eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//Liberar memoria o GLFW
	glfwTerminate();
	return 0;

}

void FrameBuffer_size_callback(GLFWwindow* _window, int _width, int _height) {
	//Primera configuración de la cámara
	glViewport(0, 0, WIDTH, HEIGHT);
}

void ProcessInput(GLFWwindow* _window) {
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(_window, true);
	}
}
