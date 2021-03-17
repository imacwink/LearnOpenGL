#include "irradiance.h"

namespace ST
{
    namespace PBR
    {
        void Irradiance::createSpheres()
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

        void Irradiance::createCamera()
        {
            mCamera = new Camera(glm::vec3(0.0f, 0.0f, 24.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
        }

        void Irradiance::createShader()
        {
            mPBRShader = new Shader();
            mPBRShader->setVertexShader("./res/shaders/pbr/ibl/pbr.vs");
            mPBRShader->setFragmentShader("./res/shaders/pbr/ibl/pbr.fs");
            mPBRShader->compile();

            mEquirectangularToCubemapShader = new Shader();
            mEquirectangularToCubemapShader->setVertexShader("./res/shaders/pbr/ibl/cubemap.vs");
            mEquirectangularToCubemapShader->setFragmentShader("./res/shaders/pbr/ibl/equirectangular_to_cubemap.fs");
            mEquirectangularToCubemapShader->compile();

            mIrradianceShader = new Shader();
            mIrradianceShader->setVertexShader("./res/shaders/pbr/ibl/cubemap.vs");
            mIrradianceShader->setFragmentShader("./res/shaders/pbr/ibl/irradiance_convolution.fs");
            mIrradianceShader->compile();

            mBackgroundShader = new Shader();
            mBackgroundShader->setVertexShader("./res/shaders/pbr/ibl/background.vs");
            mBackgroundShader->setFragmentShader("./res/shaders/pbr/ibl/background.fs");
            mBackgroundShader->compile();
        }

        void Irradiance::DrawSetting()
        {
            this->createCamera();
            this->createShader();

            mPBRShader->use();
            mPBRShader->setInt("irradianceMap", 0);
            mPBRShader->setVec3("albedo", 0.5f, 0.0f, 0.0f);
            mPBRShader->setFloat("ao", 1.0f);

            mBackgroundShader->use();
            mBackgroundShader->setInt("environmentMap", 0);
        }

        void Irradiance::ProcessInput(int a_direction, float a_deltaTime)
        {
            if (mCamera)
            {
                mCamera->ProcessKeyboard((Camera_Movement)a_direction, a_deltaTime);
            }
        }

        void Irradiance::ProcessMouseScroll(float a_yOffset) {}

        void Irradiance::Update(float a_deltaTime) {}

        void Irradiance::Draw()
        {
        }

        void Irradiance::Uninstall() {}
    }
}