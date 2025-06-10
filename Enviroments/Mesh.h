#ifndef __MESH_HEADER__
#define __MESH_HEADER__

#include <string>
#include "Buffers/VertexArray.h"
#include "Buffers/ElementBuffer.h"
#include "Camera.h"
#include "Texture.h"

class Mesh
{
    public:
        Mesh(std::vector<Vertex>& Vertices,std::vector<GLuint>& Indices,std::vector<Texture>& Textures);
        void Draw(Shader* ShaderInstance,Camera* CameraInstance,bool UseRawData,glm::mat4 PredefinedModelMatrix,float FieldOfView,float Near,float Far,glm::vec3 Translation = glm::vec3(0.0f,0.0f,0.0f),glm::quat Rotation = glm::quat(0.0f,0.0f,0.0f,1.0f),glm::vec3 Scale = glm::vec3(1.0f,1.0f,1.0f),bool UseMatrix = false,glm::mat4 Matrix = glm::mat4(1.0f));
        void Destroy();
    private:
        VertexArrayObject       VertexArray;
        std::vector <Vertex>    Vertices;
        std::vector <GLuint>    Indices;
        std::vector <Texture>   Textures;
};

#endif