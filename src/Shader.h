#pragma once

#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
  GLuint GetAmbientColorLocation();
  GLuint GetDirectionLocation();
  GLuint GetDiffuseIntensityLocation();
  GLuint GetSpecularIntensityLocation();
  GLuint GetShininessLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView;
  GLuint uniformAmbientIntensity, uniformAmbientColor;
  GLuint uniformDirection, uniformDiffuseIntensity;
  GLuint uniformSpecularIntensity, uniformShininess;
	
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* code, GLenum shaderType);
};
