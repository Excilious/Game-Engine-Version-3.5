#include "Console.h"
#include "Interface/Interface.h"

typedef struct CommandFunctions
{
    Function        ExecutableFunction;
    std::string     CommandString;
} Command;

bool                        IsConsoleOpen   = true;
double                      LastTime        = 0.0;
double                      CurrentTime     = 0.0;
float                       PositionY       = 0.0f;
std::vector<TextLabel>      Console         = {};
std::vector<glm::vec2>      PositionCache;
std::vector<Command>        CommandList;
std::string                 CommandString;
Frame                       *ConsoleFrame;
Frame                       *DragFrame;

bool                        EnableBounds    = false;
bool                        EnableLights    = false;
bool                        EnableTriangle  = false;

void SysPrint(std::string Text,int ErrorCode = 1)
{
    if (Console.size() >= 16) {Console.erase(Console.begin(),Console.begin()+1);}
    glm::vec3 Colour;
    if (ErrorCode == 1)         {Colour = CONSOLE_INFO;}
    else if (ErrorCode == 2)    {Colour = CONSOLE_WARN;}
    else if (ErrorCode == 3)    {Colour = CONSOLE_ERROR;}
    TextLabel NewTextLabel(Text.c_str(),1.0f,glm::vec2(0.0f,PositionY),Colour);
    Console.push_back(NewTextLabel);
    for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++) 
    {Console[ConsoleIndex].Position = glm::vec2(0.0f,1050-(19.0f*ConsoleIndex));}
}

void EnableBoundingBox(void)
{
    if (!EnableBounds)
    {EnableBounds = true; SysPrint("Bounds Enabled.",1);}
    else 
    {EnableBounds = false; SysPrint("Bounds Disabled.",1);}
}

void EnableEnviromentCollision(void)
{
    if (!EnableTriangle)
    {EnableTriangle = true; SysPrint("Triangle Bounds Enabled.",1);}
    else 
    {EnableTriangle = false; SysPrint("Triangle Bounds Disabled",1);}
}

void EnableEnviromentLights(void)
{
    if (!EnableLights)
    {EnableLights = true; SysPrint("Enviromental Lights Enabled",1);}
    else 
    {EnableLights = false; SysPrint("Enviromental Light Disabled",1);}
}

void CommandAddCommand(std::string NewCommandString,Function CommandFunction)
{
    Command NewCommand;
    for (int CommandIndex = 0; CommandIndex > CommandList.size(); CommandIndex++)
    {
        if (CommandList[CommandIndex].CommandString == NewCommandString)
        {SysPrint("Command already exits.",2); return;}
    }
    NewCommand.CommandString       = NewCommandString;
    NewCommand.ExecutableFunction  = CommandFunction;
    CommandList.push_back(NewCommand);
}

void CommandGenerateBackFrame(int Width,int Height)
{
    CommandAddCommand("ToggleBounds",EnableBoundingBox);
    CommandAddCommand("ToggleCollision",EnableEnviromentCollision);
    CommandAddCommand("ToggleLighting",EnableEnviromentLights);
    ConsoleFrame    = new Frame(glm::vec2(0.0f,800.0f),glm::vec2(Width,600.0f),Width,Height,glm::vec3(0.0f,0.0f,0.0f),0.0f,"../../Textures/Placeholder.png");
    DragFrame       = new Frame(glm::vec2(0.0f,-240.0f),glm::vec2(Width,1000.0f),Width,Height,glm::vec3(0.0f,0.0f,0.0f),0.0f,"../../Textures/Placeholder.png");
}

void CommandUpdateInput(GLFWwindow* NewWindow)
{

    if (glfwGetKey(NewWindow,GLFW_KEY_TAB) == GLFW_PRESS)
    {
        CurrentTime = glfwGetTime();
        if (CurrentTime - LastTime >= 2.0)
        {
            if (IsConsoleOpen)  {IsConsoleOpen = false;}
            else                {IsConsoleOpen = true;}
            LastTime = CurrentTime;
        }
    }
}

void CommandUpdateCommandExecution()
{
    int CommandIndex = -1;
    for (int Index = 0; Index < CommandList.size(); Index++)
    {
        if (CommandList[Index].CommandString == CommandString)
        {CommandIndex = Index; break;}
    }
    if (CommandIndex != -1) {CommandList[CommandIndex].ExecutableFunction();}
    else                    {SysPrint("Invalid/Unknown Command",3);}
}

void CommandUpdateKeyInput(GLFWwindow* NewWindow,unsigned int KeyCode)
{
    if (!IsConsoleOpen)
    {char NewCode = KeyCode; CommandString += NewCode;}
}

void CommandMoveFrame()
{
    if (IsConsoleOpen)
    {
        ConsoleFrame->Position.y    += 6.0f;
        DragFrame->Position.y       -= 6.0f;
        if (ConsoleFrame->Position.y >= 1440)   {ConsoleFrame->Position.y   = 1440;}
        if (ConsoleFrame->Position.y <= 0)      {DragFrame->Position.y      = 0;}
        for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++)
        {if (!(ConsoleFrame->Position.y >= 1440)) {Console[ConsoleIndex].Position.y += 6.0f;}}
    }
    else
    {
        ConsoleFrame->Position.y    -= 6.0f;
        if (ConsoleFrame->Position.y <= 740)    {ConsoleFrame->Position.y   = 740;}
        for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++)
        {if (!(ConsoleFrame->Position.y <= 740)) {Console[ConsoleIndex].Position.y -= 6.0f;}}
    }
    for (int ConsoleIndex = 0; ConsoleIndex < Console.size(); ConsoleIndex++)
    {Console[ConsoleIndex].RenderText();}
}

void CommandRenderText(GLFWwindow* NewWindow)
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    ConsoleFrame->RenderInstance();
    DragFrame->RenderInstance();
    if (glfwGetKey(NewWindow,GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (CommandString.size() != 0)
        {SysPrint(CommandString,1);CommandUpdateCommandExecution();CommandString = "";}
    }
    glfwSetCharCallback(NewWindow,CommandUpdateKeyInput);
    CommandMoveFrame();
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void CommandDelete()
{
    for (int TextLabelIndex = 0; TextLabelIndex < Console.size(); TextLabelIndex++)
    {Console[TextLabelIndex].DestroyText();}
    Console = {};
    if (ConsoleFrame)   delete ConsoleFrame;
    if (DragFrame)      delete DragFrame;
}