#include "ElementBuffer.h"

ElementBuffer::ElementBuffer(const std::vector<unsigned int>& indices) 
{
  indices_size = static_cast<unsigned int>(indices.size());
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void ElementBuffer::bind() 
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void ElementBuffer::unbind() 
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ElementBuffer::destroy() 
{
  glDeleteBuffers(1, &ID);
}