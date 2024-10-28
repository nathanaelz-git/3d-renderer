#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include <string>
#include <vector>

class Mesh {
public:
  // mesh data
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  
  // constructor
  Mesh(const std::vector<Vertex>& vertices,
       const std::vector<unsigned int>& indices,
       const std::vector<Texture>& textures);

  VertexArray VAO;
  VertexBuffer VBO;
  ElementBuffer EBO;

  // render the mesh
  void Draw(Shader &shader);

private:

  void setupMesh();
};

