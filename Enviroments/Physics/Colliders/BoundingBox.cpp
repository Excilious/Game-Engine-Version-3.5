#include "BoundingBox.h"

BoundingBox::BoundingBox() {}

glm::vec3 BoundingBox::GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride)
{return glm::vec3(Vertices[Index*Stride],Vertices[(Index*Stride)+1],Vertices[(Index*Stride)+2]);}

void BoundingBox::GetBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix)
{
    std::vector<glm::vec3> VerticesContainer;
    this->Minimum = glm::vec3(std::numeric_limits<float>::lowest());
    this->Maximum = glm::vec3(std::numeric_limits<float>::max());

    for (int Index = 0; Index < Indices.size(); Index++)
    {
        glm::vec3 Vertex = glm::vec3(ModelMatrix*glm::vec4(this->GetVertexPositions(Vertices,Index,8),1.0f));
        VerticesContainer.push_back(Vertex);
    }
    for (int VertexIndex = 0; VertexIndex < VerticesContainer.size(); VertexIndex++)
    {
        this->Minimum = glm::min(this->Minimum,VerticesContainer[VertexIndex]);
        this->Maximum = glm::max(this->Maximum,VerticesContainer[VertexIndex]);
    }
}

bool BoundingBox::BoundingCollisionOnPoint(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix)
{
    this->GetBounds(Vertices,Indices,ModelMatrix);
    return (
        Position.x >= this->Minimum.x &&
        Position.x <= this->Maximum.x &&
        Position.y >= this->Minimum.y && 
        Position.y <= this->Maximum.y &&
        Position.z >= this->Minimum.z &&
        Position.z <= this->Maximum.z
    );
}

bool BoundingBox::BoundingCollisionsOnBounds(glm::vec3 AMinimum,glm::vec3 AMaximum,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix)
{
    this->GetBounds(Vertices,Indices,ModelMatrix);
    return (
        AMinimum.x <= this->Maximum.x &&
        AMaximum.x >= this->Minimum.x &&
        AMinimum.y <= this->Maximum.y &&
        AMaximum.y >= this->Minimum.y &&
        AMinimum.z <= this->Maximum.z &&
        AMaximum.z >= this->Minimum.z
    );
}