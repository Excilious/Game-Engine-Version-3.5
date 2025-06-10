#ifndef __MODEL_HEADER__
#define __MODEL_HEADER__

#include "../Dependancies/jsonimport/json.h"
#include "Mesh.h"

using Json = nlohmann::json;

typedef struct
{
    glm::vec3 TransformedPosition;
    glm::vec3 TransformedScale;
    glm::quat TransformedRotation;
} MeshGeometry;

class Model
{
    public:
        Model();
        void Draw(Shader* NewShader,Camera* NewCamera);
        void Destroy();
        void CreateModel(const char* NewFile,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,float Near,float Far,float FieldOfView,bool UseRawData=false,bool UseModelMatrix=false,glm::mat4 UseMatrix = glm::mat4(1.0f));

        std::vector<glm::vec3>          PositionCollision;
        std::vector<float>              VerticesCollision;
        std::vector<unsigned>           IndicesCollision;
        std::vector<Mesh>               NewMeshes;
        std::vector<MeshGeometry>       GeometryMatrix;
        glm::mat4                       InstanceMatrix;
        glm::mat4                       PredefinedMatrix;
        glm::mat4                       Matrix;

        glm::vec3                       Position;
        glm::vec3                       Scale;
        glm::quat                       Rotation;
        const char*                     NewFile;

        std::vector<glm::vec3>          TranslationsMeshes;
        std::vector<glm::quat>          RotationMeshes;
        std::vector<glm::vec3>          ScaleMeshes;
    private:
        std::vector<unsigned char>  NewData;
        Json                        JSON;

        std::vector<glm::mat4>      MatricesMeshes;        
        std::vector<std::string>    LoadedTextureName;
        std::vector<Texture>        LoadedTexture;
        
        void LoadMesh(unsigned int IndicesMesh);
        void TraverseNode(unsigned int NextNode, glm::mat4 Matrix = glm::mat4(1.0f));
        
        std::vector<unsigned char>  GetData();
        std::vector<float>          GetFloats(Json Accessor);
        std::vector<GLuint>         GetIndices(Json Accessor);
        std::vector<Texture>        GetTextures();

        std::vector<Vertex> AssembleVertices
        (
            std::vector<glm::vec3> Position,
            std::vector<glm::vec3> Normal,
            std::vector<glm::vec2> TextureUV
        );

        std::vector<glm::vec2> FloatsGroupVec2(std::vector<float> FloatVector);
        std::vector<glm::vec3> FloatsGroupVec3(std::vector<float> FloatVector);
        std::vector<glm::vec4> FloatsGroupVec4(std::vector<float> FloatVector);

        float Near;
        float Far;
        float FieldOfView;
        bool  UseRawData;
        bool  UseModelMatrix;
};

#endif