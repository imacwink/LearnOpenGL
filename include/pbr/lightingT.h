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
        class LightingT : public Component
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
                Shader* mShader;
                Camera* mCamera;
                vector <Sphere*> mSpheres;

                int mRows    = 3;
                int mColumns = 3;
                float mSpacing = 2.5;

                glm::vec3 mLightPositions[1] = { glm::vec3(0.0f, 0.0f, 10.0f) };
                glm::vec3 mLightColors[1] = { glm::vec3(150.0f, 150.0f, 150.0f) };

                unsigned int mAlbedo    = -1;
                unsigned int mNormal    = -1;
                unsigned int mMetallic  = -1;
                unsigned int mRoughness = -1;
                unsigned int mAO        = -1;
        };
    }
}
