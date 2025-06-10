#include "Octree.h"
#include <iostream>

Octree::Octree(AABB Boundary, int NewCapacity)
    : Bounds(Boundary), Capacity(NewCapacity), Divided(false) 
{
    for (int i = 0; i < 8; i++)
        this->Children[i] = nullptr;
}

void Octree::Generate(AABB Boundary, int NewCapacity)
{
    this->Bounds = Boundary;
    this->Capacity = NewCapacity;
    this->Divided = false;
    this->Points.clear();
    for (int i = 0; i < 8; i++)
        this->Children[i] = nullptr;
}

void Octree::Subdivide()
{
    float X = this->Bounds.Center.x;
    float Y = this->Bounds.Center.y;
    float Z = this->Bounds.Center.z;
    float H = this->Bounds.Half / 2.0f;

    this->Children[0] = std::make_unique<Octree>(AABB(glm::vec3(X - H, Y - H, Z - H), H), Capacity);
    this->Children[1] = std::make_unique<Octree>(AABB(glm::vec3(X + H, Y - H, Z - H), H), Capacity);
    this->Children[2] = std::make_unique<Octree>(AABB(glm::vec3(X - H, Y + H, Z - H), H), Capacity);
    this->Children[3] = std::make_unique<Octree>(AABB(glm::vec3(X + H, Y + H, Z - H), H), Capacity);
    this->Children[4] = std::make_unique<Octree>(AABB(glm::vec3(X - H, Y - H, Z + H), H), Capacity);
    this->Children[5] = std::make_unique<Octree>(AABB(glm::vec3(X + H, Y - H, Z + H), H), Capacity);
    this->Children[6] = std::make_unique<Octree>(AABB(glm::vec3(X - H, Y + H, Z + H), H), Capacity);
    this->Children[7] = std::make_unique<Octree>(AABB(glm::vec3(X + H, Y + H, Z + H), H), Capacity);
    this->Divided = true;
}

bool Octree::Insert(const glm::vec3& Point)
{
    if (!this->Bounds.PointWithin(Point))
        return false;

    if (this->Points.size() < Capacity)
    {
        this->Points.push_back(Point);
        return true;
    }

    if (!this->Divided)
        this->Subdivide();

    for (int Index = 0; Index < 8; Index++)
    {
        if (this->Children[Index]->Insert(Point))
            return true;
    }

    return false;
}

void Octree::Query(const AABB& Range, std::vector<glm::vec3>& Found) const
{
    for (const auto& Point : this->Points)
    {
        if (Range.PointWithin(Point))
        {
            Found.push_back(Point);
        }
    }

    if (Divided)
    {
        for (int Index = 0; Index < 8; Index++)
        {
            if (this->Children[Index])
            {
                std::cout << "Quering" << "\n";
                this->Children[Index]->Query(Range, Found);
            }
        }
    }
}
