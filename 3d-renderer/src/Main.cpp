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

// Rotation variables
glm::vec3 objectRotation(0.0f, 0.0f, 0.0f); // Object rotation angles (degrees)
bool rotateMode = false;                    // Indicates if rotation mode is active
bool firstMouse = true;                     // Helper for mouse handling
double lastX = 0.0, lastY = 0.0;            // Last mouse cursor position

// light position
glm::vec3 lightPos(1.2f, 3.25f, 2.0f);

int main(void)
{
   DisplayManager::createDisplay();

   Model currentModel("./models/backpack/backpack.obj");

   Shader ourShader("src/Shaders/default.vert", "src/Shaders/lighting.frag");
   ourShader.use();

  ImportHandler iHandler;

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
    // Apply rotation based on objectRotation
    model = glm::rotate(model, glm::radians(objectRotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
    model = glm::rotate(model, glm::radians(objectRotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
    model = glm::rotate(model, glm::radians(objectRotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis
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

    if (drawTriangle) currentModel.Draw(ourShader);

    //GUI Start
    ImGui::Begin("Settings");
    // ImGui::Text("Some text here...");
    if (ImGui::CollapsingHeader("View")) {
       ImGui::Checkbox("Draw", &drawTriangle);
       ImGui::SliderFloat("Resize", &size, 0.5f, 2.0f);
    }
    if (ImGui::CollapsingHeader("Object Transformation"))
    {
        ImGui::Text("Rotation (W/S: X, A/D: Y, Q/E: Z)");
        ImGui::Text("X: %.2f", objectRotation.x);
        ImGui::Text("Y: %.2f", objectRotation.y);
        ImGui::Text("Z: %.2f", objectRotation.z);
    }
    if (ImGui::CollapsingHeader("Lighting")) {

       ImGui::Text("Position");
       ImGui::SliderFloat("X", &lightPos.x, -10.0f, 10.0f);
       ImGui::SliderFloat("Y", &lightPos.y, -10.0f, 10.0f);
       ImGui::SliderFloat("Z", &lightPos.z, -10.0f, 10.0f);

       ImGui::Text("Brightness");
       ImGui::SliderFloat("Ambiant", &Intensity[0], 0.0f, 10.0f);
       ImGui::SliderFloat("Diffuse", &Intensity[1], 0.0f, 10.0f);
       ImGui::SliderFloat("Specular", &Intensity[2], 0.0f, 10.0f);


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

    if (ImGui::Button("Import Object"))
    {
       std::string newObjPath = iHandler.GetObjectFile();

       if (!newObjPath.empty()) {
          currentModel = Model(newObjPath);
       }

    };
    
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

    ourShader.setVec3("pointLight.position", lightPos);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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



