#include "Camera.h"

#include <iostream>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, 
               float yaw, float pitch)
  : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM) {

  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ,
               float upX, float upY, float upZ,
               float yaw, float pitch)
  : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM) {

  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

// Processes input received from any keyboard-like input system
void Camera::ProcessKeyboard(float xoffset, float yoffset, float deltaTime) {
  float velocity = MouseSensitivity * deltaTime * 5;
    Position -= (std::cos(Pitch) * Front) * velocity;
    Position += (std::sin(Yaw) * Right) * velocity;
  std::cout << "X:"  << Position.x << " Y:" << Position.y << " Z:" << Position.z << std::endl;
}

// Processes input received from a mouse input system
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constraintPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw   += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constraintPitch) {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  // Update Front, Right and Up Vectors using the updated Euler angles
  std::cout << "Pitch: "  << Pitch << std::endl;
  std::cout << "Yaw: "  << Yaw << std::endl;
  updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event
void Camera::ProcessMouseScroll(float yoffset) {
  Zoom -= yoffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp)); 
  Up    = glm::normalize(glm::cross(Right, Front));
}