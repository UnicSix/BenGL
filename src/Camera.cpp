#include "Camera.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <iostream>
#include <cmath>

Camera::Camera(){}
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw,
      GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed){
  
  position = startPosition;
  worldUp = startUp;
  yaw = startYaw;
  pitch = startPitch;
  moveSpeed = startMoveSpeed;
  turnSpeed = startTurnSpeed;
  
  front = glm::vec3(0.0f, 0.0f, -1.0f);
  
  Update();
}
void Camera::keyControl(bool* keys, GLfloat deltaTime){
  GLfloat velocity = moveSpeed * deltaTime;
  
  if (keys[GLFW_KEY_W])
    position += front * velocity;
  if (keys[GLFW_KEY_S])
    position -= front * velocity;
  if (keys[GLFW_KEY_A])
    position -= right * velocity;
  if (keys[GLFW_KEY_D])
    position += right * velocity;
  if(keys[GLFW_KEY_SPACE])
    position += up * velocity;
  if(keys[GLFW_KEY_LEFT_ALT])
    position -= up * velocity;
    
}
void Camera::mouseControl(GLfloat xChange, GLfloat yChange){
  xChange *= turnSpeed;
  yChange *= turnSpeed;

  yaw += xChange;
  pitch += yChange;

  if(pitch > 89.0f){
    pitch = 89.0f;
  }
  if(pitch < -89.0f){
    pitch = -89.0f;
  }
  Update();
}
glm::mat4 Camera::calculateViewMatrix(){
  return glm::lookAt(position, position+front, up);
}
glm::vec3 Camera::getCameraPosition(){
  return position;
}

void Camera::Update(){
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians((pitch)));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(front);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera(){}
