#pragma once

#include "utils.h"

namespace ST 
{
    namespace COMMON 
    {
        class Component
        {
            public:
                virtual void DrawSetting() = 0;
                virtual void Update(float a_deltaTime) = 0;
                virtual void Draw() = 0;
                virtual void Uninstall() = 0;
                virtual void ProcessInput(int direction, float deltaTime) = 0;
        };
    }
}
