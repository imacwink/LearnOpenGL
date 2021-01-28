// System libs.
#include <iostream>

// Header includes.
// TODO: Include your headers here...
#include <component.h>
#include <shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Classes.
class Example : public ST::COMMON::Component
{
    public:
        Example();

        void DrawSetting();
        void Update(float a_deltaTime);
        void Draw();
        void Uninstall();
        void ProcessInput(int direction, float deltaTime);
    private:
        Shader* mShader;
        unsigned int mTexture1, mTexture2;
        unsigned int mVBO, mVAO;
};