#define DEBUG

#define VERSION_MINOR 3
#define VERSION_MAJOR 3

#include <iostream>
#include <string>
#include <math.h>
#include "Dependancies/glad/glad.h"
#include "Dependancies/GLFW/glfw3.h"
#include "Dependancies/glm/glm.hpp"

#include "Enviroments/Skybox.h"
#include "Enviroments/World/World.h"
#include "Enviroments/Interface/Interface.h"
#include "Enviroments/Console.h"

class Main
{
    public:
        Main();
        void CalculateFramesRendered();
        void Render(GLFWwindow* NewWindow);
        void Release(GLFWwindow* NewWindow);
        int Width               = 1920;
        int Height              = 1080;
        
        float NearView          = 0.1f;
        float FarView           = 1000.0f;
        float FieldOfView       = 90.0f;

        double PreviousTime     = 0.0;
        double CurrentTime      = 0.0;
        double DeltaTime;
        double XPosition;
        double YPosition;
        unsigned int Counter    = 0;

        std::string FramesPerSecond;
};

