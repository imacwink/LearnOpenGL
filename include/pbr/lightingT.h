#pragma once

#include <component.h>
#include <shader.h>
#include <camera.h>

using namespace ST::COMMON;

namespace ST
{
    namespace PBR
    {
        class LightingT : public Component
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
                glm::vec3 lightPositions[1] = { glm::vec3(0.0f, 0.0f, 10.0f) };
                glm::vec3 lightColors[1] = { glm::vec3(150.0f, 150.0f, 150.0f) };

                unsigned int albedo    = -1;
                unsigned int normal    = -1;
                unsigned int metallic  = -1;
                unsigned int roughness = -1;
                unsigned int ao        = -1;
        };
    }
}
