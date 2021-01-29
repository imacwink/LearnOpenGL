#include "lightingT.h"
#include <vector>

namespace ST
{
    namespace PBR
    {
        void LightingT::createSpheres()
        {
            for (int i = 0; i < mRows; ++i) 
            {
                for (int j = 0; j < mColumns; ++j) 
                {
                    Sphere* sphere =  new Sphere();
                    sphere->Initialize();
                    mSpheres.push_back(sphere);
                }
            }
        }

        void LightingT::createCamera()
        {
            mCamera = new Camera(glm::vec3(0.0f, 0.0f, 24.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        void LightingT::createShader()
        {
            mShader = new Shader();
            mShader->setVertexShader("./res/shaders/pbr/pbr_lightingT.vs");
            mShader->setFragmentShader("./res/shaders/pbr/pbr_lightingT.fs");
            mShader->compile();
            mShader->use();
        }

        void LightingT::DrawSetting()
        {
            this->createCamera();
            this->createShader();

            mShader->setInt("albedoMap", 0);
            mShader->setInt("normalMap", 1);
            mShader->setInt("metallicMap", 2);
            mShader->setInt("roughnessMap", 3);
            mShader->setInt("aoMap", 4);

            // load PBR material textures
            mAlbedo    = Utils::loadTexture("./res/textures/pbr/rusted_iron/albedo.png");
            mNormal    = Utils::loadTexture("./res/textures/pbr/rusted_iron/normal.png");
            mMetallic  = Utils::loadTexture("./res/textures/pbr/rusted_iron/metallic.png");
            mRoughness = Utils::loadTexture("./res/textures/pbr/rusted_iron/roughness.png");
            mAO        = Utils::loadTexture("./res/textures/pbr/rusted_iron/ao.png");

            glm::mat4 projection = glm::perspective(glm::radians(mCamera->mZoom), (float)Utils::mScreenWidth / (float)Utils::mScreenHeight, 0.1f, 1000.0f);
            mShader->setMat4("projection", projection);

            this->createSpheres();
        }

        void LightingT::ProcessInput(int a_direction, float a_deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)a_direction, a_deltaTime);
            }
        }

        void LightingT::ProcessMouseScroll(float a_yOffset) {}

        void LightingT::Update(float a_deltaTime) {}

        void LightingT::Draw()
        {
            // render
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mShader->use();
            glm::mat4 view = mCamera->GetViewMatrix();
            mShader->setMat4("view", view);
            mShader->setVec3("camPos", mCamera->mPos);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mAlbedo);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, mNormal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, mMetallic);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, mRoughness);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, mAO);

            // render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
            glm::mat4 model = glm::mat4(1.0f);
            for (int i = 0; i < mRows; ++i)
            {
                for (int j = 0; j < mColumns; ++j)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((float)(j - (mColumns / 2)) * mSpacing, (float)(i - (mRows / 2)) * mSpacing, 0.0f));
                    mShader->setMat4("model", model);
                    mSpheres[i * mColumns + j]->render();
                }
            }

            // render light source (simply re-render sphere at light positions)
            // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
            // keeps the codeprint small.
            for (unsigned int i = 0; i < sizeof(mLightPositions) / sizeof(mLightPositions[0]); ++i)
            {
                glm::vec3 newPos = mLightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = mLightPositions[i];
                mShader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                mShader->setVec3("lightColors[" + std::to_string(i) + "]", mLightColors[i]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.5f));
                mShader->setMat4("model", model);
            }
        }

        void LightingT::Uninstall() {}
    }
}