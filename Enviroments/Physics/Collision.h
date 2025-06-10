#ifndef __COLLISION_H__
#define __COLLISION_H__
#define GLM_ENABLE_EXPERIMENTAL

#ifndef COLLISION_DEBUG
    #define COLLISION_DEBUG 1
#endif

#include <vector>
#include <utility>
#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../Dependancies/glm/gtx/vector_angle.hpp"
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

typedef enum 
{
    COLLISION_PRECISE,
    COLLISION_BOUNDING_BOX,
} CollisionEnum;

class Collision
{
    public:
        Collision();
        bool        IsVectorInTriangle(glm::vec3 Point,glm::vec3 Vertex0,glm::vec3 Vertex1,glm::vec3 Vertex2);
        bool        GenerateCollisionPrecise(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix,float Threshold);
        bool        GenerateCollisionBounding(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix);
        bool        GeneratePlayerCollisionBounding(glm::vec3 Position,std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 ModelMatrix);
        bool        GenerateCollisionPosition(glm::vec3 Position,std::vector<glm::vec3> VerticesPosition,glm::mat4 ModelMatrix);
        glm::vec3   GetPlayerBoundingBox(glm::vec3 Position,glm::vec3 Scale);
        std::pair<glm::vec3,glm::vec3> GetMaximumMinimumBounds(std::vector<float> Vertices,std::vector<unsigned int> Indices,glm::mat4 Matrix);

    private:
        bool        IsPointNearPlane(glm::vec3 Point,glm::vec3 PlanePoint,glm::vec3 PlaneNormal,float Threshold);
        glm::vec3   GetVertexPosition(std::vector<float> Vertices,size_t Index);
        glm::vec3   MatrixPosition(glm::vec3 Position,glm::mat4 Matrix);

};

#endif