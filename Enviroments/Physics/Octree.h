#ifndef __OCTTREE_H__
#define __OECTREE_H__

#include <vector>
#include <memory>
#include "../../Dependancies/glm/glm.hpp"

struct AABB
{
    glm::vec3   Center;
    float       Half;

    AABB(glm::vec3 PointCenter, float PointHalf)
        : Center(PointCenter), Half(PointHalf) {}

    bool PointWithin(const glm::vec3& NewPoint) const {
        return (NewPoint.x >= Center.x - Half && NewPoint.x <= Center.x + Half &&
                NewPoint.y >= Center.y - Half && NewPoint.y <= Center.y + Half &&
                NewPoint.z >= Center.z - Half && NewPoint.z <= Center.z + Half);
    }

    bool Intersects(const AABB& Other) const {
        return (std::abs(Center.x - Other.Center.x) <= (Half + Other.Half) &&
                std::abs(Center.y - Other.Center.y) <= (Half + Other.Half) &&
                std::abs(Center.z - Other.Center.z) <= (Half + Other.Half));
    }
};

class Octree
{
    AABB                        Bounds;
    std::vector<glm::vec3>      Points;
    int                         Capacity;
    bool                        Divided;
    std::unique_ptr<Octree>     Children[8];

    public:
        Octree(AABB Boundary, int NewCapacity);
        void Generate(AABB Boundary, int NewCapacity);
        void Subdivide();
        void Query(const AABB& Range, std::vector<glm::vec3>& Found) const;
        bool Insert(const glm::vec3& Point);
};

#endif
