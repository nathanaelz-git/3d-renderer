#pragma once
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>



class ImportHandler
{
public:
   Platform platform;
   ImportHandler(Platform platform);
   void GetObjectFile();
};

enum Platform {
   WINDOWS, 
   LINUX, 
   MACOS
};

