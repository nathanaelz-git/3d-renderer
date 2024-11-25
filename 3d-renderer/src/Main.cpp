#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "DisplayManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

// Timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float settingTime = 0.0f;


// light position
glm::vec3 lightPos(1.2f, 3.25f, 2.0f);

int main(void)
{
   DisplayManager::createDisplay();

   Model backpack("./models/backpack/backpack.obj");

   Shader ourShader("src/Shaders/default.vert", "src/Shaders/lighting.frag");
   ourShader.use();

   bool drawTriangle = true;

   float size = 1.0f;

   //lighting variables
   float lightConstant = 1.0f;
   float lightLinear = 0.09f;
   float lightQuadratic = 0.032f;

   float ambiant[3] = {0.15f, 0.15f, 0.15f};
   float diffuse[3] = {1.0f,  1.0f,  1.0f};
   float specular[3] = {1.0f,  1.0f,  1.0f};
   float Intensity[3] = { 1.0f,  1.0f,  1.0f };

   ourShader.setFloat("size", size);

  glEnable(GL_DEPTH_TEST);  
  while (!glfwWindowShouldClose(DisplayManager::getWindow()))
  {
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

    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    model *= DisplayManager::rotateMatrix;

    view = camera.GetViewMatrix();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float) DisplayManager::m_SCR_WIDTH / (float) DisplayManager::m_SCR_HEIGHT, 0.1f, 100.0f);

    ourShader.setMat4("model", model);
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("viewPos", camera.Position); 
    ourShader.setVec3("pointLight.position", lightPos);
    ourShader.setVec3("pointLight.ambient", ambiant[0], ambiant[1], ambiant[2]);
    ourShader.setVec3("pointLight.diffuse", diffuse[0], diffuse[1], diffuse[2]);
    ourShader.setVec3("pointLight.specular", specular[0], specular[1], specular[2]);
    ourShader.setFloat("material.shininess", 32.0f);
    ourShader.setFloat("pointLight.constant", lightConstant);
    ourShader.setFloat("pointLight.linear", lightLinear);
    ourShader.setFloat("pointLight.quadratic", lightQuadratic);
    ourShader.setFloat("pointLight.aIntensity", Intensity[0]);
    ourShader.setFloat("pointLight.dIntensity", Intensity[1]);
    ourShader.setFloat("pointLight.sIntensity", Intensity[2]);

    if (drawTriangle) backpack.Draw(ourShader);

    //GUI Start
    ImGui::Begin("Settings");
    // ImGui::Text("Some text here...");
    if (ImGui::CollapsingHeader("View")) {
       ImGui::Checkbox("Draw", &drawTriangle);
       ImGui::SliderFloat("Resize", &size, 0.5f, 2.0f);
    }
    if (ImGui::CollapsingHeader("Lighting")) {

       ImGui::Text("Brightness");
       ImGui::SliderFloat("Ambiant", &Intensity[0], 0.0f, 100.0f);
       ImGui::SliderFloat("Diffuse", &Intensity[1], 0.0f, 100.0f);
       ImGui::SliderFloat("Specular", &Intensity[2], 0.0f, 100.0f);


       ImGui::Text("Color");
       ImGui::ColorEdit3("Ambiant", ambiant);
       ImGui::ColorEdit3("Diffuse", diffuse);
       ImGui::ColorEdit3("Specular", specular);

       //removed since it doesnt seem to do much 
       /*ImGui::Text("math");
       ImGui::SliderFloat("Light Constant", &lightConstant, 0.0f, 2.0f);
       ImGui::SliderFloat("Linear", &lightLinear, 0.0f, 1.0f);
       ImGui::SliderFloat("Quadratic", &lightQuadratic, 0.0f, 0.1f);*/
    }
     
    ImGui::End();

    //updating Shader
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    DisplayManager::updateDisplay();
  }
  DisplayManager::closeDisplay();
  return 0;
}

void DisplayManager::processInput()
{
  if(glfwGetKey(m_WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(m_WINDOW, true);

  // Rotate the object around
  if (glfwGetMouseButton(m_WINDOW, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
  {
    rotateMode = true;
  } 
  else
  {
    rotateMode = false;
    firstMouse = true;
  }
}


