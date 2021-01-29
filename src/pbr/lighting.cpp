#include "lighting.h"

namespace ST
{
    namespace PBR
    {
        void Lighting::createSpheres()
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

        void Lighting::createCamera()
        {
            mCamera = new Camera(glm::vec3(0.0f, 0.0f, 24.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        void Lighting::createShader()
        {
            mShader = new Shader();
            mShader->setVertexShader("./res/shaders/pbr/pbr_lighting.vs");
            mShader->setFragmentShader("./res/shaders/pbr/pbr_lighting.fs");
            mShader->compile();
        }

        void Lighting::DrawSetting()
        {
            this->createCamera();
            this->createShader();

            mShader->use();
            
            // Albedo 漫反射就是基础的颜色输入，即我们常说的颜色（Diffuse）贴图
            // Albedo 贴图表现的是漫反射光的颜色。PBR 系统中的 Albedo 贴图与传统
            // Diffuse 贴图最大的区别就是 Albedo 贴图中没有方向光和环境遮罩（AO）
            // 传统 Diffuse 贴图中的方向光在某些光照条件下看上去并不正确，环境遮
            // 罩(AO)应该放入 PBR 系统给出的单独 AO 槽中
            mShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
            mShader->setFloat("ao", 1.0f);
            
            float fFovy = glm::radians(mCamera->mZoom);
            float fAspect = Utils::mScreenWidth * 1.0f / Utils::mScreenHeight * 1.0f;
            glm::mat4 projection = glm::perspective(fFovy, fAspect, 0.1f, 1000.0f);
            mShader->setMat4("projection", projection);

            this->createSpheres();
        }

        void Lighting::ProcessInput(int a_direction, float a_deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)a_direction, a_deltaTime);
            }
        }

        void Lighting::ProcessMouseScroll(float a_yOffset) {}

        void Lighting::Update(float a_deltaTime) {}

        void Lighting::Draw()
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mShader->use();
            glm::mat4 view = mCamera->GetViewMatrix();
            mShader->setMat4("view", view);
            mShader->setVec3("camPos", mCamera->mPos);

            // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
            glm::mat4 model = glm::mat4(1.0f);
            for (int i = 0; i < mRows; ++i) 
            {
                mShader->setFloat("metallic", (float)i / (float)mRows);
                for (int j = 0; j < mColumns; ++j) 
                {
                    // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                    // on direct lighting.
                    mShader->setFloat("roughness", glm::clamp((float)j / (float)mColumns, 0.05f, 1.0f));
                    
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((j - (mColumns / 2)) * mSpacing, (i - (mRows / 2)) * mSpacing, 0.0f));

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

        void Lighting::Uninstall() {}
    }
}