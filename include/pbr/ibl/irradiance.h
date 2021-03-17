#ifndef __IRRADIANCE__
#define __IRRADIANCE__

#pragma once

#include <component.h>
#include <shader.h>
#include <camera.h>
#include <sphere.h>
#include <vector>

using namespace ST::COMMON;
using namespace std;

namespace ST
{
    namespace PBR
    {
        class Irradiance : public Component
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
                void createSpheres();

            private:
                Shader* mPBRShader;
                Shader* mEquirectangularToCubemapShader;
                Shader* mIrradianceShader;
                Shader* mBackgroundShader;

                Camera* mCamera;
                vector <Sphere*> mSpheres;

                int mRows    = 7;
                int mColumns = 7;
                float mSpacing = 2.5;

                glm::vec3 mLightPositions[4] = { 
                    glm::vec3(-10.0f,  10.0f, 10.0f), 
                    glm::vec3( 10.0f,  10.0f, 10.0f),
                    glm::vec3(-10.0f, -10.0f, 10.0f),
                    glm::vec3( 10.0f, -10.0f, 10.0f)};
                glm::vec3 mLightColors[4] = {
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f),
                    glm::vec3(300.0f, 300.0f, 300.0f)};
        };
    }
}

#endif