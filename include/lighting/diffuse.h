#ifndef __DIFFUSE__
#define __DIFFUSE__

#pragma once

#include <component.h>
#include <shader.h>
#include <camera.h>

using namespace ST::COMMON;

namespace ST
{
    namespace Lighting
    {
        class Diffuse : public Component
        {
            public:
                void DrawSetting();
                void Update(float a_deltaTime);
                void Draw();
                void Uninstall();
                void ProcessInput(int a_direction, float a_deltaTime);
                void ProcessMouseScroll(float a_yOffset);
            
            private:
                void createCamera();
                void createShader();
                void stepGL();

            private:
                Shader* mLightingShader;
                Shader* mLightCubeShader;
                Camera* mCamera;

                unsigned int mVBO;
                unsigned int mCubeVAO;
                unsigned int mLightCubeVAO;

                // world postion
                glm::vec3 mLightPos = {1.2f, 1.0f, 2.0f};

                // vertex data
                float mVertices[6 * 6 * 6] = {
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
                };
        };
    }
}

#endif
