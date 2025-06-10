#include "Collision.h"

Collision::Collision() {};

glm::vec3 Collision::GetVertexPosition(std::vector<float> Vertices,size_t Index)
{return glm::vec3(Vertices[Index*8+0],Vertices[Index*8+1],Vertices[Index*8+2]);}

glm::vec3 Collision::MatrixPosition(glm::vec3 Position,glm::mat4 Matrix)
{return glm::vec3(Matrix*glm::vec4(Position,1.0f));}

std::pair<glm::vec3,glm::vec3> Collision::GetMaximumMinimumBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Matrix)
{
    std::vector<glm::vec3> PositionalVerticesContainer;
    glm::vec3 MinVector = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 MaxVector = glm::vec3(std::numeric_limits<float>::lowest());
    for (int Index = 0; Index < Indices.size(); Index++)
    {
        glm::vec3 VertexA = glm::vec3(Matrix*glm::vec4(this->GetVertexPosition(Vertices,Indices[Index]),1.0f));
        MinVector = glm::min(MinVector,VertexA);
        MaxVector = glm::max(MaxVector,VertexA);
    }
    return std::make_pair(MinVector,MaxVector);
}

glm::vec3 Collision::GetPlayerBoundingBox(glm::vec3 Position,glm::vec3 Scale)
{return glm::vec3(Position.x-Scale.x,Position.y-Scale.y,Position.z-Scale.z),glm::vec3(Position.x+Scale.x,Position.y+Scale.y,Position.z+Scale.z);}

bool Collision::IsPointNearPlane(glm::vec3 Point,glm::vec3 PlanePoint,glm::vec3 PlaneNormal,float Threshold)
{return glm::abs(glm::dot(PlaneNormal,Point-PlanePoint)) < Threshold;}

bool Collision::GenerateCollisionBounding(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix)
{
    std::pair<glm::vec3,glm::vec3> BoundTuple = this->GetMaximumMinimumBounds(Vertices,Indices,ModelMatrix);
    if (Position.x > BoundTuple.first.x && Position.y < BoundTuple.second.x && Position.y > BoundTuple.first.y && Position.y < BoundTuple.second.y && Position.z > BoundTuple.first.z && Position.z < BoundTuple.second.z)
    {return true;}
    return false;
}

bool Collision::GeneratePlayerCollisionBounding(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix)
{
    glm::vec3 MinBounds,MaxBounds                   = this->GetPlayerBoundingBox(Position,glm::vec3(1,2,1));
    std::pair<glm::vec3,glm::vec3> BoundTuple       = this->GetMaximumMinimumBounds(Vertices,Indices,ModelMatrix);
    return (
        MinBounds.x <= BoundTuple.second.x &&
        MaxBounds.x >= BoundTuple.first.x &&
        MinBounds.y <= BoundTuple.second.y &&
        MaxBounds.y >= BoundTuple.first.y &&
        MinBounds.z <= BoundTuple.second.z &&
        MaxBounds.z >= BoundTuple.first.z
    );
}

bool Collision::GenerateCollisionPosition(glm::vec3 Position,std::vector<glm::vec3> VerticesPosition,glm::mat4 ModelMatrix)
{
    for (int Index = 0; Index < VerticesPosition.size(); Index+=3)
    {
        glm::vec3 Position0 = glm::vec3(ModelMatrix*glm::vec4(VerticesPosition[Index],1.0f));
        glm::vec3 Position1 = glm::vec3(ModelMatrix*glm::vec4(VerticesPosition[Index+1],1.0f));
        glm::vec3 Position2 = glm::vec3(ModelMatrix*glm::vec4(VerticesPosition[Index+2],1.0f));

        glm::vec3 Edge1 = Position1-Position0;
        glm::vec3 Edge2 = Position2-Position0;
        glm::vec3 Normal = glm::normalize(glm::cross(Edge1,Edge2));

        if (this->IsPointNearPlane(Position,Position0,Normal,0.01))
        {
            if (this->IsVectorInTriangle(Position,Position0,Position1,Position2))
            {return true;}
        }
    }
    return false;
}

bool Collision::GenerateCollisionPrecise(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,float Threshold)
{
    for (int Index = 0; Index < Indices.size(); Index += 3)
    {
        glm::vec3 Vertex0 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPosition(Vertices,Indices[Index]),1.0f));
        glm::vec3 Vertex1 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPosition(Vertices,Indices[Index+1]),1.0f));
        glm::vec3 Vertex2 = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPosition(Vertices,Indices[Index+2]),1.0f));

        glm::vec3 Edge1 = Vertex1-Vertex0;
        glm::vec3 Edge2 = Vertex2-Vertex0;
        glm::vec3 Normal = glm::normalize(glm::cross(Edge1,Edge2));

        if (this->IsPointNearPlane(Position,Vertex0,Normal,Threshold))
        {
            if (this->IsVectorInTriangle(Position,Vertex0,Vertex1,Vertex2))
            {return true;}
        }
    }
    return false;
}

bool Collision::IsVectorInTriangle(glm::vec3 Point,glm::vec3 Vertex0,glm::vec3 Vertex1,glm::vec3 Vertex2)
{
    glm::vec3 Vertex0Point = Point-Vertex0;
    glm::vec3 Vertex1Point = Point-Vertex1;
    glm::vec3 Vertex2Point = Point-Vertex2;

    glm::vec3 Normal0 = glm::cross(Vertex1-Vertex0,Vertex0Point);
    glm::vec3 Normal1 = glm::cross(Vertex2-Vertex1,Vertex1Point);
    glm::vec3 Normal2 = glm::cross(Vertex0-Vertex2,Vertex2Point);
    if (glm::dot(Normal0,Normal1) >= 0 && glm::dot(Normal0,Normal2) >= 0)
    {return true;}
    return false;
}