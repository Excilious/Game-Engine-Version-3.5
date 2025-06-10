#ifndef __TERRAIN_H__
#define __TERRAIN_H__
#define GLM_ENABLE_EXPERIMENTAL

#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../Dependancies/glm/gtx/vector_angle.hpp"
#include "../Buffers/PlaneBuffer.h"

#include <random>
#include <vector>

class Terrain
{
    public:
        Terrain();
        void BuildTerrain(int GridLength);
        void UpdateTilePosition(glm::vec3 CameraPosition);
        void Draw(glm::vec3 CameraPosition,glm::mat4 CameraMatrix,unsigned int ShaderA,unsigned int ShaderB);

        float Up = 0.0f;
    private:
        static const int TileWidth = 10.0f*100.0f;
        PlaneBuffer Buffer;
        glm::mat4   ModelMatrix;
        glm::vec3   RockColour;
        std::vector<glm::vec2> PositionVector;

        float   Octaves;
        float   Frequency;
        float   GrassCoverage;
        float   Mutliplier;
        float   Displacement;
        float   FogFallout;
        float   Power;

        int     GridLength;
        int     Resolution;

        bool DrawFog = false;
        unsigned int VertexBuffer;
        unsigned int VertexArray;
        unsigned int ElementBuffer;
        unsigned int *Textures;
        unsigned int *PositionBuffer;

        glm::vec3 GenerateRandomSeed();
        bool CheckIfInTile(glm::vec3 CameraPosition,glm::vec2 Position);
        bool TileCameraPosition(glm::vec3 CameraPosition,glm::vec2 &Position);
        void DrawVertices(int Instances,unsigned int ShaderA,unsigned int ShaderB);
        void GenerateTileGrid(glm::vec2 Offset);
        void SetPositionArray(std::vector<glm::vec2> &Position);
};

#endif