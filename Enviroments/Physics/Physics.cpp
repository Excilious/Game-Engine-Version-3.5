#include "Physics.h"

Physics::Physics() {}

void Physics::GeneratePhysics(double DeltaTime)
{
    this->DeltaTime = DeltaTime;
}

void Physics::GeneratePhysicsData(ModelPhysics EntityPhysics,int NodeID)
{
    EntityPhysics.Acceleration.y = 9.81;
    EntityPhysics.Acceleration.x = 0;
    EntityPhysics.Acceleration.z = 0;

    EntityPhysics.NodeID        = NodeID;
    EntityPhysics.Velocity.z    = EntityPhysics.Velocity.z + EntityPhysics.Acceleration.z*this->DeltaTime;
    EntityPhysics.Velocity.y    = EntityPhysics.Velocity.y + EntityPhysics.Acceleration.y*this->DeltaTime;
    EntityPhysics.Velocity.x    = EntityPhysics.Velocity.x + EntityPhysics.Acceleration.x*this->DeltaTime;
    EntityPhysics.Mass          = EntityPhysics.Density/EntityPhysics.Volume;
}

glm::vec3 Physics::CheckNextPosition(glm::vec3 Position,ModelPhysics EntityPhysics)
{
    glm::vec3 NewPosition = Position;
    //Use S = UT + 1/2AT^2;
    NewPosition.x = EntityPhysics.Velocity.x*this->DeltaTime + 0.5*EntityPhysics.Acceleration.x*(this->DeltaTime*this->DeltaTime);
    NewPosition.y = EntityPhysics.Velocity.y*this->DeltaTime + 0.5*EntityPhysics.Acceleration.y*(this->DeltaTime*this->DeltaTime);
    NewPosition.z = EntityPhysics.Velocity.z*this->DeltaTime + 0.5*EntityPhysics.Acceleration.z*(this->DeltaTime*this->DeltaTime);
    return NewPosition;
}