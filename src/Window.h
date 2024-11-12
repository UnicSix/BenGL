#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialise();
	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose(){ return glfwWindowShouldClose(mainWindow); }
	void swapBuffers(){glfwSwapBuffers(mainWindow);}

  bool* getKeys(){return keys;}
  GLfloat getXChange();
  GLfloat getYChange();
  GLFWwindow* getWindow();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
  bool keys[1024];
  GLfloat lastX;
  GLfloat lastY;
  GLfloat xChange;
  GLfloat yChange;
  GLfloat mouseSensitivity;
  bool mouseFirstMoved;
  bool mouseDragging;

  void createCallbacks();
  static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
  static void handleMouseDrag(GLFWwindow* window, double xPos, double yPos);
};
