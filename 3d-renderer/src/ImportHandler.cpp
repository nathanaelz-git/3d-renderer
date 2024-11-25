#include "ImportHandler.h"
#include <nfd/include/nfd.h>

void ImportHandler::GetObjectFile() {
   std::cout << "Getting the Object File" << std::endl;

   nfdchar_t* outPath = NULL;
   nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

   /*if (result == NFD_OKAY) {
      puts("Success!");
      puts(outPath);
      free(outPath);
   }
   else if (result == NFD_CANCEL) {
      puts("User pressed cancel.");
   }
   else {
      printf("Error: %s\n", NFD_GetError());
   }*/

   return;
}

ImportHandler::ImportHandler() {
   
}

