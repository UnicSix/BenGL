#include <cstddef>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(){

	int WIDTH = 800;
	int HEIGHT = 600;

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

	// loop until window closed
	while(!glfwWindowShouldClose(mainWindow)){
		// get and handle user input events
		glfwPollEvents();
		// clear window
		glClearColor(0.4f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
