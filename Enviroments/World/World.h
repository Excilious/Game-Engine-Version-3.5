#ifndef __WORLD_H__
#define __WORLD_H__

#ifndef WORLD_DEBUG
    #define WORLD_DEBUG 1
#endif

#include <vector>
#include <string>

#include "Voxel.h"
#include "Lighting.h"
#include "Water.h"
#include "Terrain.h"
#include "../Physics/Octree.h"
#include "../Entity/Actor.h"
#include "../Dependancies/jsonimport/json.h"
#include "../Physics/Physics.h"
#include "../Model.h"
#include "../Developer.h"

typedef struct
{
    Bounds          DeveloperBounds;
    Model           NewModel;
    Shader          ModelShader;
    CollisionEnum   CollisionType;
    ModelPhysics    EntityPhysics;
    bool            EnableGravity;
    int             NodeID;
    glm::vec3       Maximum;
    glm::vec3       Minimum;
} ModelNode;

typedef struct
{
    Bounds          DeveloperBounds;
    Actor           NewActorInstance;
    Shader          ShaderActor;
    CollisionEnum   CollisionType;
    glm::vec3       Maximum;
    glm::vec3       Minimum;
} ActorNode;

typedef struct
{
    Lighting        LightObject;
    Shader          LightShader;
} LightNode;

class World
{
    public:
        World();
        void SetWorldData(const char* LevelDirectory,double DeltaTime,float Near,float Far,float FieldOfView,Camera EngineCamera);
        void RenderWorld(Camera* EngineCamera);
        void GeneratePlayer(std::string FileDirectory,glm::vec3 CharacterPosition,glm::quat CharacterRotation,glm::vec3 CharacterScale,Camera EngineCamera);
        void DestroyWorld();
        bool CheckPlayerCollision(glm::vec3 Minimum,glm::vec3 Maximum);

        std::vector<ModelNode>      EntityNode;
        std::vector<LightNode>      LightEntityNode;
        std::vector<ActorNode>      CharacterNode;
        std::vector<Bounds>         TriangleBoundsRendering;
        ModelNode                   WorldNode;
        ActorNode                   PlayerNode;
        Voxel                       SceneVoxel;

    private:
        void ApplyObjectLight(unsigned int ShaderProgram);
        void GenerateCharacterEntity(std::string FileDirectory,glm::vec3 CharacterPosition,glm::quat CharacterRotation,glm::vec3 CharacterScale,Camera EngineCamera);
        void GenerateLightEntity(std::string FileDirectory,glm::vec3 LightPosition,glm::quat LightRotation,glm::vec3 LightScale,glm::vec3 Colour,float Brightness,float Gloss,float Transparency);
        void GenerateScene();

        float                       Near;
        float                       Far;
        float                       FieldOfView;

        double                      DeltaTime;
        int                         AnimationDeltaTime;
        Terrain                     WorldTerrain;
        Collision                   WorldCollider;
        Physics                     WorldPhysics;
        Json                        WorldContents;  
        std::string                 SceneDirectory;
        std::string                 Contents;

        Bounds                      TestBounds;
        AABB                        *PlayerCollisionBox;
        Octree                      *NewOctree;
        std::vector<glm::vec3>      VectorFound;

        Shader                      WorldShaderA;
        Shader                      WorldShaderB;
};

#endif