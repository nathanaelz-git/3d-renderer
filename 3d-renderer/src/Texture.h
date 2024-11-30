#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include"Shader.h"

#include <iostream>
#include <string>

class Texture {
public:
  unsigned int textureID;
  std::string name;
  std::string path;
  bool selected;
  std::string filename;


  // Texture(const char* image, const std::string& textureType, unsigned int slot, unsigned int format, unsigned int pixelType);
  Texture(const std::string& texturePath, const std::string& directory, std::string& typeName, bool gamma);
  Texture(const std::string& path, std::string& typeName, bool gamma);

  // Binds a texture
  void bind();
  // Unbinds a texture
  void unbind();
  // Deletes a texture
  void destroy();
  // Generates the Texture
  void generate();
};

