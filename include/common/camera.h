#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

namespace ST 
{
    namespace COMMON 
    {
        class Camera
        {
            public:
                // camera Attributes
                glm::vec3 mPos;
                glm::vec3 mFront;
                glm::vec3 mUp;
                glm::vec3 mRight;
                glm::vec3 mWorldUp;

                // euler Angles
                float mYaw;
                float mPitch;

                // camera options
                float mMovementSpeed;
                float mMouseSensitivity;
                float mZoom;

            public:
                // constructor with vectors
                Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
                
                // constructor with scalar values
                Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

                glm::mat4 GetViewMatrix();
                void ProcessKeyboard(Camera_Movement direction, float deltaTime);
                
                // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
                void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

                // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
                void ProcessMouseScroll(float yoffset);

            private:
                // calculates the front vector from the Camera's (updated) Euler Angles
                void updateCameraVectors();
        };
    }
}
