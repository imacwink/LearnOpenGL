#ifndef __SPHERE__
#define __SPHERE__

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace ST
{
    namespace COMMON
    {
        class Sphere
        {
            public:
                void Initialize();
                void render();
            
            private:


            private:
                unsigned int mVAO = 0;
                unsigned int mIndexCount = 0;

        };
    }
}

#endif
