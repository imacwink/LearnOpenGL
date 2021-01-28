#include "lightingT.h"
#include <vector>

namespace ST
{
    namespace PBR
    {
        void LightingT::createCamera()
        {
            mCamera = new Camera(glm::vec3(0.0f, 0.0f, 24.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        void LightingT::createShader()
        {
            mShader = new Shader();
            mShader->setVertexShader("./res/shaders/pbr_lightingT.vs");
            mShader->setFragmentShader("./res/shaders/pbr_lightingT.fs");
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
            albedo    = Utils::loadTexture("./res/textures/pbr/rusted_iron/albedo.png");
            normal    = Utils::loadTexture("./res/textures/pbr/rusted_iron/normal.png");
            metallic  = Utils::loadTexture("./res/textures/pbr/rusted_iron/metallic.png");
            roughness = Utils::loadTexture("./res/textures/pbr/rusted_iron/roughness.png");
            ao        = Utils::loadTexture("./res/textures/pbr/rusted_iron/ao.png");

            glm::mat4 projection = glm::perspective(glm::radians(mCamera->mZoom), (float)Utils::mScreenWidth / (float)Utils::mScreenHeight, 0.1f, 1000.0f);
            mShader->setMat4("projection", projection);
        }

        void LightingT::ProcessInput(int direction, float deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)direction, deltaTime);
            }
        }

        void LightingT::Update(float a_deltaTime)
        {
        }

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
            glBindTexture(GL_TEXTURE_2D, albedo);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normal);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, metallic);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, roughness);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, ao);

            // render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
            glm::mat4 model = glm::mat4(1.0f);
            for (int row = 0; row < nrRows; ++row)
            {
                for (int col = 0; col < nrColumns; ++col)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(
                        (float)(col - (nrColumns / 2)) * spacing,
                        (float)(row - (nrRows / 2)) * spacing,
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
                // renderSphere();
            }
        }

        void LightingT::Uninstall()
        {
        }
        
        // renders (and builds at first invocation) a sphere
        // -------------------------------------------------
        void LightingT::renderSphere()
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