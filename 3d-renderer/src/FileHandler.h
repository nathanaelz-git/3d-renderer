#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>


class FileHandler
{
public:
   FileHandler();
   std::string GetObjectFile();
   std::string GetTextureFile(std::string &name);

private: 
   std::string GetFileName(std::string path);
};



