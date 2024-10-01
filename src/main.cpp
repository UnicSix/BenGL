#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;
GLuint VAO, VBO, shader, uniformXMove, uniformYMove;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.00005f;

void CreateTriangle(){
	GLfloat vertices[] = {
		 0.0f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};

	// Bind VAO and VBO
	glGenVertexArrays(1, &VAO); // Assign ID to VAO
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO and VBO
}

// Vertex shader
static const char* vShader = "					\n\
#version 330									\n\
												\n\
layout (location = 0) in vec3 pos;				\n\
												\n\
uniform float xMove;							\n\
void main()										\n\
{												\n\
	gl_Position = vec4(pos.x+xMove, pos.y, pos.z, 1.0);\n\
}";

// Fragment shader
static const char* fShader = "					\n\
#version 330									\n\
out vec4 color;									\n\
												\n\
void main()										\n\
{												\n\
	color = vec4(0.4f, 0.8f, 0.75f, 1.0f);		\n\
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
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: \n" << eLog << std::endl;
	}
	uniformXMove = glGetUniformLocation(shader, "xMove");
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

	// set viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

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

		// clear window
		glClearColor(0.4f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glUniform1f(uniformXMove, triOffset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(0);
		glBindVertexArray(0);
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
