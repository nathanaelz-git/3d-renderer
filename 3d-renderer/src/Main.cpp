#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "DisplayManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "FileHandler.h"

#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>  
#include <sstream> 
#include <vector>

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// Timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float settingTime = 0.0f;

// Rotation variables
glm::vec3 objectRotation(0.0f, 0.0f, 0.0f); // Object rotation angles (degrees)
bool rotateMode = false;                    // Indicates if rotation mode is active
bool firstMouse = true;                     // Helper for mouse handling
double lastX = 0.0, lastY = 0.0;            // Last mouse cursor position

// light positions
glm::vec3 lightPos(1.2f, 3.25f, 2.0f);
glm::vec3 lightPos2(1.2f, 3.25f, 2.0f);

// Blinn-Phong shading
bool blinnPhong = true;

//Object color
glm::vec3 objColor(1.0f, 1.0f, 1.0f);

//Monochrome toggle
bool monochrome = false;

/* SCREEN SHOT */
bool screenshotRequested = false;
std::chrono::steady_clock::time_point lastScreenshotTime = std::chrono::steady_clock::now();
const double screenshotCooldown = 1.0; 

/* SKYBOX */
std::vector<std::string> faces1 {
  "skyboxes/skybox2/right.jpg",
  "skyboxes/skybox2/left.jpg",
  "skyboxes/skybox2/top.jpg",
  "skyboxes/skybox2/bottom.jpg",
  "skyboxes/skybox2/front.jpg",
  "skyboxes/skybox2/back.jpg"
};

std::vector<std::string> faces2 {
  "skyboxes/skybox1/right.jpg",
  "skyboxes/skybox1/left.jpg",
  "skyboxes/skybox1/top.jpg",
  "skyboxes/skybox1/bottom.jpg",
  "skyboxes/skybox1/front.jpg",
  "skyboxes/skybox1/back.jpg"
};

std::vector<std::string> faces3 {
  "skyboxes/skybox3/right.png",
  "skyboxes/skybox3/left.png",
  "skyboxes/skybox3/top.png",
  "skyboxes/skybox3/bottom.png",
  "skyboxes/skybox3/front.png",
  "skyboxes/skybox3/back.png"
};
std::vector<std::string> faces = faces1;

// Controls whether the skybox is enabled
bool skyboxEnabled = false;
bool prevSkyboxEnabled = skyboxEnabled;

// Skybox selection index
int currentSkyboxIndex = 0;

// Flag to indicate that a new skybox needs to be loaded
bool newSkybox = false;

// Cubemap texture ID
unsigned int cubemapTexture = 0;

unsigned int loadCubemap(std::vector<std::string> faces);
std::vector<glm::vec3> skyboxVertices = {
  { -1.0f, -1.0f,  1.0f },
  {  1.0f, -1.0f,  1.0f },
  {  1.0f, -1.0f, -1.0f },
  { -1.0f, -1.0f, -1.0f },
  { -1.0f,  1.0f,  1.0f },
  {  1.0f,  1.0f,  1.0f },
  {  1.0f,  1.0f, -1.0f },
  { -1.0f,  1.0f, -1.0f }
};

std::vector<unsigned int> skyboxIndices = {
  // Right
  1, 2, 6,
  6, 5, 1,
  // Left
  0, 4, 7,
  7, 3, 0,
  // Top
  4, 5, 6,
  6, 7, 4,
  // Bottom
  0, 3, 2,
  2, 1, 0,
  // Back
  0, 1, 5,
  5, 4, 0,
  // Front
  3, 7, 6,
  6, 2, 3
};

int main(void)
{
  DisplayManager::createDisplay();

  Model currentModel("./models/backpack/backpack.obj");

  Shader ourShader("src/Shaders/default.vert", "src/Shaders/lighting.frag");
  Shader outliningShader("src/Shaders/outlining.vert", "src/Shaders/outlining.frag");
  Shader skyboxShader("src/Shaders/skybox.vert", "src/Shaders/skybox.frag");

  FileHandler fileHandler;

  bool drawTriangle = true;
  bool drawOutline = false;

  float size = 1.0f;

  //lighting variables for base light
  float lightConstant = 1.0f;
  float lightLinear = 0.09f;
  float lightQuadratic = 0.032f;

  float ambiant[3] = {0.15f, 0.15f, 0.15f};
  float diffuse[3] = {1.0f,  1.0f,  1.0f};
  float specular[3] = {1.0f,  1.0f,  1.0f};
  float Intensity[3] = { 1.0f,  1.0f,  1.0f };

  //lighting variables for aux light
  bool auxLightEnabled = false;

  float ambiant2[3] = { 0.15f, 0.15f, 0.15f };
  float diffuse2[3] = { 1.0f,  1.0f,  1.0f };
  float specular2[3] = { 1.0f,  1.0f,  1.0f };
  float Intensity2[3] = { 1.0f,  1.0f,  1.0f };

  float shininess = 32.0f;

  ourShader.setFloat("size", size);

  // Enable the depth buffer
  glEnable(GL_DEPTH_TEST);  


  glEnable(GL_STENCIL_TEST);
  // Enable cull facing, improve performance
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);

  // FPS display variables
  double previousTime = glfwGetTime();
  int frameCount = 0;
  float fps = 0.0f;

  // Default v-sync setting 
  bool vSyncEnabled = true;

  bool antiAliasEnabled = true;

  
  VertexArray skyboxVAO;
  skyboxVAO.bind();

  VertexBuffer skyboxVBO(skyboxVertices);

  skyboxVAO.linkAttrib(skyboxVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
  ElementBuffer skyboxEBO(skyboxIndices);

  skyboxVAO.unbind();
  skyboxVBO.unbind();
  // skyboxEBO.unbind();

  cubemapTexture = loadCubemap(faces);

  //main while loop
  while (!glfwWindowShouldClose(DisplayManager::getWindow()))
  {
    if (newSkybox && skyboxEnabled) {
      // Delete the previous cubemap texture
      glDeleteTextures(1, &cubemapTexture);

      // Load the new cubemap texture
      cubemapTexture = loadCubemap(faces);

      newSkybox = false;
    }
    ourShader.use();
    // enable vsync
    if (vSyncEnabled)
      glfwSwapInterval(1);
    else glfwSwapInterval(0);

    // enable anti aliasing
    if (antiAliasEnabled)
      glEnable(GL_MULTISAMPLE);
    else glDisable(GL_MULTISAMPLE);

    double currentTime = glfwGetTime();
    frameCount++;

    // Update FPS every second
    if (currentTime - previousTime >= 1.0) {
      fps = frameCount / (currentTime - previousTime);

      // Reset for next calculation
      previousTime = currentTime;
      frameCount = 0;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::mat4 view = glm::mat4(1.0f);
    // per-frame time logic
    float currentFrame = (float) glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    DisplayManager::processInput();
    /* Render */ 
    // Bind the custom framebuffer
    glViewport(0, 0, DisplayManager::m_SCR_WIDTH, DisplayManager::m_SCR_HEIGHT);
    // Specifying the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Clean the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    // Apply rotation based on objectRotation
    model = glm::scale(model, glm::vec3(size));
    model = glm::rotate(model, glm::radians(objectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
    model = glm::rotate(model, glm::radians(objectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
    model = glm::rotate(model, glm::radians(objectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis
    model *= DisplayManager::rotateMatrix;

    view = camera.GetViewMatrix();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float) DisplayManager::m_SCR_WIDTH / (float) DisplayManager::m_SCR_HEIGHT, 0.1f, 100.0f);

    //setting starting variables for the shader we will be using
    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("viewPos", camera.Position); 

    ourShader.setVec3("pointLight.position", lightPos);
    ourShader.setVec3("pointLight.ambient", ambiant[0], ambiant[1], ambiant[2]);
    ourShader.setVec3("pointLight.diffuse", diffuse[0], diffuse[1], diffuse[2]);
    ourShader.setVec3("pointLight.specular", specular[0], specular[1], specular[2]);
    ourShader.setFloat("pointLight.aIntensity", Intensity[0]);
    ourShader.setFloat("pointLight.dIntensity", Intensity[1]);
    ourShader.setFloat("pointLight.sIntensity", Intensity[2]);

    ourShader.setVec3("pointLight2.position", lightPos2);
    ourShader.setVec3("pointLight2.ambient", ambiant2[0], ambiant2[1], ambiant2[2]);
    ourShader.setVec3("pointLight2.diffuse", diffuse2[0], diffuse2[1], diffuse2[2]);
    ourShader.setVec3("pointLight2.specular", specular2[0], specular2[1], specular2[2]);
    ourShader.setFloat("pointLight2.aIntensity", Intensity2[0]);
    ourShader.setFloat("pointLight2.dIntensity", Intensity2[1]);
    ourShader.setFloat("pointLight2.sIntensity", Intensity2[2]);

    ourShader.setFloat("material.shininess", shininess);

    ourShader.setFloat("pointLight.constant", lightConstant);
    ourShader.setFloat("pointLight.linear", lightLinear);
    ourShader.setFloat("pointLight.quadratic", lightQuadratic);

    ourShader.setFloat("pointLight2.constant", lightConstant);
    ourShader.setFloat("pointLight2.linear", lightLinear);
    ourShader.setFloat("pointLight2.quadratic", lightQuadratic);
    
    ourShader.setBool("monochrome", monochrome);
    ourShader.setBool("auxLightOn", auxLightEnabled);
    ourShader.setInt("blinnPhong", blinnPhong);

    if (drawTriangle) {
      if (drawOutline) {
        // 1st pass: draw object normally
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        currentModel.Draw(ourShader);

        // 2nd pass: draw outline
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        outliningShader.use();
        outliningShader.setMat4("model", model);
        outliningShader.setMat4("view", view);
        outliningShader.setMat4("projection", projection);

        float baseOutlineSize = 0.08f;
        float adjustedOutlineSize = baseOutlineSize / size;
        outliningShader.setFloat("size", adjustedOutlineSize);

        currentModel.Draw(outliningShader);

        // Restore default settings
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
      }
      else {
        currentModel.Draw(ourShader);
      }
    }

    // Disable befpre drawing the skybox
    glDisable(GL_CULL_FACE);
    if (skyboxEnabled) {
      // Skybox display
      glDepthFunc(GL_LEQUAL);
      skyboxShader.use();

      glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
      skyboxShader.setMat4("view", skyboxView);
      skyboxShader.setMat4("projection", projection);

      skyboxVAO.bind();
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(skyboxIndices.size()), GL_UNSIGNED_INT, 0);
      skyboxVAO.unbind();
      glDepthFunc(GL_LESS);
    }
    glEnable(GL_CULL_FACE);

    ourShader.use();
    // Begin Main Menu Bar
    // Replace this block with the menu bar
    if (ImGui::BeginMainMenuBar()) { // Creates the top bar
        // "View" menu
        if (ImGui::BeginMenu("View")) {
            ImGui::Checkbox("Draw", &drawTriangle);
            ImGui::Checkbox("VSync", &vSyncEnabled);
            ImGui::Checkbox("Anti Aliasing", &antiAliasEnabled);
            // Checkbox to enable/disable the skybox
            ImGui::Checkbox("Enable Skybox", &skyboxEnabled);
            // If the skybox is enabled, show the combo box
            if (skyboxEnabled) {
              const char *skyboxNames[] = { "Skybox 1", "Skybox 2", "Skybox 3" };
              // Create a combo box for skybox selection
              if (ImGui::Combo("Select Skybox", &currentSkyboxIndex, skyboxNames, IM_ARRAYSIZE(skyboxNames))) {
                // Update the 'faces' vector based on the selection
                switch (currentSkyboxIndex) {
                  case 0:
                    faces = faces1;
                    break;
                  case 1:
                    faces = faces2;
                    break;
                  case 2:
                    faces = faces3;
                    break;
                }
                newSkybox = true; // Set the flag to load the new skybox
              }
            }
            ImGui::SliderFloat("Resize", &size, 0.001f, 5.0f);
            ImGui::Text("FPS: %.1f", fps);
            ImGui::EndMenu();
        }

        // "Object" menu
        if (ImGui::BeginMenu("Object")) {
            ImGui::Text("Rotation (W/S: X, A/D: Y, Q/E: Z)");
            ImGui::Text("X: %.2f", objectRotation.x);
            ImGui::Text("Y: %.2f", objectRotation.y);
            ImGui::Text("Z: %.2f", objectRotation.z);

            ImGui::Text("Material");
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 100.0f);
            ImGui::ColorEdit3("Object Color", glm::value_ptr(objColor));
            ImGui::EndMenu();
        }

        // "Lighting" menu
        if (ImGui::BeginMenu("Lighting")) {
           if (ImGui::CollapsingHeader("Base Light"))
           {
              ImGui::Checkbox("Blinn-Phong", &blinnPhong);
              ImGui::Text("Position");
              ImGui::SliderFloat("X", &lightPos.x, -10.0f, 10.0f);
              ImGui::SliderFloat("Y", &lightPos.y, -10.0f, 10.0f);
              ImGui::SliderFloat("Z", &lightPos.z, -10.0f, 10.0f);

              ImGui::Text("Brightness");
              ImGui::SliderFloat("Ambient", &Intensity[0], 0.0f, 10.0f);
              ImGui::SliderFloat("Diffuse", &Intensity[1], 0.0f, 10.0f);
              ImGui::SliderFloat("Specular", &Intensity[2], 0.0f, 10.0f);

              ImGui::Text("Color");
              ImGui::ColorEdit3("Ambient", ambiant);
              ImGui::ColorEdit3("Diffuse", diffuse);
              ImGui::ColorEdit3("Specular", specular);
           }

            if (ImGui::CollapsingHeader("Aux Light" ))
            {
               ImGui::Checkbox("Enabled", &auxLightEnabled);

               ImGui::Text("Aux Position");
               ImGui::SliderFloat("Aux X", &lightPos2.x, -10.0f, 10.0f);
               ImGui::SliderFloat("Aux Y", &lightPos2.y, -10.0f, 10.0f);
               ImGui::SliderFloat("Aux Z", &lightPos2.z, -10.0f, 10.0f);

               ImGui::Text("Aux Brightness");
               ImGui::SliderFloat("Aux Ambient", &Intensity2[0], 0.0f, 10.0f);
               ImGui::SliderFloat("Aux Diffuse", &Intensity2[1], 0.0f, 10.0f);
               ImGui::SliderFloat("Aux Specular", &Intensity2[2], 0.0f, 10.0f);

               ImGui::Text("Aux Color");
               ImGui::ColorEdit3("Aux Ambient", ambiant2);
               ImGui::ColorEdit3("Aux Diffuse", diffuse2);
               ImGui::ColorEdit3("Aux Specular", specular2);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Filters")) {
           
           ImGui::Checkbox("Monochrome Toggle", &monochrome);
           ImGui::Checkbox("Outline", &drawOutline);

           ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Textures")) {

           ImGui::Text("Loaded Textures");
           //List of currently applied Textures
           if (ImGui::BeginChild("Loaded", ImVec2(ImGui::CalcItemWidth(), ImGui::GetTextLineHeight() * 7.5f), ImGuiChildFlags_FrameStyle))
           {

              for (Texture item : currentModel.textures) {

                 std::string itemName = item.name;

                 if (itemName == "") {

                    continue;
                 }

                 if (ImGui::Selectable(itemName.c_str(), &item.selected)) {

                    for (Texture t : currentModel.textures_loaded) {
                       currentModel.UnLoadTexture(t);
                    }

                    Texture texture(item.filename, item.name, false);
                    currentModel.textures_loaded.push_back(texture);
                 }
              }
           }
           ImGui::EndChild();
           ImGui::AlignTextToFramePadding();


           if (ImGui::Button("Import Texture"))
           {
     
              std::string fileName;
              std::string newObjPath = fileHandler.GetTextureFile(fileName);

              if (!newObjPath.empty()) {

                 for (Texture t : currentModel.textures_loaded) {
                    currentModel.UnLoadTexture(t);
                 }

                 Texture texture(newObjPath, fileName, false);
                 currentModel.textures_loaded.push_back(texture);
                 currentModel.textures.push_back(texture);
                 
              }
           };
           ImGui::EndMenu();
        }

        // "File Controls" menu
        if (ImGui::BeginMenu("File Controls")) {
            if (ImGui::Button("Import Object")) {
                std::string newObjPath = fileHandler.GetObjectFile();
                if (!newObjPath.empty()) {

                   std::vector<Texture> tempTextures = currentModel.textures;
                   std::vector<Texture> tempTexturesLoaded = currentModel.textures_loaded;

                   currentModel = Model(newObjPath);

                   currentModel.textures = tempTextures;
                   currentModel.textures_loaded = tempTexturesLoaded;
                }
            }
            if (ImGui::MenuItem("Reset Scene")) {
               // Reset all parameters to their defaults
               objectRotation = glm::vec3(0.0f, 0.0f, 0.0f);
               objColor = glm::vec3(1.0f, 1.0f, 1.0f);
               lightPos = glm::vec3(1.2f, 3.25f, 2.0f);
               size = 1.0f;
               
               ambiant[0] = 0.15f; ambiant[1] = 0.15f; ambiant[2] = 0.15f;
               diffuse[0] = 1.0f; diffuse[1] = 1.0f; diffuse[2] = 1.0f;
               specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f;
               Intensity[0] = 1.0f; Intensity[1] = 1.0f; Intensity[2] = 1.0f;
               shininess = 32.0f;

               monochrome = false;
               drawOutline = false;
               vSyncEnabled = true;

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar(); // End the top menu bar
    }

    // Update Object 
    ourShader.setFloat("material.shininess", shininess);

    // Update Object Color
    ourShader.setVec3("objColor", objColor);

    // Updating Shader
    ourShader.setFloat("size", size);
    ourShader.setVec3("pointLight.ambient",   (ambiant[0] / 255),
                                              (ambiant[1] / 255),
                                              (ambiant[2] / 255));

    ourShader.setVec3("pointLight.diffuse",   (diffuse[0] / 255),
                                              (diffuse[1] / 255),
                                              (diffuse[2] / 255));

    ourShader.setVec3("pointLight.specular",  (specular[0] / 255),
                                              (specular[1] / 255),
                                              (specular[2] / 255));

    ourShader.setFloat("pointLight.constant", lightConstant);
    ourShader.setFloat("pointLight.linear", lightLinear);
    ourShader.setFloat("pointLight.quadratic", lightQuadratic);

    ourShader.setFloat("pointLight.aIntensity", Intensity[0]);
    ourShader.setFloat("pointLight.dIntensity", Intensity[1]);
    ourShader.setFloat("pointLight.sIntensity", Intensity[2]);

    ourShader.setVec3("pointLight.position", lightPos);

    //determine if aux light is enabled and update value if true

    ourShader.setBool("auxLightOn", auxLightEnabled);

    ourShader.setVec3("pointLight2.ambient", (ambiant2[0] / 255),
       (ambiant2[1] / 255),
       (ambiant2[2] / 255));

    ourShader.setVec3("pointLight2.diffuse", (diffuse2[0] / 255),
       (diffuse2[1] / 255),
       (diffuse2[2] / 255));

    ourShader.setVec3("pointLight2.specular", (specular2[0] / 255),
       (specular2[1] / 255),
       (specular2[2] / 255));

    ourShader.setFloat("pointLight2.aIntensity", Intensity2[0]);
    ourShader.setFloat("pointLight2.dIntensity", Intensity2[1]);
    ourShader.setFloat("pointLight2.sIntensity", Intensity2[2]);
       
    ourShader.setVec3("pointLight2.position", lightPos2);
   

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (screenshotRequested) {
      screenshotRequested = false;

      // Read pixels
      int width = DisplayManager::m_SCR_WIDTH;
      int height = DisplayManager::m_SCR_HEIGHT;

      unsigned char* pixels = new unsigned char[width * height * 3]; // For RGB

      glPixelStorei(GL_PACK_ALIGNMENT, 1); // Ensure alignment is 1
      glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

      // Flip the image vertically
      int rowSize = width * 3;
      unsigned char* row = new unsigned char[rowSize];
      for (int y = 0; y < height / 2; y++) {
        unsigned char* row1 = pixels + y * rowSize;
        unsigned char* row2 = pixels + (height - 1 - y) * rowSize;
        memcpy(row, row1, rowSize);
        memcpy(row1, row2, rowSize);
        memcpy(row2, row, rowSize);
      }
      delete[] row;

      // Generate filename with timestamp
      auto now = std::chrono::system_clock::now();
      std::time_t now_c = std::chrono::system_clock::to_time_t(now);
      struct tm timeinfo;

      localtime_s(&timeinfo, &now_c);
      std::ostringstream oss;
      oss << "screenshot_"
          << std::put_time(&timeinfo, "%Y%m%d_%H%M%S")
          << ".png";
      std::string filename = oss.str();

      // Use stb_image_write to save image
      stbi_write_png(filename.c_str(), width, height, 3, pixels, width * 3);

      delete[] pixels;

      // Confirmation message
      std::cout << "Screenshot saved to " << filename << std::endl;
    }

    DisplayManager::updateDisplay();
  }
  DisplayManager::closeDisplay();
  return 0;
}

void DisplayManager::processInput()
{
    if (glfwGetKey(m_WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_WINDOW, true);

    if (glfwGetKey(m_WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_WINDOW, true);

    // Rotate the object
    float rotationSpeed = 50.0f * deltaTime; // Adjust rotation speed as needed

    if (glfwGetKey(m_WINDOW, GLFW_KEY_W) == GLFW_PRESS)
        objectRotation.x += rotationSpeed; // Rotate up (around X-axis)
    if (glfwGetKey(m_WINDOW, GLFW_KEY_S) == GLFW_PRESS)
        objectRotation.x -= rotationSpeed; // Rotate down (around X-axis)
    if (glfwGetKey(m_WINDOW, GLFW_KEY_A) == GLFW_PRESS)
        objectRotation.y -= rotationSpeed; // Rotate left (around Y-axis)
    if (glfwGetKey(m_WINDOW, GLFW_KEY_D) == GLFW_PRESS)
        objectRotation.y += rotationSpeed; // Rotate right (around Y-axis)
    if (glfwGetKey(m_WINDOW, GLFW_KEY_Q) == GLFW_PRESS)
        objectRotation.z -= rotationSpeed; // Rotate counterclockwise (around Z-axis)
    if (glfwGetKey(m_WINDOW, GLFW_KEY_E) == GLFW_PRESS)
        objectRotation.z += rotationSpeed; // Rotate clockwise (around Z-axis)

    // Handle screenshot input
    static bool fKeyPreviouslyPressed = false;
    bool fKeyCurrentlyPressed = glfwGetKey(m_WINDOW, GLFW_KEY_F) == GLFW_PRESS;

    if (fKeyCurrentlyPressed && !fKeyPreviouslyPressed) {
      auto now = std::chrono::steady_clock::now();
      double secondsSinceLastScreenshot = std::chrono::duration<double>(now - lastScreenshotTime).count();

      if (secondsSinceLastScreenshot >= screenshotCooldown) {
        screenshotRequested = true;
        lastScreenshotTime = now;
      }
    }
    fKeyPreviouslyPressed = fKeyCurrentlyPressed;

    // Rotate the object using mouse movement
    if (glfwGetMouseButton(m_WINDOW, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
    {
        double xpos, ypos;
        glfwGetCursorPos(m_WINDOW, &xpos, &ypos);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        double xOffset = xpos - lastX;
        double yOffset = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        objectRotation.y += (float)xOffset * 0.1f; // Adjust sensitivity as needed
        objectRotation.x -= (float)yOffset * 0.1f;
    }
    else
    {
        firstMouse = true;
    }
}

unsigned int loadCubemap(std::vector<std::string> faces) 
{
  stbi_set_flip_vertically_on_load(false); // Disable vertical flipping
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++)
  {
      unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
      if (data)
      {
        unsigned int format = GL_RGB;
        if (nrChannels == 1)
          format = GL_RED;
        else if (nrChannels == 3)
          format = GL_RGB;
        else if (nrChannels == 4)
          format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
      }
      else
      {
        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
        stbi_image_free(data);
      }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}



