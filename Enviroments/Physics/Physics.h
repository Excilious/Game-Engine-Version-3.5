#ifndef __PHYSICS_H__
#define __PHYSICS_H__
#include "Collision.h"

typedef struct
{
    int Mass;
    int Density;
    int Volume;
    int NodeID;
    glm::vec3 Velocity;
    glm::vec3 Acceleration;
} ModelPhysics;

class Physics
{
    public:
        Physics();
        glm::vec3   CheckNextPosition(glm::vec3 Position,ModelPhysics EntityPhysics);
        void        GeneratePhysicsData(ModelPhysics EntityPhysicsData,int NodeID);
        void        GeneratePhysics(double DeltaTime);
    private:
        double      DeltaTime;
};

#endif