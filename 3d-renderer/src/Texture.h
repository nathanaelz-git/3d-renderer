#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"Shader.h"

#include <iostream>
#include <string>

class Texture {
public:
  unsigned int textureID;
  std::string type;
  std::string path;
  // Texture(const char* image, const std::string& textureType, unsigned int slot, unsigned int format, unsigned int pixelType);
  Texture(const std::string& texturePath, const std::string& directory, std::string& typeName, bool gamma);

  // Binds a texture
  void bind();
  // Unbinds a texture
  void unbind();
  // Deletes a texture
  void destroy();
};

