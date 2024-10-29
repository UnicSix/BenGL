#pragma once

#include <GL/glew.h>
#include "vendor/stb_image.h"

class Texture{
public:
  Texture();
  Texture(char*);
  ~Texture();

  void LoadTexture();
  void UseTexture();
  void ClearTexture();

private:
  GLuint textureID;
  int width, height, bitDepth;

  char* fileLocation;
};
