#include "VertexArray.h"

VertexArray::VertexArray() 
{
  glGenVertexArrays(1, &ID);
}

void VertexArray::linkAttrib(VertexBuffer VBO, unsigned int layout, unsigned int numComponents, 
              GLenum type, GLsizeiptr stride, void*offset) 
{
  VBO.bind();
  if (type == GL_INT) glVertexAttribIPointer(layout, numComponents, type, stride, offset);
  else glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(layout);
  VBO.unbind();
}

void VertexArray::bind() 
{
  glBindVertexArray(ID);
}

void VertexArray::unbind() 
{
  glBindVertexArray(0);
}

void VertexArray::destroy() 
{
  glDeleteVertexArrays(1, &ID);
}
