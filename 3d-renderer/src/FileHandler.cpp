#include "FileHandler.h"
#include <nfd/include/nfd.h>

std::string FileHandler::GetObjectFile() {
   std::cout << "Getting the Object File" << std::endl;

   nfdchar_t* outPath = NULL;
   
   nfdresult_t result = NFD_OpenDialog("obj", NULL, &outPath);
   

   if (result == NFD_OKAY) {
      puts("Success!");
      puts(outPath);
      return outPath;
      free(outPath);
   }
   else if (result == NFD_CANCEL) {
      puts("User pressed cancel.");
   }
   else {
      printf("Error: %s\n", NFD_GetError());
   }

   return "";
}

std::string FileHandler::GetTextureFile(std::string &name)
{
   std::cout << "Getting the Texture File" << std::endl;

   nfdchar_t* outPath = NULL;

   nfdresult_t result = NFD_OpenDialog("png", NULL, &outPath);


   if (result == NFD_OKAY) {
      puts("Success!");
      puts(outPath);

      //get and set file name 
      name = GetFileName(outPath);

      return outPath;
      free(outPath);
   }
   else if (result == NFD_CANCEL) {
      puts("User pressed cancel.");
   }
   else {
      printf("Error: %s\n", NFD_GetError());
   }

   return "";
}

std::string FileHandler::GetFileName(std::string path)
{
   

   std::string delimiter = "\\";

   size_t pos = 0;

   std::string token;

   while ((pos = path.find(delimiter)) != std::string::npos) {

      token = path.substr(0, pos);

      path.erase(0, pos + delimiter.length());

   }

   return path;
}

FileHandler::FileHandler() {
   
}

