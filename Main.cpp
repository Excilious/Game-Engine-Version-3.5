#include "Main.h"

Main::Main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    //glfwGetPrimaryMonitor()
    GLFWwindow* EngineWindow = glfwCreateWindow(this->Width,this->Height,"Fusion",NULL,NULL);
    glfwMakeContextCurrent(EngineWindow);
    gladLoadGL();
    glViewport(0,0,this->Width,this->Height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glCullFace(GL_FRONT);
    glfwSwapInterval(1);
    glFrontFace(GL_CCW);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    std::string NewString = std::to_string(VERSION_MAJOR)+std::to_string(VERSION_MINOR);
    std::string Resolution = std::to_string(this->Width)+"x"+std::to_string(this->Height);
    
    CommandGenerateBackFrame(this->Width,this->Height);
    SysPrint("Fusion Engine, GL Version "+NewString,1);
    SysPrint("Resolution: "+Resolution,1);

    if (EngineWindow == NULL) {glfwTerminate();} 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {glfwTerminate();}

    this->Render(EngineWindow);
    glfwDestroyWindow(EngineWindow);
    glfwTerminate();
}

void OnWindowResizedCallback(GLFWwindow* EngineWindow,int Width,int Height) {glViewport(0,0,Width,Height);}

void Main::Release(GLFWwindow* NewWindow)
{
    if (glfwGetKey(NewWindow,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {glfwTerminate(); exit(0);}
}

void Main::CalculateFramesRendered()
{
    this->CurrentTime = glfwGetTime();
    this->DeltaTime = this->CurrentTime - this->PreviousTime;
    this->Counter++;

    if (this->DeltaTime >= 1.0f / 30.0)
    {
        this->FramesPerSecond = std::to_string((1.0/this->DeltaTime) * this->Counter);
        this->PreviousTime = this->CurrentTime;
        this->Counter = 0;
    }
}

void Main::Render(GLFWwindow* EngineWindow)
{
    Shader              SkyboxShader;
    Shader              InterfaceShader;
    World               NewWorld;
    Camera              NewCamera(this->Width,this->Height,glm::vec3(0.0f,0.0f,0.0f));
    Skybox              NewSkybox(this->Width,this->Height,this->NearView,this->FarView);

    glfwSetWindowSizeCallback(EngineWindow,OnWindowResizedCallback);
    glUniform1i(glGetUniformLocation(SkyboxShader.Program,"skybox"),0);
    SkyboxShader.CreateNewShader("../../Shaders/Skybox.vert","../../Shaders/Skybox.frag");
    InterfaceShader.CreateNewShader("../../Shaders/Interface.vert","../../Shaders/Interface.frag");
    SkyboxShader.ActivateShader();

    NewWorld.SetWorldData("../../Shaders/Datamodel.json",this->DeltaTime,this->NearView,this->FarView,this->FieldOfView,NewCamera);
    NewWorld.GeneratePlayer("../../Meshes/Ranger.gltf",glm::vec3(0.0f,0.0f,0.0f),glm::quat(0.0f,0.0f,0.0f,0.0f),glm::vec3(0.02f,0.03f,0.02f),NewCamera);
    NewCamera.Position = glm::vec3(0.0f,10.0f,0.0f);

    while (!glfwWindowShouldClose(EngineWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glfwSetInputMode(EngineWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

        if (IsConsoleOpen)
        {
            if (glfwGetKey(EngineWindow,GLFW_KEY_W) == GLFW_PRESS) {NewWorld.PlayerNode.NewActorInstance.Position += NewCamera.Speed * NewCamera.Orientation;}
            if (glfwGetKey(EngineWindow,GLFW_KEY_A) == GLFW_PRESS) {NewWorld.PlayerNode.NewActorInstance.Position += NewCamera.Speed * -glm::normalize(glm::cross(NewCamera.Orientation,NewCamera.UpPosition));}
            if (glfwGetKey(EngineWindow,GLFW_KEY_S) == GLFW_PRESS) {NewWorld.PlayerNode.NewActorInstance.Position += NewCamera.Speed * -NewCamera.Orientation;}
            if (glfwGetKey(EngineWindow,GLFW_KEY_D) == GLFW_PRESS) {NewWorld.PlayerNode.NewActorInstance.Position += NewCamera.Speed * glm::normalize(glm::cross(NewCamera.Orientation,NewCamera.UpPosition));}
        }


        NewCamera.Position = NewWorld.PlayerNode.NewActorInstance.Position - glm::vec3(0.0f,-1.0f,1.0f);;
        NewCamera.UpdateMatrix(this->FieldOfView,this->NearView,this->FarView);
        NewCamera.UpdateCameraPosition(EngineWindow,this->XPosition,this->YPosition);
        NewSkybox.RenderSkybox(NewCamera.Position,NewCamera.Orientation,NewCamera.UpPosition,&SkyboxShader);
        NewWorld.RenderWorld(&NewCamera);

        CommandUpdateInput(EngineWindow);
        CommandRenderText(EngineWindow);
        glfwGetCursorPos(EngineWindow,&XPosition,&YPosition);
        glfwSwapBuffers(EngineWindow);
        this->Release(EngineWindow);
        glfwPollEvents();
    }

    CommandDelete();
    InterfaceShader.DestroyShader();
    SkyboxShader.DestroyShader();
}

int main()
{Main Engine; return 0;}