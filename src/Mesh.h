#pragma once
#include <GL/glew.h>
class Mesh
{
public:
  Mesh();
  void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
  void CreateMesh(GLfloat *vertices, GLfloat *texCoords, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
  void RenderMesh();
  void ClearMesh();

  ~Mesh();
private:
  GLuint VAO, VBO, IBO, TCBO;
  GLsizei indexCount;
};
