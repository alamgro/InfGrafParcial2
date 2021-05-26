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

/*
Tenemos luz:
	-Ambiental: luz homogénea, que se va reflejar en un ángulo de 90° que va a iluminar todos los objetos de nuestro shader
	-Especular: es un punto de luz concentrado dentro de la figura. Entre más grande sea el valor, se concentrará más en un punto
	-combinada: Para crear efectos mejores
*/
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana

//medidas de la pantalla
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

//camara
Camara camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Punto donde estará posicionada nuestra luz
vec3 posLuz(0.0f, 2.0f, 0.0f);

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
		std::cout << "Fallo en gcrear GLFW y la ventana" << std::endl;
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

	float vertices[]{
		//posiciones		//cómo afecta
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//luces
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//segundo shader
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

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
		nuestroShader.setVec3("light.position", posLuz);
		nuestroShader.setVec3("viewPos", camera.Position);
		//posLuz = vec3(posLuz.x, posLuz.y, 2.0f - (glfwGetTime() * 0.25f));
		float radio = 3.0f;
		float multipVelocidad = 1.2f;
		posLuz = vec3((radio * cos(glfwGetTime() * multipVelocidad)), posLuz.y, posLuz.z);

		nuestroShader.setVec3("lightPos", posLuz);
		nuestroShader.setVec3("viewPos", camera.Position);
		

		//Propiedades avanzadas de la luz
		vec3 lightColor(1.0f);
		/*lightColor.x = sin(glfwGetTime() * 2.21f);
		lightColor.y = sin(glfwGetTime() * 1.33f);
		lightColor.z = sin(glfwGetTime() * 3.18f);*/
		vec3 diffuseColor = lightColor * vec3(0.5f);
		vec3 ambientColor = diffuseColor * vec3(0.2f);
		nuestroShader.setVec3("light.ambient", ambientColor);
		nuestroShader.setVec3("light.diffuse", diffuseColor);
		nuestroShader.setVec3("light.specular", vec3(1.0f));


		//mvp
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		glBindVertexArray(VAO);

		mat4 model = mat4(1.0f);
		model = translate(model, vec3(-4.2f, 0.0f, 0.0f));

#pragma region ESMERALDA
		//Propiedades de los materiales
		nuestroShader.setVec3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
		nuestroShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		nuestroShader.setVec3("material.specular", 0.633f, 0.727811f, 0.633f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region JADE
		nuestroShader.setVec3("material.ambient", 0.135f, 0.2225f, 0.1575f);
		nuestroShader.setVec3("material.diffuse", 0.54f, 0.89f, 0.63f);
		nuestroShader.setVec3("material.specular", 0.316228f, 0.316228f, 0.316228f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
		model = translate(model, vec3(1.2f, 0.0f, 0.0f));//Propiedades de los materiales
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region OBSIDIANA
		model = translate(model, vec3(1.2f, 0.0f, 0.0f)); 
		nuestroShader.setVec3("material.ambient", 0.05375f, 0.05f, 0.06625f);
		nuestroShader.setVec3("material.diffuse", 0.18275f, 0.17f, 0.22525f);
		nuestroShader.setVec3("material.specular", 0.332741f, 0.328634f, 0.346435f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.3f);
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region PERLA
		nuestroShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
		nuestroShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
		nuestroShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.088f);
		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region RUBY
		nuestroShader.setVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
		nuestroShader.setVec3("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		nuestroShader.setVec3("material.specular", 0.727811f, 0.626959f, 0.626959f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
		model = translate(model, vec3(1.2f, 0.0f, 0.0f)); 
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region TURQUESA
		nuestroShader.setVec3("material.ambient", 0.1f, 0.18725f, 0.1745f);
		nuestroShader.setVec3("material.diffuse", 0.396f, 0.74151f, 0.69102f);
		nuestroShader.setVec3("material.specular", 0.297254f, 0.30829f, 0.306678f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
		model = translate(model, vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

		//Configurar punto de luz
		luzShader.use();
		luzShader.setMat4("projection", projection);
		luzShader.setMat4("view", view);
		model = mat4(1.0f);
		model = translate(model, posLuz);
		model = scale(model, vec3(0.2f));
		//model = translate(model, vec3(posLuz.x, posLuz.y, -glfwGetTime()));
		model = rotate(model, 45.0f, vec3(0.0f, 1.0f, 0.0f));
		luzShader.setMat4("model", model);
		
		//renderizar luz
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
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