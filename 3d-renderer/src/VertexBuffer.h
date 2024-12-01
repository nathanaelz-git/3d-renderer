#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
  //bone indexes which will influence this vertex
  int m_BoneIDs[MAX_BONE_INFLUENCE];
  //weights from each bone
  float m_Weights[MAX_BONE_INFLUENCE];
};


class VertexBuffer {
public:
  unsigned int ID;
  VertexBuffer(const std::vector<glm::vec3>& positions);
  VertexBuffer(const std::vector<Vertex>& vertices);

  void bind();
  void unbind();
  void destroy();
};
