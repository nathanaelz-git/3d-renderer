#include "DisplayManager.h"

#include <iostream>

int DisplayManager::m_SCR_WIDTH;
int DisplayManager::m_SCR_HEIGHT;
GLFWwindow* DisplayManager::m_WINDOW;
float DisplayManager::lastX;
float DisplayManager::lastY;
bool DisplayManager::rotateMode = false;
bool DisplayManager::firstMouse = true;

// Model rotate
glm::mat4 DisplayManager::rotateMatrix = glm::mat4(1.0f);
float Yaw = 0.0f;
float Pitch = 0.0f;

void DisplayManager::createDisplay() {
  glfwInit();
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWmonitor* MyMonitor =  glfwGetPrimaryMonitor(); 
  const GLFWvidmode* mode = glfwGetVideoMode(MyMonitor);
  m_SCR_WIDTH = mode->width / 1.25;
  m_SCR_HEIGHT = mode->height / 1.25;
  lastX = m_SCR_WIDTH / 2.0f;
  lastY = m_SCR_HEIGHT / 2.0f;

  m_WINDOW = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "Renderer", NULL, NULL);
  // Full screen
  // m_WINDOW = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "Renderer", MyMonitor, NULL);
  if (m_WINDOW == NULL) {
    std::cout << ("Failed to create GLFW window\n") << '\n';
    closeDisplay();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(m_WINDOW);
  glfwSetFramebufferSizeCallback(m_WINDOW, framebuffer_size_callback); 
  glfwSetCursorPosCallback(m_WINDOW, mouse_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << ("Failed to initialize GLAD\n") << '\n';
    exit(EXIT_FAILURE);
  }
  glViewport(0, 0, m_SCR_WIDTH, m_SCR_HEIGHT);

  glEnable(GL_DEPTH_TEST);  

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(DisplayManager::getWindow(), true);
  ImGui_ImplOpenGL3_Init("#version 460");
  io.FontGlobalScale = 1.85f;  
}

void DisplayManager::updateDisplay() {
  glfwSwapBuffers(m_WINDOW);
  glfwPollEvents();
}

void DisplayManager::closeDisplay() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(m_WINDOW);
  glfwTerminate();
}

GLFWwindow* DisplayManager::getWindow() {
  return m_WINDOW;
}

void DisplayManager::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
void DisplayManager::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
  // Forward mouse input to ImGui
  if (rotateMode)
  {
    float xPos = (float) xposIn;
    float yPos = (float) yposIn;

    if (firstMouse) 
    {
      lastX = xPos;
      lastY = yPos;
      firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top 

    lastX = xPos;
    lastY = yPos;

    Yaw += xoffset * 0.1f;
    Pitch -= yoffset * 0.1f;
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
    rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateMatrix = glm::rotate(rotateMatrix, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
  }
}
