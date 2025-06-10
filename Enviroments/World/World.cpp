#include "World.h"
#include "../Console.h"
#include "../Texture.h"

World::World() {}

void World::GeneratePlayer(std::string FileDirectory,glm::vec3 CharacterPosition,glm::quat CharacterRotation,glm::vec3 CharacterScale,Camera EngineCamera)
{
    Bounds      NewCharacterBounds;
    Shader      NewActorShader;
    ActorNode   NewActorNode;
    Actor       NewActor;

    NewActor.CreateModel(FileDirectory.c_str(),CharacterPosition,CharacterScale,CharacterRotation,this->Near,this->Far,this->FieldOfView,false);
    NewActorShader.CreateNewShader("../../Shaders/Default.vert","../../Shaders/Default.frag");
    NewActorNode.NewActorInstance   = NewActor;
    NewActorNode.ShaderActor        = NewActorShader;
    NewActorNode.CollisionType      = COLLISION_BOUNDING_BOX;
    this->PlayerNode = NewActorNode;
}

void World::GenerateCharacterEntity(std::string FileDirectory,glm::vec3 CharacterPosition,glm::quat CharacterRotation,glm::vec3 CharacterScale,Camera EngineCamera)
{
    Bounds      NewCharacterBounds;
    Shader      NewActorShader;
    ActorNode   NewActorNode;
    Actor       NewActor;

    NewActor.CreateModel(FileDirectory.c_str(),CharacterPosition,CharacterScale,CharacterRotation,this->Near,this->Far,this->FieldOfView,false);
    NewActorShader.CreateNewShader("../../Shaders/Default.vert","../../Shaders/Default.frag");
    NewActorNode.NewActorInstance   = NewActor;
    NewActorNode.ShaderActor        = NewActorShader;
    NewActorNode.CollisionType      = COLLISION_BOUNDING_BOX;
    this->CharacterNode.push_back(NewActorNode);
}

void World::GenerateLightEntity(std::string FileDirectory,glm::vec3 LightPosition,glm::quat LightRotation,glm::vec3 LightScale,glm::vec3 Colour,float Brightness,float Gloss,float Transparency)
{
    Shader      LightShader;
    Lighting    NewLight;
    LightNode   NewNode;

    LightShader.CreateNewShader("../../Shaders/Default.vert","../../Shaders/Default.frag");
    NewLight.CreateLight(Colour);
    NewLight.CreateModel(FileDirectory.c_str(),LightPosition,LightScale,LightRotation,this->Near,this->Far,this->FieldOfView,false);
    NewLight.Brightness     = Brightness;
    NewLight.Gloss          = Gloss;
    NewLight.Transparency   = Transparency;
    NewNode.LightObject     = NewLight;
    NewNode.LightShader     = LightShader;
    this->LightEntityNode.push_back(NewNode);
}

void World::GenerateScene()
{
    this->WorldContents     = Json::parse(this->Contents);
    this->SceneDirectory    = this->WorldContents["Scene"][0];

    ModelNode   SceneNode;
    Shader      SceneShader;
    Model       SceneData;

    AABB        PlayerCollision(glm::vec3(0.0f,0.0f,0.0f),2);
    AABB        NewBounds(glm::vec3(0.0f,0.0f,0.0f),SceneData.VerticesCollision.size());
    Octree      LocalOctree(NewBounds,SceneData.VerticesCollision.size());

    SceneData.CreateModel(this->SceneDirectory.c_str(),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f),glm::quat(0.0f,0.0f,0.0f,0.0f),this->Near,this->Far,this->FieldOfView,false);
    SceneShader.CreateNewShader("../../Shaders/Default.vert","../../Shaders/Default.frag");
    
    SceneNode.ModelShader   = SceneShader; 
    SceneNode.NewModel      = SceneData;
    SceneNode.CollisionType = COLLISION_PRECISE;
    SceneNode.EnableGravity = false;
    this->WorldNode         = SceneNode;

    this->PlayerCollisionBox    = &PlayerCollision;
    this->NewOctree             = &LocalOctree;    
}

void World::SetWorldData(const char* LevelDirectory,double DeltaTime,float Near,float Far,float FieldOfView,Camera EngineCamera)
{
    std::string Directory   = LevelDirectory;
    SysPrint("Parsing "+Directory,1);
    this->Near          = Near;
    this->Far           = Far;
    this->FieldOfView   = FieldOfView;
    this->Contents      = GetShaderContents(LevelDirectory);
    this->DeltaTime     = DeltaTime;
    this->WorldPhysics.GeneratePhysics(this->DeltaTime);

    this->GenerateCharacterEntity("../../Meshes/Ranger.gltf",glm::vec3(0.0f,0.0f,0.0f),glm::quat(0.0f,0.0f,0.0f,0.0f),glm::vec3(0.02f,0.03f,0.02f),EngineCamera);
    this->GenerateScene();

    for (int Index = 0; Index < this->WorldContents["Lights"].size(); Index++)
    {
        std::string Directory   = this->WorldContents["Lights"][Index]["Directory"];
        glm::vec3 Position      = glm::vec3(this->WorldContents["Lights"][Index]["Position"][0],this->WorldContents["Lights"][Index]["Position"][1],this->WorldContents["Lights"][Index]["Position"][2]);
        glm::vec3 Scale         = glm::vec3(this->WorldContents["Lights"][Index]["Scale"][0],this->WorldContents["Lights"][Index]["Scale"][1],this->WorldContents["Lights"][Index]["Scale"][2]);  
        glm::vec3 Colour        = glm::vec3(this->WorldContents["Lights"][Index]["Colour"][0],this->WorldContents["Lights"][Index]["Colour"][1],this->WorldContents["Lights"][Index]["Colour"][2]);
        glm::quat Rotation      = glm::quat(this->WorldContents["Lights"][Index]["Rotation"][0],this->WorldContents["Lights"][Index]["Rotation"][1],this->WorldContents["Lights"][Index]["Rotation"][2],this->WorldContents["Lights"][Index]["Rotation"][3]);
        float Brightness        = this->WorldContents["Lights"][Index]["Brightness"][0];
        float Gloss             = this->WorldContents["Lights"][Index]["Gloss"][0];
        float Transparency      = this->WorldContents["Lights"][Index]["Transparency"][0];

        this->GenerateLightEntity(Directory.c_str(),Position,Rotation,Scale,Colour,Brightness,Gloss,Transparency);
    }
    
    for (int Index = 0; Index < this->WorldContents["Objects"].size(); Index++)
    {
        std::string Directory   = this->WorldContents["Objects"][Index]["Directory"];
        glm::vec3 Position      = glm::vec3(this->WorldContents["Objects"][Index]["Position"][0],this->WorldContents["Objects"][Index]["Position"][1],this->WorldContents["Objects"][Index]["Position"][2]);
        glm::vec3 Scale         = glm::vec3(this->WorldContents["Objects"][Index]["Scale"][0],this->WorldContents["Objects"][Index]["Scale"][1],this->WorldContents["Objects"][Index]["Scale"][2]);
        glm::quat Rotation      = glm::quat(this->WorldContents["Objects"][Index]["Rotation"][0],this->WorldContents["Objects"][Index]["Rotation"][1],this->WorldContents["Objects"][Index]["Rotation"][2],this->WorldContents["Objects"][Index]["Rotation"][3]);
        int       Density       = this->WorldContents["Objects"][Index]["Density"];
        if (!Density)           {Density  = 20;}

        ModelNode   ObjectNode;
        Shader      ObjectShader;
        Model       ObjectData;
        Bounds      NewBounds;

        ObjectData.CreateModel(Directory.c_str(),Position,Scale,Rotation,this->Near,this->Far,this->FieldOfView,false);
        ObjectShader.CreateNewShader("../../Shaders/Default.vert","../../Shaders/Default.frag");
        ObjectNode.NewModel      = ObjectData;
        ObjectNode.CollisionType = COLLISION_BOUNDING_BOX; 
        ObjectNode.ModelShader   = ObjectShader;
        ObjectNode.EntityPhysics.Density = Density;
        ObjectNode.EnableGravity = true;
    }
}

bool World::CheckPlayerCollision(glm::vec3 Minimum,glm::vec3 Maximum)
{
    bool HasCollided;
    HasCollided = this->WorldCollider.GenerateCollisionPrecise(Minimum,this->WorldNode.NewModel.VerticesCollision,this->WorldNode.NewModel.IndicesCollision,this->WorldNode.NewModel.InstanceMatrix,0.01f);
    HasCollided = this->WorldCollider.GenerateCollisionPrecise(Maximum,this->WorldNode.NewModel.VerticesCollision,this->WorldNode.NewModel.IndicesCollision,this->WorldNode.NewModel.InstanceMatrix,0.01f);
    return HasCollided;
}

void World::ApplyObjectLight(unsigned int ShaderProgram)
{
    if (!EnableLights) {return;}
    for (int LightIndex = 0; LightIndex < this->LightEntityNode.size(); LightIndex++)
    {
        LightNode Node = this->LightEntityNode[LightIndex];
        std::string LightColourArray    = "LightColour["+std::to_string(LightIndex)+"]";
        std::string LightSourceArray    = "LightSource["+std::to_string(LightIndex)+"]";
        std::string LightGlossArray     = "LightGloss["+std::to_string(LightIndex)+"]";
        std::string LightBrightArray    = "Brightness["+std::to_string(LightIndex)+"]";

        glUniform1i(glGetUniformLocation(ShaderProgram,"FullBright"),0);
        glUniform1i(glGetUniformLocation(ShaderProgram,"ObjectLightInstanceNumber"),this->LightEntityNode.size());
        glUniform1f(glGetUniformLocation(ShaderProgram,"Transparency"),Node.LightObject.Transparency);
        glUniform3f(glGetUniformLocation(ShaderProgram,LightColourArray.c_str()),Node.LightObject.Colours.x,Node.LightObject.Colours.y,Node.LightObject.Colours.z);
        glUniform3f(glGetUniformLocation(ShaderProgram,LightSourceArray.c_str()),Node.LightObject.Position.x,Node.LightObject.Position.y,Node.LightObject.Position.z);
        glUniform1f(glGetUniformLocation(ShaderProgram,LightGlossArray.c_str()),Node.LightObject.Gloss);
        glUniform1f(glGetUniformLocation(ShaderProgram,LightBrightArray.c_str()),Node.LightObject.Brightness);
    }
}

void World::RenderWorld(Camera* EngineCamera)
{  
    this->WorldNode.ModelShader.ActivateShader();
    this->ApplyObjectLight(this->WorldNode.ModelShader.Program);
    this->WorldNode.NewModel.Draw(&this->WorldNode.ModelShader,EngineCamera);

    this->PlayerNode.NewActorInstance.Draw(&this->PlayerNode.ShaderActor,EngineCamera);
    this->ApplyObjectLight(this->PlayerNode.ShaderActor.Program);

    for (int EntityIndex = 0; EntityIndex < this->EntityNode.size(); EntityIndex++)
    {
        if (EnableBounds){this->EntityNode[EntityIndex].DeveloperBounds.RenderBoundViews(EngineCamera->CameraMatrix);}
        this->EntityNode[EntityIndex].ModelShader.ActivateShader();
        this->ApplyObjectLight(this->EntityNode[EntityIndex].ModelShader.Program);
        this->EntityNode[EntityIndex].NewModel.Draw(&this->EntityNode[EntityIndex].ModelShader,EngineCamera);
    }

    if (this->PlayerNode.NewActorInstance.Position.y != this->WorldNode.NewModel.Position.y)
    {
        this->PlayerNode.NewActorInstance.Position.y = this->WorldNode.NewModel.Position.y;
    }

    for (int CharacterIndex = 0; CharacterIndex < this->CharacterNode.size(); CharacterIndex++)
    {
        if (EnableBounds){this->CharacterNode[CharacterIndex].DeveloperBounds.RenderBoundViews(EngineCamera->CameraMatrix);}
        this->CharacterNode[CharacterIndex].ShaderActor.ActivateShader();
        this->ApplyObjectLight(this->CharacterNode[CharacterIndex].ShaderActor.Program);
        this->CharacterNode[CharacterIndex].NewActorInstance.Draw(&this->CharacterNode[CharacterIndex].ShaderActor,EngineCamera);
    }    
    for (int LightIndex = 0; LightIndex < this->LightEntityNode.size(); LightIndex++)
    {
        if (!EnableLights) {return;}
        this->LightEntityNode[LightIndex].LightShader.ActivateShader();
        this->LightEntityNode[LightIndex].LightObject.Draw(&this->LightEntityNode[LightIndex].LightShader,EngineCamera);
        
        glUniform3f(glGetUniformLocation(this->LightEntityNode[LightIndex].LightShader.Program,"LightColour[0]"),this->LightEntityNode[LightIndex].LightObject.Colours.x,this->LightEntityNode[LightIndex].LightObject.Colours.y,this->LightEntityNode[LightIndex].LightObject.Colours.z);
        glUniform1i(glGetUniformLocation(this->LightEntityNode[LightIndex].LightShader.Program,"FullBright"),1);
    }
}

void World::DestroyWorld()
{
    for (int EntityIndex = 0; EntityIndex < this->EntityNode.size(); EntityIndex++)
    {
        this->EntityNode[EntityIndex].ModelShader.DestroyShader();
        this->EntityNode[EntityIndex].NewModel.Destroy();
    }
    for (int LightIndex = 0; LightIndex < this->LightEntityNode.size(); LightIndex++)
    {
        this->LightEntityNode[LightIndex].LightShader.DestroyShader();
        this->LightEntityNode[LightIndex].LightObject.Destroy();
    }
    this->EntityNode = {};
    this->LightEntityNode = {};
}