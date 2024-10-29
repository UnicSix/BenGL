#include "Window.h"
#include "GLFW/glfw3.h"
#include <cstddef>
#include <iostream>

Window::Window(){
  width = 800;
  height = 600;

  for(size_t i=0; i<1024; i++){
    keys[i]=0;
  }
  xChange = 0.0f;
  yChange = 0.0f;
  mouseFirstMoved = true;
  mouseSensitivity = 0.1f;
}
Window::Window(GLint windowWidth, GLint windowHeight){
  width = windowWidth;
  height = windowHeight;

  for(size_t i=0; i<1024; i++){
    keys[i] = 0;
  }
  xChange = 0.0f;
  yChange = 0.0f;
  mouseFirstMoved = true;
  mouseSensitivity = 0.3f;
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

  // Handle key and mouse input
  createCallbacks();
  glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  glfwSetWindowUserPointer(mainWindow, this);
  return 0;
}

void Window::createCallbacks(){
  glfwSetKeyCallback(mainWindow, handleKeys);
  glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange(){
  GLfloat theChange = xChange*mouseSensitivity;
  xChange = 0.0f;
  return theChange;
}
GLfloat Window::getYChange(){
  GLfloat theChange = yChange*mouseSensitivity;
  yChange = 0.0f;
  return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action , int mode){
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if(key>=0 && key<1024){
    if(action == GLFW_PRESS){
      theWindow->keys[key]=true;
    } else if(action == GLFW_RELEASE){
      theWindow->keys[key] = false;
    }
  }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos){
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if(theWindow->mouseFirstMoved){
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
    theWindow->mouseFirstMoved = false;
  }

  theWindow->xChange = xPos - theWindow->lastX;
  theWindow->yChange = theWindow->lastY - yPos;
  theWindow->lastX = xPos;
  theWindow->lastY = yPos;
}

Window::~Window(){
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
