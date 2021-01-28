#include "engine.h"

int main()
{
    Engine engine(Utils::mScreenWidth, Utils::mScreenHeight, "GL Window By MacWink");

    if(!engine.Initialize())
    {
        std::cout << std::endl << "Press Any Key To Close Program..." << std::endl;
        std::cin.get();
    }
}