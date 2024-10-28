#pragma once

#include <glad/glad.h>

#include <vector>

class ElementBuffer {
public:
  unsigned int ID;
  unsigned int indices_size;
  ElementBuffer(const std::vector<unsigned int>& indices);

  void bind();
  void unbind();
  void destroy();
};
