#include "Window.h"
#include <cstddef>
#include <iostream>

Window::Window(){
	width = 800;
	height = 600;
}
Window::Window(GLint windowWidth, GLint windowHeight){
	width = windowWidth;
	height = windowHeight;
}

int Window::Initialise(){
	if(!glfwInit()){
		std::cout << "GLFW initialisation failed" << std::endl;
		glfwTerminate();
		return 1;
	}
	// setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Core profile = No backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window
	mainWindow = glfwCreateWindow(width, height, "BenGL", NULL, NULL);
	if(!mainWindow){
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	// Set current context
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if(error != GLEW_OK){
		std::cout << "Error: " << glewGetErrorString(error) << "\n";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	return 0;
}

Window::~Window(){
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
