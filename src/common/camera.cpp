#include "camera.h"

namespace ST 
{
    namespace COMMON 
    {
        // constructor with vectors
        Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
        {
            mPos = pos;
            mWorldUp = up;
            mYaw = yaw;
            mPitch = pitch;
            mFront = glm::vec3(0.0f, 0.0f, -1.0f);
            mMovementSpeed = SPEED;
            mMouseSensitivity = SENSITIVITY;
            mZoom = ZOOM;
            updateCameraVectors();
        }

        // constructor with scalar values
        Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM)
        {
            mPos = glm::vec3(posX, posY, posZ);
            mWorldUp = glm::vec3(upX, upY, upZ);
            mYaw = yaw;
            mPitch = pitch;
            updateCameraVectors();
        }

        glm::mat4 Camera::GetViewMatrix()
        {
            return glm::lookAt(mPos, mPos + mFront, mUp);
        }

        void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = mMovementSpeed * deltaTime;
            if (direction == FORWARD)
                mPos += mFront * velocity;
            if (direction == BACKWARD)
                mPos -= mFront * velocity;
            if (direction == LEFT)
                mPos -= mRight * velocity;
            if (direction == RIGHT)
                mPos += mRight * velocity;
        }

        void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
        {
            xoffset *= mMouseSensitivity;
            yoffset *= mMouseSensitivity;

            mYaw   += xoffset;
            mPitch += yoffset;

            // make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (mPitch > 89.0f)
                    mPitch = 89.0f;
                if (mPitch < -89.0f)
                    mPitch = -89.0f;
            }

            // update Front, Right and Up Vectors using the updated Euler angles
            updateCameraVectors();
        }

        void Camera::ProcessMouseScroll(float yoffset)
        {
            mZoom -= (float)yoffset/100;
            if (mZoom < 1.0f)
                mZoom = 1.0f;
            if (mZoom > 45.0f)
                mZoom = 45.0f; 
        }

        void Camera::updateCameraVectors()
        {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            front.y = sin(glm::radians(mPitch));
            front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
            mFront = glm::normalize(front);

            // also re-calculate the Right and Up vector
            mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            mUp    = glm::normalize(glm::cross(mRight, mFront));
        }
    }
}