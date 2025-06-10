#include "Model.h"
#include "Console.h"

Model::Model() {}

void Model::CreateModel(const char* NewFile,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,float Near,float Far,float FieldOfView,bool UseRawData,bool UseModelMatrix,glm::mat4 UseMatrix)
{
    std::string WholeString = NewFile;
    SysPrint("Rendering "+WholeString,1);
    std::string NewText     = GetShaderContents(NewFile);
    this->JSON              = Json::parse(NewText);
    this->NewFile           = NewFile;
    this->Position          = Position;
    this->Rotation          = Rotation;
    this->Scale             = Scale;
    this->NewData           = GetData();
    this->Near              = Near;
    this->Far               = Far;
    this->UseRawData        = UseRawData;
    this->UseModelMatrix    = UseModelMatrix;
    this->FieldOfView       = FieldOfView;
    this->Matrix            = UseMatrix;

    glm::mat4 TranslationM = glm::mat4(1.0f);
    glm::mat4 RotationM = glm::mat4(1.0f);
    glm::mat4 ScaleM = glm::mat4(1.0f);

    TranslationM = glm::translate(TranslationM,Position);
    RotationM = glm::mat4_cast(Rotation);
    ScaleM = glm::scale(ScaleM,Scale);

    this->TraverseNode(0);
    this->InstanceMatrix    = TranslationM * RotationM * ScaleM;
}

void Model::Draw(Shader* NewShader,Camera* NewCamera)
{for (unsigned int x = 0; x < this->NewMeshes.size(); x++) {this->NewMeshes[x].Mesh::Draw(NewShader,NewCamera,this->UseRawData,this->InstanceMatrix,this->FieldOfView,this->Near,this->Far,this->Position,this->Rotation,this->Scale,this->UseModelMatrix,this->Matrix);}}

void Model::TraverseNode(unsigned int NextNode, glm::mat4 Matrix)
{
    Json Node                   = this->JSON["nodes"][NextNode];
    this->PredefinedMatrix      = Matrix;

    if (Node.find("mesh") != Node.end())
    {
        TranslationsMeshes.push_back(this->Position);
        RotationMeshes.push_back(this->Rotation);
        ScaleMeshes.push_back(this->Scale);
        MatricesMeshes.push_back(this->PredefinedMatrix);
        this->LoadMesh(Node["mesh"]);
    }

    if (Node.find("children") != Node.end())
    {for (unsigned int x = 0; x < Node["children"].size(); x++) {TraverseNode(Node["children"][x],glm::mat4(1.0f));}}
}

void Model::LoadMesh(unsigned int IndicesMesh)
{
    unsigned int PositionAccessorIndices    = this->JSON["meshes"][IndicesMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int NormalAccessorIndices      = this->JSON["meshes"][IndicesMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int TextureAccessorIndices     = this->JSON["meshes"][IndicesMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int IndicesAccessorIndices     = this->JSON["meshes"][IndicesMesh]["primitives"][0]["indices"];

    std::vector<float> PositionVector   = this->GetFloats(this->JSON["accessors"][PositionAccessorIndices]);
    std::vector<glm::vec3> Positions    = this->FloatsGroupVec3(PositionVector);
    std::vector<float> NormalVector     = this->GetFloats(this->JSON["accessors"][NormalAccessorIndices]);
    std::vector<glm::vec3> Normals      = this->FloatsGroupVec3(NormalVector);
    std::vector<float> TextureVector    = this->GetFloats(this->JSON["accessors"][TextureAccessorIndices]);
    std::vector<glm::vec2> TextureUV    = this->FloatsGroupVec2(TextureVector);

    std::vector<Vertex> Vertices    = this->AssembleVertices(Positions,Normals,TextureUV);
    std::vector<GLuint> Indices     = this->GetIndices(this->JSON["accessors"][IndicesAccessorIndices]);
    std::vector<Texture> Textures   = this->GetTextures();

    this->PositionCollision   = Positions;
    this->VerticesCollision   = PositionVector;
    this->IndicesCollision    = Indices;
    this->NewMeshes.push_back(Mesh(Vertices,Indices,Textures));
}

void Model::Destroy()
{for (unsigned int x = 0; x < this->NewMeshes.size(); x++) {this->NewMeshes[x].Mesh::Destroy();}}

std::vector<unsigned char> Model::GetData()
{
    std::string ByteText;
    std::string BufferURI       = this->JSON["buffers"][0]["uri"];

    std::string FileString      = std::string(NewFile);
    std::string FileDirectory   = FileString.substr(0,FileString.find_last_of('/')+1);
    ByteText = GetShaderContents((FileDirectory+BufferURI).c_str());
    
    std::vector<unsigned char> NewData(ByteText.begin(),ByteText.end());
    return NewData;
}

std::vector<float> Model::GetFloats(Json Accessor)
{
    std::vector<float> FloatVector;

    unsigned int BufferViewIndex    = Accessor.value("bufferView",1);
    unsigned int Count              = Accessor["count"];
    unsigned int AccessorByteOffset = Accessor.value("byteOffset",0);

    std::string Type        = Accessor["type"];
    Json BufferView         = this->JSON["bufferViews"][BufferViewIndex];
    unsigned int ByteOffset = BufferView["byteOffset"];

    unsigned int NumberPerVertices;
    if (Type == "SCALAR") NumberPerVertices = 1;
    else if (Type == "VEC2") NumberPerVertices = 2;
    else if (Type == "VEC3") NumberPerVertices = 3;
    else if (Type == "VEC4") NumberPerVertices = 4;
    else SysPrint("Vector doesn't exist.",2);

    unsigned int StartOfData    = ByteOffset + AccessorByteOffset;
    unsigned int LengthOfData   = Count * 4 * NumberPerVertices;
    for (unsigned int x = StartOfData; x < StartOfData + LengthOfData; x)
    {
        unsigned char Bytes[] = {NewData[x++],NewData[x++],NewData[x++],NewData[x++]};
        float Value;
        std::memcpy(&Value,Bytes,sizeof(float));
        FloatVector.push_back(Value);
    }
    return FloatVector;
}

std::vector<GLuint> Model::GetIndices(Json Accessor)
{
    std::vector<GLuint> Indices;

    unsigned int BufferViewIndices  = Accessor.value("bufferView",0);
    unsigned int Count              = Accessor["count"];
    unsigned int AccessorByteOffset = Accessor.value("byteOffset",0);
    unsigned int ComponentType      = Accessor["componentType"];

    Json BufferView             = this->JSON["bufferViews"][BufferViewIndices];
    unsigned int ByteOffet      = BufferView["byteOffset"];
    unsigned int StartOfData    = ByteOffet + AccessorByteOffset;
    
    if (ComponentType == 5125)
    {
        for (unsigned int x = StartOfData; x < ByteOffet + AccessorByteOffset + Count * 4;x)
        {
            unsigned char Bytes[] = {NewData[x++],NewData[x++],NewData[x++],NewData[x++]};
            unsigned int Value;
            std::memcpy(&Value,Bytes,sizeof(unsigned int));
            Indices.push_back((GLuint)Value);
        }
    }
    else if (ComponentType == 5123)
    {
        for (unsigned int x = StartOfData; x < ByteOffet + AccessorByteOffset + Count * 2; x)
        {
          unsigned char Bytes[] = {NewData[x++],NewData[x++]};
          unsigned short Value;
          std::memcpy(&Value,Bytes,sizeof(unsigned short));
          Indices.push_back((GLuint)Value);

        }
    }
    else if (ComponentType == 5122)
    {
        for (unsigned int x = StartOfData; x < ByteOffet + AccessorByteOffset + Count * 2; x)
        {
            unsigned char Bytes[] = {NewData[x++],NewData[x++]};
            short Value;
            std::memcpy(&Value,Bytes,sizeof(short));
            Indices.push_back((GLuint)Value);
        }
    }
    return Indices;
}

std::vector<Texture> Model::GetTextures()
{
    std::vector<Texture> Textures;
    std::string FileString      = std::string(this->NewFile);
    std::string FileDirectory   = FileString.substr(0,FileString.find_last_of('/')+1);

    for (unsigned int x = 0; x < this->JSON["images"].size(); x++)
    {
        std::string TexturePath = this->JSON["images"][x]["uri"];
        bool Skip = false;
        for (unsigned int y = 0; y < LoadedTextureName.size(); y++)
        {
            if (LoadedTextureName[y] == TexturePath)
            {
                Textures.push_back(LoadedTexture[y]);
                Skip = true;
                break;
            }
        }

        if (!Skip)
        {
            if (TexturePath.find("baseColor") != std::string::npos)
            {
                Texture Diffuse;
                Diffuse.CreateNormalTexture((FileDirectory+TexturePath).c_str(),"diffuse",LoadedTexture.size());
                Textures.push_back(Diffuse);
                LoadedTexture.push_back(Diffuse);
            }
            else if (TexturePath.find("metallicRoughness") != std::string::npos)
            {
                Texture Specular;
                Specular.CreateNormalTexture((FileDirectory+TexturePath).c_str(),"specular",LoadedTexture.size());
                Textures.push_back(Specular);
                LoadedTexture.push_back(Specular);
            }
            LoadedTextureName.push_back(TexturePath);
        }
    }
    return Textures;
}

std::vector<Vertex> Model::AssembleVertices(std::vector<glm::vec3> Position, std::vector<glm::vec3> Normal, std::vector<glm::vec2> TextureUV)
{
    std::vector<Vertex> Vertices;
    for (int x = 0; x < Position.size(); x++)
    {Vertices.push_back (Vertex{Position[x],Normal[x],glm::vec3(1.0f,1.0f,1.0f),TextureUV[x]});}
    return Vertices;
}

std::vector<glm::vec2> Model::FloatsGroupVec2(std::vector<float> FloatVector)
{
    std::vector<glm::vec2> Vectors;
    for (int x = 0; x < FloatVector.size(); x)
    {Vectors.push_back(glm::vec2(FloatVector[x++],FloatVector[x++]));}
    return Vectors;
}

std::vector<glm::vec3> Model::FloatsGroupVec3(std::vector<float> FloatVector)
{
    std::vector<glm::vec3> Vectors;
    for (int x = 0; x < FloatVector.size(); x)
    {Vectors.push_back(glm::vec3(FloatVector[x++],FloatVector[x++],FloatVector[x++]));}
    return Vectors;
}

std::vector<glm::vec4> Model::FloatsGroupVec4(std::vector<float> FloatVector)
{
    std::vector<glm::vec4> Vectors;
    for (int x = 0; x < FloatVector.size(); x)
    {Vectors.push_back(glm::vec4(FloatVector[x++],FloatVector[x++],FloatVector[x++],FloatVector[x++]));}
    return Vectors;
}