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

std::string FileHandler::GetTextureFile()
{
   std::cout << "Getting the Texture File" << std::endl;

   nfdchar_t* outPath = NULL;

   nfdresult_t result = NFD_OpenDialog("png,jpg", NULL, &outPath);


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

FileHandler::FileHandler() {
   
}

