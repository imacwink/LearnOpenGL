#include "specular.h"
#include <vector>

namespace ST
{
    namespace Lighting
    {
        void Specular::createCamera()
        {
            mCamera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        void Specular::createShader()
        {
            mLightingShader = new Shader();
            mLightingShader->setVertexShader("./res/shaders/lighting/specular.vs");
            mLightingShader->setFragmentShader("./res/shaders/lighting/specular.fs");
            mLightingShader->compile();

            mLightCubeShader = new Shader();
            mLightCubeShader->setVertexShader("./res/shaders/lighting/specular_light_cube.vs");
            mLightCubeShader->setFragmentShader("./res/shaders/lighting/specular_light_cube.fs");
            mLightCubeShader->compile();
        }

        void Specular::stepGL() 
        {
            glEnable(GL_DEPTH_TEST);

            // configure the cube's VAO (and VBO);
            glGenVertexArrays(1, &mCubeVAO);
            glGenBuffers(1, &mVBO);

            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_STATIC_DRAW);

            glBindVertexArray(mCubeVAO);

            // position attribute;
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // normal attribute;
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube);
            glGenVertexArrays(1, &mLightCubeVAO);
            glBindVertexArray(mLightCubeVAO);

            // we only need to bind to the VBO (to link it with glVertexAttribPointer),
            // no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
        }

        void Specular::DrawSetting()
        {
            this->createCamera();
            this->createShader();
            this->stepGL();
        }

        void Specular::ProcessInput(int a_direction, float a_deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)a_direction, a_deltaTime);
            }
        }

        void Specular::ProcessMouseScroll(float a_yOffset)
        {
            if (mCamera)
            {
                mCamera->ProcessMouseScroll(a_yOffset);
            }
        }

        void Specular::Update(float a_deltaTime)
        {
        }

        void Specular::Draw()
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // activate shader;
            mLightingShader->use();

            // setting uniforms/drawing objects;
            mLightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            mLightingShader->setVec3("lightColor",  1.0f, 1.0f, 1.0f);
            mLightingShader->setVec3("lightPos", mLightPos);
            mLightingShader->setVec3("viewPos", mCamera->mPos);

            // mvp;
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = mCamera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(mCamera->mZoom), (float)Utils::mScreenWidth / (float)Utils::mScreenHeight, 0.1f, 1000.0f);
            mLightingShader->setMat4("projection", projection);
            mLightingShader->setMat4("view", view);
            mLightingShader->setMat4("model", model);

            // render the cube;
            glBindVertexArray(mCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // draw the lamp object;
            // activate shader;
            mLightCubeShader->use();

            // mvp;
            model = glm::mat4(1.0f);
            model = glm::translate(model, mLightPos);
            model = glm::scale(model, glm::vec3(0.2f)); 
            mLightCubeShader->setMat4("projection", projection);
            mLightCubeShader->setMat4("view", view);
            mLightCubeShader->setMat4("model", model);

            // render the light cube;
            glBindVertexArray(mLightCubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void Specular::Uninstall()
        {
        }
    }
}