#include "lighting.h"
#include <vector>

namespace ST
{
    namespace PBR
    {
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
            mShader->use();
        }

        void Lighting::DrawSetting()
        {
            this->createCamera();
            this->createShader();

            mShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
            mShader->setFloat("ao", 1.0f);

            glm::mat4 projection = glm::perspective(glm::radians(mCamera->mZoom), Utils::mScreenWidth * 1.0f / Utils::mScreenHeight * 1.0f, 0.1f, 1000.0f);
            mShader->setMat4("projection", projection);
        }

        void Lighting::ProcessInput(int a_direction, float a_deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)a_direction, a_deltaTime);
            }
        }

        void Lighting::ProcessMouseScroll(float a_yOffset)
        {
            
        }

        void Lighting::Update(float a_deltaTime)
        {
        }

        void Lighting::Draw()
        {
            // render
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mShader->use();
            glm::mat4 view = mCamera->GetViewMatrix();
            mShader->setMat4("view", view);
            mShader->setVec3("camPos", mCamera->mPos);

            // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
            glm::mat4 model = glm::mat4(1.0f);
            for (int row = 0; row < nrRows; ++row) 
            {
                mShader->setFloat("metallic", (float)row / (float)nrRows);
                for (int col = 0; col < nrColumns; ++col) 
                {
                    // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                    // on direct lighting.
                    mShader->setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
                    
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(
                        (col - (nrColumns / 2)) * spacing, 
                        (row - (nrRows / 2)) * spacing, 
                        0.0f
                    ));
                    mShader->setMat4("model", model);
                    renderSphere();
                }
            }

            // render light source (simply re-render sphere at light positions)
            // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
            // keeps the codeprint small.
            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
            {
                glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
                newPos = lightPositions[i];
                mShader->setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
                mShader->setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

                model = glm::mat4(1.0f);
                model = glm::translate(model, newPos);
                model = glm::scale(model, glm::vec3(0.5f));
                mShader->setMat4("model", model);
                renderSphere();
            }
        }

        void Lighting::Uninstall()
        {
        }

        // renders (and builds at first invocation) a sphere
        // -------------------------------------------------
        void Lighting::renderSphere()
        {
            if (sphereVAO == 0)
            {
                glGenVertexArrays(1, &sphereVAO);

                unsigned int vbo, ebo;
                glGenBuffers(1, &vbo);
                glGenBuffers(1, &ebo);

                std::vector<glm::vec3> positions;
                std::vector<glm::vec2> uv;
                std::vector<glm::vec3> normals;
                std::vector<unsigned int> indices;

                const unsigned int X_SEGMENTS = 64;
                const unsigned int Y_SEGMENTS = 64;
                const float PI = 3.14159265359;
                for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
                {
                    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        float xSegment = (float)x / (float)X_SEGMENTS;
                        float ySegment = (float)y / (float)Y_SEGMENTS;
                        float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                        float yPos = std::cos(ySegment * PI);
                        float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                        positions.push_back(glm::vec3(xPos, yPos, zPos));
                        uv.push_back(glm::vec2(xSegment, ySegment));
                        normals.push_back(glm::vec3(xPos, yPos, zPos));
                    }
                }

                bool oddRow = false;
                for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
                {
                    if (!oddRow) // even rows: y == 0, y == 2; and so on
                    {
                        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                        {
                            indices.push_back(y       * (X_SEGMENTS + 1) + x);
                            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        }
                    }
                    else
                    {
                        for (int x = X_SEGMENTS; x >= 0; --x)
                        {
                            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                            indices.push_back(y       * (X_SEGMENTS + 1) + x);
                        }
                    }
                    oddRow = !oddRow;
                }
                indexCount = indices.size();

                std::vector<float> data;
                for (unsigned int i = 0; i < positions.size(); ++i)
                {
                    data.push_back(positions[i].x);
                    data.push_back(positions[i].y);
                    data.push_back(positions[i].z);
                    if (uv.size() > 0)
                    {
                        data.push_back(uv[i].x);
                        data.push_back(uv[i].y);
                    }
                    if (normals.size() > 0)
                    {
                        data.push_back(normals[i].x);
                        data.push_back(normals[i].y);
                        data.push_back(normals[i].z);
                    }
                }
                glBindVertexArray(sphereVAO);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
                float stride = (3 + 2 + 3) * sizeof(float);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
            }

            glBindVertexArray(sphereVAO);
            glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}