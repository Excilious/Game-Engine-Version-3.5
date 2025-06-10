#include "Voxel.h"

Voxel::Voxel() {}

void Voxel::GenerateMesh()
{
    for (int IndexX = 0; IndexX < MAXSIZE; IndexX++)
    {
        for (int IndexZ = 0; IndexZ < MAXSIZE; IndexZ++)
        {
            float Y = HeightMap[IndexX][IndexZ];
            VoxelVertex NewVertex;
            NewVertex.Position      = glm::vec3(IndexX,Y,IndexZ);
            NewVertex.Normal        = glm::vec3(0.0f,1.0f,0.0f);
            NewVertex.Texture.x     = (float)IndexX/MAXSIZE;
            NewVertex.Texture.y     = (float)IndexZ/MAXSIZE;
            this->Vertices.push_back(NewVertex);
        }
    }

    for (int IndexX = 0; IndexX < MAXSIZE; IndexX++)
    {
        for (int IndexZ = 0; IndexZ < MAXSIZE; IndexZ++)
        {
            int TopLeft     = IndexX*MAXSIZE+IndexZ;
            int TopRight    = (IndexX+1)*MAXSIZE+IndexZ;
            int BottomLeft  = IndexX*MAXSIZE+(IndexZ+1);
            int BottomRight = (IndexX+1)*MAXSIZE+(IndexZ+1);

            this->Indices.push_back(TopLeft);
            this->Indices.push_back(BottomLeft);
            this->Indices.push_back(TopRight);
            this->Indices.push_back(TopRight);
            this->Indices.push_back(BottomLeft);
            this->Indices.push_back(BottomRight);
        }
    }
}