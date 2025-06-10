#ifndef __BOUNDING_BOX_HEADER__
#define __BOUNDING_BOX_HEADER__

#include <vector>
#include "../../../Dependancies/glm/glm.hpp"

class BoundingBox
{
    public:
        BoundingBox();
        bool BoundingCollisionsOnBounds(glm::vec3 AMinimum,glm::vec3 AMaximum,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix);
        bool BoundingCollisionOnPoint(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix);
        void GetBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix);
    private:
        glm::vec3 GetVertexPositions(std::vector<float> Vertices,size_t Index,int Stride);
        glm::vec3 Minimum;
        glm::vec3 Maximum;
};

#endif