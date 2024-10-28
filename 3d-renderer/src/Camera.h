#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  5.0f;
const float SENSITIVITY =  0.025f;
const float ZOOM        =  45.0f;

class Camera {
public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
         float yaw = YAW, 
         float pitch = PITCH);

  Camera(float posX, float posY, float posZ, 
         float upX, float upY, float upZ, 
         float yaw, float pitch);

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix();

  // Processes input received from any keyboard-like input system
  void ProcessKeyboard(float xoffset, float yoffset, float deltaTime);

  // Processes input received from a mouse input system
  void ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);

  // Processes input received from a mouse scroll-wheel event
  void ProcessMouseScroll(float yoffset);

private:
  // calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();

};
