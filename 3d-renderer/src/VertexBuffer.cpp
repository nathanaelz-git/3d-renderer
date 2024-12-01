#include "VertexBuffer.h"

// TODO: change GLsizeiptr to size_t after
VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices) 
{
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const std::vector<glm::vec3>& positions) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
}

void VertexBuffer::bind() 
{
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() 
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::destroy() 
{
  glDeleteBuffers(1, &ID);
}