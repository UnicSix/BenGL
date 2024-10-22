#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <string.h>
#include <utility>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 940;
const GLint HEIGHT = 800;
const float toRadians = 3.14159265f / 180.0f;
std::vector<std::unique_ptr<Mesh>> meshList;
std::vector<std::unique_ptr<Shader>> shaderList;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.00005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

static const char* vShader = "../res/shaders/vertex.shader";
static const char* fShader = "../res/shaders/fragment.shader";


void CreateShaders(){
	std::unique_ptr<Shader> shader1 = std::make_unique<Shader>();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(std::move(shader1));
}

void CreateObjects(){
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

	std::unique_ptr<Mesh> obj1 = std::make_unique<Mesh>();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(std::move(obj1)); 
	//unique_ptr's can't have copies
	//the address of the local var becomes invalid in the end of the scope
	std::unique_ptr<Mesh> obj2 = std::make_unique<Mesh>();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(std::move(obj2)); 
}

int main(){

	Window mainWindow = Window(WIDTH, HEIGHT);	
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	// loop until window closed
	while(!mainWindow.getShouldClose()){
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

		shaderList[0]->UseShader();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformModel = shaderList[0]->GetModelLocation();

		glm::mat4 model(1.0);
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, -2.5f));
		// model = glm::rotate(model, curAngle* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-triOffset, 0.5f, -5.5f));
		model = glm::scale(model, glm::vec3(curSize, curSize, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
