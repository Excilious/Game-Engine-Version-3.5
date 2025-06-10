#include "Camera.h"
#include "Console.h"

Camera::Camera(int WindowWidth,int WindowHeight,glm::vec3 CameraPosition)
{
    this->Width         = WindowWidth;
    this->Height        = WindowHeight;
    this->Position      = CameraPosition;
}

void Camera::UpdateMatrix(float FieldOfView,float NearPlane,float FarPlane)
{
    glm::mat4 ViewMatrix        = glm::mat4(1.0f);
    glm::mat4 ProjectionMatrix  = glm::mat4(1.0f);

    ViewMatrix          = glm::lookAt(this->Position,this->Position+this->Orientation,this->UpPosition);
    ProjectionMatrix    = glm::perspective(glm::radians(FieldOfView),(float)(this->Width/this->Height),NearPlane,FarPlane);
    this->ViewMatrix    = ViewMatrix;
    this->CameraMatrix  = ProjectionMatrix * ViewMatrix;
}

void Camera::SetMatrix(Shader* ShaderInstance,const char* Uniform)
{glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,Uniform),1,GL_FALSE,glm::value_ptr(this->CameraMatrix));};

void Camera::MoveCamera(GLFWwindow* InputWindow)
{
    float RotationX;
    float RotationY;
    double MouseX;
    double MouseY;
    RotationX = this->Sensitivity * (float)(MouseY - (this->Height/2)) / this->Height;
    RotationY = this->Sensitivity * (float)(MouseX - (this->Width/2)) / this->Width;

    glm::vec3 NewOrientation = glm::rotate(this->Orientation,glm::radians(-RotationX),glm::normalize(glm::cross(this->Orientation,this->UpPosition)));

    if (!((glm::angle(NewOrientation,UpPosition) <= glm::radians(5.0f)) or (glm::angle(NewOrientation,-UpPosition) <= glm::radians(5.0f))))
    {this->Orientation  = NewOrientation;}
    this->Orientation   = glm::rotate(this->Orientation,glm::radians(-RotationY),this->UpPosition);
}

void Camera::UpdateCameraPosition(GLFWwindow* InputWindow,double XPosition,double YPosition)
{
    if (FirstMouse)
    {
        this->LastX         = XPosition;
        this->LastY         = YPosition;
        this->FirstMouse    = false;
    }

    this->XOffset   = XPosition - this->LastX;
    this->YOffset   = this->LastY - YPosition;
    this->LastX     = XPosition;
    this->LastY     = YPosition;

    this->XOffset *= this->Sensitivity;
    this->YOffset *= this->Sensitivity;
    this->RotateX += this->XOffset;
    this->RotateY += this->YOffset;

    if (this->RotateY > 89.0f)  {this->RotateY = 89.0f;}
    if (this->RotateY < -89.0f) {this->RotateY = -89.0f;}

    this->Orientation.x = cos(glm::radians(this->RotateX)) * cos(glm::radians(this->RotateY));
    this->Orientation.y = sin(glm::radians(this->RotateY));
    this->Orientation.z = sin(glm::radians(this->RotateX)) * cos(glm::radians(this->RotateY));
    this->Orientation = glm::normalize(this->Orientation);
}

void Camera::Inputs(GLFWwindow* InputWindow)
{
    glfwSetInputMode(InputWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    if (IsConsoleOpen)
    {
        if (glfwGetKey(InputWindow,GLFW_KEY_W) == GLFW_PRESS) {this->Position += this->Speed * this->Orientation;}
        if (glfwGetKey(InputWindow,GLFW_KEY_A) == GLFW_PRESS) {this->Position += this->Speed * -glm::normalize(glm::cross(this->Orientation,this->UpPosition));}
        if (glfwGetKey(InputWindow,GLFW_KEY_S) == GLFW_PRESS) {this->Position += this->Speed * -this->Orientation;}
        if (glfwGetKey(InputWindow,GLFW_KEY_D) == GLFW_PRESS) {this->Position += this->Speed * glm::normalize(glm::cross(this->Orientation,this->UpPosition));}
    }
}