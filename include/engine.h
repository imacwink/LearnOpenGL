// System libs.
#include <iostream>
#include <vector>

// Include libs.
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>

// Header includes.
// TODO: Include your headers here...
#include <component.h>

using namespace std;
using namespace ST::COMMON;

// Function prototypes.
void WindowResizeEventCallback(GLFWwindow* a_window, int a_width, int a_height);
void MouseEventCallback(GLFWwindow* a_window, double a_xPos, double a_yPos);
void ScrollEventCallback(GLFWwindow* a_window, double a_xOffset, double a_yOffset);

static double g_xOffset = 0.0f;
static double g_yOffset = 0.0f;

// Classes.
class Engine
{
public:
    int Initialize();
    float mGameSpeed = 0.1f;

private:
    int mScreenWidth = Utils::mScreenWidth;
    int mScreenHeight = Utils::mScreenHeight;

    const char* mWindowName = "Default Name";

    GLFWwindow* mWindow;

    // Game loop.
    float mLastFrameTime = 0.0f;
    glm::vec3 mClearColor = glm::vec3(0.0f, 0.0f, 0.0f);

    // User Define Instance
    vector <Component*> mComponents;

public:
    Engine(int a_width, int a_height, const char* a_windowName);

    void Update(float a_deltaTime);
    void Draw();

    void ProcessInput(GLFWwindow* a_window, float a_deltaTime);
    void ProcessScroll(double a_xOffset, double a_yOffset);

private:
    // OpenGL
    void SetupOpenGlRendering();
    void UninstallParams();
};