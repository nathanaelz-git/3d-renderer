#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "DisplayManager.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ImportHandler.h"

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


  //setting model to backpack by default
  Model backpack("./models/backpack/backpack.obj");

  Shader ourShader("src/Shaders/default.vert", "src/Shaders/lighting.frag");
  ourShader.use();

  ImportHandler iHandler;

  bool drawTriangle = true;

  float size = 1.0f;
  ourShader.setFloat("size", size);

  glEnable(GL_DEPTH_TEST);  

  //main while loop
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
    ourShader.setVec3("pointLight.ambient", 0.15f, 0.15f, 0.15f);
    ourShader.setVec3("pointLight.diffuse", 1.0f, 1.0f, 1.0f);
    ourShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
    ourShader.setFloat("material.shininess", 32.0f);
    ourShader.setFloat("pointLight.constant", 1.0f);
    ourShader.setFloat("pointLight.linear", 0.09f);
    ourShader.setFloat("pointLight.quadratic", 0.032f);

    if (drawTriangle) backpack.Draw(ourShader);

    ImGui::Begin("Settings");
    // ImGui::Text("Some text here...");
    ImGui::Checkbox("Draw", &drawTriangle);
    ImGui::SliderFloat("Resize", &size, 0.5f, 2.0f);

    if (ImGui::Button("Import"))
    {
       iHandler.GetObjectFile();

    };
    
    ImGui::End();

    ourShader.setFloat("size", size);

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


