#pragma once

#include <component.h>
#include <shader.h>
#include <camera.h>

using namespace ST::COMMON;

namespace ST
{
    namespace PBR
    {
        class Lighting : public Component
        {
            public:
                void DrawSetting();
                void Update(float a_deltaTime);
                void Draw();
                void Uninstall();
                void ProcessInput(int direction, float deltaTime);
            
            private:
                void createCamera();
                void createShader();
                void renderSphere();

            private:
                Shader* mShader;
                Camera* mCamera;

                int nrRows    = 7;
                int nrColumns = 7;
                float spacing = 2.5;
                unsigned int sphereVAO = 0;
                unsigned int indexCount;
                glm::vec3 lightPositions[4] = { 
                    glm::vec3(-10.0f,  10.0f, 10.0f), 
                    glm::vec3( 10.0f,  10.0f, 10.0f),
                    glm::vec3(-10.0f, -10.0f, 10.0f),
                    glm::vec3( 10.0f, -10.0f, 10.0f)};
                glm::vec3 lightColors[4] = {
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f)};
        };
    }
}
