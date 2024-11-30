#include "Texture.h"

Texture::Texture(const std::string& texturePath, const std::string& directory, std::string& typeName, bool gamma)
{
  path = texturePath;
  name = typeName;
  selected = false;
  filename = std::string(path);
  
  filename = directory + '/' + filename;

  // Generates an OpenGL texture object
  glGenTextures(1, &textureID);

  // Stores the width, height, and the number of color channels of the image
  int width, height, nrComponents;
  // Flips the image so it appears right side up
  stbi_set_flip_vertically_on_load(true);

  // Reads the image from a file and stores it in bytes
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) 
  {
    unsigned int format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    // Assigns the texture to a Texture Unit
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Assigns the image to the OpenGL Texture object
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // Generates MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Configures the way the texture repeats (if it does at all)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Configures the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Deletes the image data as it is already in the OpenGL Texture object
    stbi_image_free(data);
  } 
  else 
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  // Unbinds the OpenGL Texture object so that it can't accidentally be modified
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& path, std::string& typeName, bool gamma)
{
   name = typeName;
   selected = false;
   filename = std::string(path);

   filename = path;

   // Generates an OpenGL texture object
   glGenTextures(1, &textureID);

   // Stores the width, height, and the number of color channels of the image
   int width, height, nrComponents;
   // Flips the image so it appears right side up
   stbi_set_flip_vertically_on_load(true);

   // Reads the image from a file and stores it in bytes
   unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
   if (data)
   {
      unsigned int format;
      if (nrComponents == 1)
         format = GL_RED;
      else if (nrComponents == 3)
         format = GL_RGB;
      else if (nrComponents == 4)
         format = GL_RGBA;

      // Assigns the texture to a Texture Unit
      glBindTexture(GL_TEXTURE_2D, textureID);
      // Assigns the image to the OpenGL Texture object
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      // Generates MipMaps
      glGenerateMipmap(GL_TEXTURE_2D);

      // Configures the way the texture repeats (if it does at all)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      // Configures the type of algorithm that is used to make the image smaller or bigger
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Deletes the image data as it is already in the OpenGL Texture object
      stbi_image_free(data);
   }
   else
   {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      stbi_image_free(data);
   }

   // Unbinds the OpenGL Texture object so that it can't accidentally be modified
   glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() 
{
  glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() 
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy() 
{
  glDeleteTextures(1, &textureID);
}

