#include "Shader.h"
#include <string>
#include <cstring>
#include <fstream>

Shader::Shader(){
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode){
	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation){
	std::ifstream file(fileLocation, std::ios::in);
	std::string content;

	if(!file.is_open()){
		std::cout << "Failed to open file " << fileLocation << "\n";
		return "";
	}
	std::string line = "";
	while(!file.eof()){
		std::getline(file, line);
		content.append(line + "\n");
	}
	std::cout << content << std::endl;
	file.close();
	return content;
}

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation){
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode){
	shaderID = glCreateProgram();

	if(!shaderID){
		std::cout << "Error Creating Shader Program" << std::endl;
		return;
	}
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program: \n" << eLog << std::endl;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: \n" << eLog << std::endl;
		return;
	}
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
  uniformView = glGetUniformLocation(shaderID, "view");
  uniformAmbientColor = glGetUniformLocation(shaderID, "dirLight.color");
  uniformAmbientIntensity = glGetUniformLocation(shaderID, "dirLight.ambientIntensity");
}

GLuint Shader::GetProjectionLocation(){
	return uniformProjection;
}
GLuint Shader::GetModelLocation(){
	return uniformModel;
}
GLuint Shader::GetViewLocation(){
  return uniformView;
}
GLuint Shader::GetAmbientIntensityLocation(){
  return uniformAmbientIntensity;
}
GLuint Shader::GetAmbientColorLocation(){
  return uniformAmbientColor;
}

void Shader::UseShader(){
	glUseProgram(shaderID);
}

void Shader::ClearShader(){
	if(shaderID!=0){
		glDeleteProgram(shaderID);
	}
	uniformProjection = 0;
	uniformModel = 0;
}


void Shader::AddShader(GLuint theProgram, const char* code, GLenum shaderType){
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = code;

	GLint codeLength[1];
	codeLength[0] = strlen(code);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = {0};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if(!result){
		glGetProgramInfoLog(theShader, 1024, NULL, eLog);
		std::cout << "Error Compiling " << std::hex << shaderType << " Shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
	glDeleteShader(theShader);
}

Shader::~Shader(){
	ClearShader();
}
