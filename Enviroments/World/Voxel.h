#ifndef __VOXEL_HEADER__
#define __VOXEL_HEADER__

#include <vector>
#include "../../Dependancies/glad/glad.h"
#include "../../Dependancies/GLFW/glfw3.h"
#include "../../Dependancies/glm/glm.hpp"
#include "../../Dependancies/glm/gtc/type_ptr.hpp"
#define MAXSIZE 512

struct VoxelVertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Texture;
};

class Voxel
{
    public:
        Voxel();
        void GenerateMesh();
        float HeightMap[MAXSIZE][MAXSIZE];
        std::vector<VoxelVertex>    Vertices;
        std::vector<unsigned int>   Indices;
    private:
};

#endif