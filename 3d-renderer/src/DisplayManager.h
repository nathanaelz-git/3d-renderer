#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class DisplayManager {
  private:
    static float lastX;
    static float lastY;
    static bool rotateMode;
    static bool firstMouse;
    static GLFWwindow* m_WINDOW;

  public:
    static int m_SCR_WIDTH;
    static int m_SCR_HEIGHT;

    static glm::mat4 rotateMatrix;
    
    static void createDisplay();

    static void updateDisplay();

    static void closeDisplay();

    static GLFWwindow* getWindow();

    static void processInput();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
};

