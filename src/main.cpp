#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"
#include "glm/trigonometric.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Window mainWindow;
const GLint WIDTH = 940;
const GLint HEIGHT = 800;
const float toRadians = 3.14159265f / 180.0f;
std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<std::unique_ptr<Shader>> shaderList;
Camera mainCamera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.00005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

Texture catTex;
Texture cabaTex;
Texture woodenFloor;

Light mainLight(0.0f,1.0f,0.0f,0.4f);

static const char* vShader = "../res/shaders/vertex.glsl";
static const char* fShader = "../res/shaders/fragment.glsl";


void CreateShaders(){
  std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
  shader1->CreateFromFile(vShader, fShader);
  shaderList.push_back(std::move(shader1));
}

void CreateObjects(){
  GLfloat vertices[] = {
    // positions        // texture coords
    -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
     0.0f, -1.0f,  1.0f, 1.0f, 0.0f,
     1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
     0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
  };
  unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2,
  };

  GLfloat cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 1
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 3
                                      // 
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // 4
    -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, // 5
     0.5f,  0.5f,  0.0f,  1.0f, 1.0f, // 6
     0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // 7
                                      // 
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // 4
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 0
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 3
     0.5f, -0.5f,  0.0f,  1.0f, 0.0f, // 7
                                      // 
    -0.5f,  0.5f,  0.0f,  0.0f, 0.0f, // 5
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 1
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
     0.5f,  0.5f,  0.0f,  1.0f, 0.0f, // 6

    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f, // 4
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 0
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 1
    -0.5f,  0.5f,  0.0f,  1.0f, 0.0f, // 5

     0.5f,  0.5f,  0.0f,  0.0f, 0.0f, // 6
     0.5f, -0.5f,  0.0f,  0.0f, 1.0f, // 7
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 3
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 2
  };

  unsigned int cubeIndices[] = {
    0 ,  1 ,  2 ,  2 ,  3 ,  0 , // Face back
    4 ,  7 ,  5 ,  6 ,  5 ,  7 , // Face front
    8 ,  9 ,  10,  10,  11,  8 , // Face left
    12,  13,  14,  14,  15,  12, // Face right
    16,  17,  18,  18,  19,  16, // Face up
    20,  21,  22,  22,  23,  20, // Face bottom
  };

  std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
  obj1->CreateMesh(vertices, indices, 40, 24);
  meshList.push_back(std::move(obj1)); 
  //unique_ptr's can't have copies
  //the address of the local var becomes invalid in the end of the scope
  std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
  obj2->CreateMesh(cubeVertices, cubeIndices, 120, 36);
  meshList.push_back(std::move(obj2)); 
}

int main(){

  mainWindow = Window(WIDTH, HEIGHT); 
  mainWindow.Initialise();

  CreateObjects();
  CreateShaders();

  mainCamera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 
                      -90.0f, 0.0f, 5.0f, 0.5f);

  catTex = Texture("../res/textures/johnnyCat.png");
  catTex.LoadTexture();
  cabaTex = Texture("../res/textures/cabaWut.png");
  cabaTex.LoadTexture();
  woodenFloor = Texture("../res/textures/wooden-floor.png");
  woodenFloor.LoadTexture();
  GLuint uniformProjection = 0;
  GLuint uniformModel = 0;
  GLuint uniformView = 0;
  GLuint uniformAmbientIntensity = 0;
  GLuint uniformAmbientColor = 0;

  glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
  // loop until window closed
  while(!mainWindow.getShouldClose()){
    GLfloat now = glfwGetTime();
    deltaTime = now-lastTime;
    lastTime = now;

    // get and handle user input events
    glfwPollEvents();
    mainCamera.keyControl(mainWindow.getKeys(), deltaTime);
    mainCamera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

    // mainCamera.keyControl(mainWindow.get, GLfloat deltaTime)

    if(direction){
      // triOffset += triIncrement;
    }
    else{
      triOffset -= triIncrement;
    }
    if(abs(triOffset) >= triMaxOffset){
      direction = !direction;
    }
    curAngle += deltaTime*25.0f;  
    if(curAngle >= 360.0f){
      curAngle = 0.0f;
    }
    if(curSize >= maxSize || curSize <= minSize){
      sizeDirection = !sizeDirection;
    }

    // clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0]->UseShader();
    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformModel = shaderList[0]->GetModelLocation();
    uniformView = shaderList[0]->GetViewLocation();
    uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
    uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();

    mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor);

    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(-0.5f, 0.3f, -2.5f));
    model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
    model = glm::rotate(model, glm::radians(curAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.calculateViewMatrix()));
    woodenFloor.UseTexture();
    meshList[0]->RenderMesh();

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(0.8f, 0.3f, -2.5f));
    model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
    model = glm::rotate(model, glm::radians(curAngle), glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    catTex.UseTexture();
    meshList[1]->RenderMesh();

    glUseProgram(0);
    mainWindow.swapBuffers();
  }

  return 0;
}
