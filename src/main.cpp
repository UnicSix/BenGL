#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;
GLuint VAO, IBO, VBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.00005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

void CreateTriangle(){
	GLfloat vertices[] = {
		-1.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	// Bind VAO and VBO
	glGenVertexArrays(1, &VAO); // Assign ID to VAO
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO and VBO
}

// Vertex shader
static const char* vShader = "					\n\
#version 330									\n\
												\n\
layout (location = 0) in vec3 pos;				\n\
												\n\
out vec4 vCol;									\n\
												\n\
uniform mat4 model;							\n\
uniform mat4 projection;						\n\
void main()										\n\
{												\n\
	gl_Position = projection* model* vec4(pos, 1.0);\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);	\n\
}";

// Fragment shader
static const char* fShader = "					\n\
#version 330									\n\
in vec4 vCol;									\n\
out vec4 color;									\n\
												\n\
void main()										\n\
{												\n\
	color = vCol;		\n\
}";

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType){
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(theShader, 1024, NULL, eLog);
		std::cout << "Error Compiling " << shaderType << "Shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(program, theShader);
	glDeleteShader(theShader);
}

void CompileShader(){
	shader = glCreateProgram();

	if(!shader){
		std::cout << "Error Creating Shader Program" << std::endl;
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program: \n" << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: \n" << eLog << std::endl;
		return;
	}
	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection");
}

int main(){

	//Initialise GLFW
	if(!glfwInit()){
		std::cerr << "Failed to initialize GLFW" << std::endl;
		glfwTerminate();
		return 1;
	}

	// setup GLFW properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Core profile = No backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "BenGL", NULL, NULL);
	if(!mainWindow){
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK){
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

    std::cout << glGetString(GL_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);
	// set viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	// loop until window closed
	while(!glfwWindowShouldClose(mainWindow)){
		// get and handle user input events
		glfwPollEvents();

		if(direction){
			triOffset += triIncrement;
		}
		else{
			triOffset -= triIncrement;
		}
		if(abs(triOffset) >= triMaxOffset){
			direction = !direction;
		}
		curAngle += 0.05f;	
		if(curAngle >= 360.0f){
			curAngle = 0.0f;
		}
		if(curSize >= maxSize || curSize <= minSize){
			sizeDirection = !sizeDirection;
		}

		// clear window
		glClearColor(0.4f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		// model = glm::rotate(model, curAngle* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
