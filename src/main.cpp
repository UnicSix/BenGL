#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cwchar>
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

Material shinyMaterial;
Material dullMaterial;

Light mainLight( 1.0f, 1.0f, 1.0f, 0.1f,
                 0.0f,-5.0f, 0.0f, 1.0f);

static const char* vShader = "../res/shaders/vertex.glsl";
static const char* fShader = "../res/shaders/fragment.glsl";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, 
                        GLfloat* vertices,     unsigned int verticesCount, unsigned int vLength, 
                        unsigned int normalOffset){
  for (size_t i=0; i<indiceCount; i += 3) {
    unsigned int in0 = indices[i]* vLength;
    unsigned int in1 = indices[i+1]* vLength;
    unsigned int in2 = indices[i+2]* vLength;
    glm::vec3 v1(vertices[in1]-vertices[in0], vertices[in1+1]-vertices[in0+1], vertices[in1+2]-vertices[in0+2]);
    glm::vec3 v2(vertices[in2]-vertices[in0], vertices[in2+1]-vertices[in0+1], vertices[in2+2]-vertices[in0+2]);
    glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

    in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
    vertices[in0] += normal.x; vertices[in0+1] += normal.y; vertices[in0+2] += normal.z;
    vertices[in1] += normal.x; vertices[in1+1] += normal.y; vertices[in1+2] += normal.z;
    vertices[in2] += normal.x; vertices[in2+1] += normal.y; vertices[in2+2] += normal.z;
  }
  for (size_t i=0; i<verticesCount/vLength; i++) {
    unsigned int nOffset = i* vLength + normalOffset;
    glm::vec3 vec(vertices[nOffset], vertices[nOffset+1], vertices[nOffset+2]);
    vec = glm::normalize(vec);
    vertices[nOffset] = vec.x; vertices[nOffset+1] = vec.y; vertices[nOffset+2] = vec.z;
  }
}

void CreateShaders(){
  std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
  shader1->CreateFromFile(vShader, fShader);
  shaderList.push_back(std::move(shader1));
}

void CreateObjects(){
  GLfloat vertices[] = {
    // Positions         TexCoords    Normals
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
     0.0f, -1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f
  };
  unsigned int indices[] = {
    0, 3, 1,
    1, 3, 2,
    2, 3, 0,
    0, 1, 2 
  };

  GLfloat cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 0
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 1
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 2
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 3
                                       
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 4
    -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 5
     0.5f,  0.5f,  0.0f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 6
     0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 7
                                       
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 4
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 0
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 3
     0.5f, -0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 7
                                       
    -0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 5
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 1
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 2
     0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 6
    //                                    
    -0.5f, -0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 4
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 0
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 1
    -0.5f,  0.5f,  0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 5
                                       
     0.5f,  0.5f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,// 6
     0.5f, -0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 7
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f,// 3
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f // 2
  };

  unsigned int cubeIndices[] = {
    0 ,  2 ,  1 ,  2 ,  0 ,  3 , // Face back
    4 ,  5 ,  7 ,  6 ,  7 ,  5 , // Face front
    8 ,  11,  9 ,  11,  10,  9 , // Face left
    12,  13,  15,  14,  15,  13, // Face right
    16,  17,  19,  18,  19,  17, // Face up
    20,  23,  21,  22,  21,  23  // Face bottom
  };

  calcAverageNormals(indices, 12, vertices, 32, 8, 5);
  calcAverageNormals(cubeIndices, 36, cubeVertices, 192, 8, 5);

  std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
  obj1->CreateMesh(vertices, indices, 32, 12);
  meshList.push_back(std::move(obj1)); 
  //unique_ptr's can't have copies
  //the address of the local var becomes invalid in the end of the scope
  std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
  obj2->CreateMesh(cubeVertices, cubeIndices, 192, 36);
  meshList.push_back(std::move(obj2)); 
}

int main(){
  mainWindow = Window(WIDTH, HEIGHT); 
  mainWindow.Initialise();
  {
    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
    ImGui_ImplOpenGL3_Init();
    // ImGui Setup

    CreateObjects();
    CreateShaders();

    mainCamera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), 
                        glm::vec3(0.0f, 1.0f, 0.0f), 
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
    GLuint uniformEyePos = 0;
    GLuint uniformAmbientIntensity = 0;
    GLuint uniformAmbientColor = 0;
    GLuint uniformDirection = 0;
    GLuint uniformDiffuseIntensity = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;

    glm::vec4 clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
    // loop until window closed
    while(!mainWindow.getShouldClose()){
      GLfloat now = glfwGetTime();
      deltaTime = now-lastTime;
      lastTime = now;

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      { // ImGui Control Panel
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Control Panel");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useless text.");               // Display some text (you can use a format strings too)
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))
        // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.1f ms/frame (%.0f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
      }
      // Rendering
      ImGui::Render();

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
      mainLight.modLightColor(clear_color.r, clear_color.g, clear_color.b);
      glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.z);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      shaderList[0]->UseShader();
      uniformProjection = shaderList[0]->GetProjectionLocation();
      uniformModel = shaderList[0]->GetModelLocation();
      uniformView = shaderList[0]->GetViewLocation();
      uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
      uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();
      uniformDirection = shaderList[0]->GetDirectionLocation();
      uniformDiffuseIntensity = shaderList[0]->GetDiffuseIntensityLocation();
      uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
      uniformShininess = shaderList[0]->GetShininessLocation();

      mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

      glm::mat4 model(1.0);
      model = glm::translate(model, glm::vec3(-0.5f, 0.3f,  -3.5f));
      model = glm::rotate(model, glm::radians(curAngle), glm::vec3(1.0f, 1.0f, 0.0f));
      model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
      glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
      glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(mainCamera.calculateViewMatrix()));
      glUniform3f(uniformEyePos, mainCamera.getCameraPosition().x, mainCamera.getCameraPosition().y, mainCamera.getCameraPosition().z);
      woodenFloor.UseTexture();
      shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
      meshList[0]->RenderMesh();

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(0.8f, 0.3f, -2.5f));
      model = glm::rotate(model, glm::radians(curAngle), glm::vec3(-1.0f, -1.0f, -1.0f));
      model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
      glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
      catTex.UseTexture();
      meshList[1]->RenderMesh();

      glUseProgram(0);
      mainWindow.swapBuffers();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
  return 0;
}
