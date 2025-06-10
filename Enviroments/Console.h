#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define CONSOLE_ERROR   glm::vec3(1.0f,0.0f,0.0f)
#define CONSOLE_WARN    glm::vec3(1.0f,1.0f,0.0f)
#define CONSOLE_INFO    glm::vec3(1.0f,1.0f,1.0f)

#include <windows.h>
#include "Interface/Interface.h"
#include "../Dependancies/GLFW/glfw3.h"

typedef void (*Function) (void);
void CommandAddCommand(std::string NewCommandString,Function CommandFunction);
void CommandUpdateCommandExecution();

void EnableBoundingBox(void);
void EnableEnviromentCollision(void);
void EnableEnviromentLights(void);

extern bool EnableLights;
extern bool EnableBounds;
extern bool EnableTriangle;
extern bool IsConsoleOpen;
extern std::vector<TextLabel> Console;
extern void SysPrint(std::string Text,int ErrorCode);
extern void CommandUpdateInput(GLFWwindow* NewWindow);
extern void CommandRenderText(GLFWwindow* NewWindow);
extern void CommandGenerateBackFrame(int Width,int Height);
extern void CommandUpdateKeyInput(GLFWwindow* NewWindow,unsigned int KeyCode);
extern void CommandDelete();

#endif