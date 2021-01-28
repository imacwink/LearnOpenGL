#ifndef __COLOR__
#define __COLOR__

#pragma once

#include <component.h>
#include <shader.h>
#include <camera.h>

using namespace ST::COMMON;

namespace ST
{
    namespace Lighting
    {
        class Color : public Component
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
                float mVertices[3 * 6 * 6] = {
                    -0.5f, -0.5f, -0.5f, 
                    0.5f, -0.5f, -0.5f,  
                    0.5f,  0.5f, -0.5f,  
                    0.5f,  0.5f, -0.5f,  
                    -0.5f,  0.5f, -0.5f, 
                    -0.5f, -0.5f, -0.5f, 

                    -0.5f, -0.5f,  0.5f, 
                    0.5f, -0.5f,  0.5f,  
                    0.5f,  0.5f,  0.5f,  
                    0.5f,  0.5f,  0.5f,  
                    -0.5f,  0.5f,  0.5f, 
                    -0.5f, -0.5f,  0.5f, 

                    -0.5f,  0.5f,  0.5f, 
                    -0.5f,  0.5f, -0.5f, 
                    -0.5f, -0.5f, -0.5f, 
                    -0.5f, -0.5f, -0.5f, 
                    -0.5f, -0.5f,  0.5f, 
                    -0.5f,  0.5f,  0.5f, 

                    0.5f,  0.5f,  0.5f,  
                    0.5f,  0.5f, -0.5f,  
                    0.5f, -0.5f, -0.5f,  
                    0.5f, -0.5f, -0.5f,  
                    0.5f, -0.5f,  0.5f,  
                    0.5f,  0.5f,  0.5f,  

                    -0.5f, -0.5f, -0.5f, 
                    0.5f, -0.5f, -0.5f,  
                    0.5f, -0.5f,  0.5f,  
                    0.5f, -0.5f,  0.5f,  
                    -0.5f, -0.5f,  0.5f, 
                    -0.5f, -0.5f, -0.5f, 

                    -0.5f,  0.5f, -0.5f, 
                    0.5f,  0.5f, -0.5f,  
                    0.5f,  0.5f,  0.5f,  
                    0.5f,  0.5f,  0.5f,  
                    -0.5f,  0.5f,  0.5f, 
                    -0.5f,  0.5f, -0.5f, 
                };
        };
    }
}

#endif
