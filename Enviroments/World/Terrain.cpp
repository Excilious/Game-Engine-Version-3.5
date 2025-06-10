#include "Terrain.h"
#include "../Texture.h"

Terrain::Terrain() {}

glm::vec3 Terrain::GenerateRandomSeed()
{
    std::random_device RandomDevice;
    std::mt19937 Generate(RandomDevice());
    std::uniform_real_distribution<> Distribution(0.0f,100.0f);
    float X,Y,Z;
    X = Distribution(Generate);
    Y = Distribution(Generate);
    Z = Distribution(Generate);
    return glm::vec3(X,Y,Z);
}

bool Terrain::CheckIfInTile(glm::vec3 CameraPosition,glm::vec2 Position)
{
    float CameraPositionX = CameraPosition.x;
    float CameraPositionY = CameraPosition.z;
    float PositionX = Position.x;
    float PositionY = Position.y;
    bool IsInX = false;
    bool IsInY = false;

    if ((CameraPositionX <= PositionX+1.0f*this->TileWidth/2.0f) && (CameraPositionX >= PositionX-1.0f*this->TileWidth/2.0f)) {IsInX = true;}
    if ((CameraPositionY <= PositionY+1.0f*this->TileWidth/2.0f) && (CameraPositionY >= PositionY-1.0f*this->TileWidth/2.0f)) {IsInY = true;}

    return (IsInX && IsInY);
}

bool Terrain::TileCameraPosition(glm::vec3 CameraPosition,glm::vec2 &Position)
{
    for (int PositionIndex = 0; PositionIndex < this->PositionVector.size(); PositionIndex++)
    {
        if (this->CheckIfInTile(CameraPosition,this->PositionVector[PositionIndex]))
        {
            Position = this->PositionVector[PositionIndex];
            return true;
        }
    }
    return false;
}

void Terrain::BuildTerrain(int GridLength)
{
    glm::mat4 MatrixID;
    glm::mat4 ScaleMatrix       = glm::scale(MatrixID,glm::vec3(1.0f,0.0f,1.0f));
    glm::mat4 PositionMatrix    = glm::translate(MatrixID,glm::vec3(0.0f,0.0f,0.0f));
    this->ModelMatrix = PositionMatrix;

    this->Octaves           = 13.0f;
    this->Frequency         = 0.01f;
    this->GrassCoverage     = 0.65f;
    this->Mutliplier        = 1.0f;
    this->Displacement      = 20.0f;
    this->FogFallout        = 1.5f;
    this->PositionBuffer    = 0;

    Texture WorldTexture;

    this->GridLength = GridLength+(GridLength+1)%2;
    this->Resolution = 4;

    this->Buffer.CreatePlaneBuffer(this->Resolution,this->TileWidth,&this->VertexArray,&this->VertexBuffer,&this->ElementBuffer);
    this->Textures = new unsigned int[6];

    this->Textures[0] = WorldTexture.GetTextureFromFile("../../Textures/World/Sand.jpg",false);
    this->Textures[1] = WorldTexture.GetTextureFromFile("../../Textures/World/Grass.jpg",false);
    this->Textures[2] = WorldTexture.GetTextureFromFile("../../Textures/World/RDiffuse.jpg",false);
    this->Textures[3] = WorldTexture.GetTextureFromFile("../../Textures/World/Snow2.jpg",false);
    this->Textures[4] = WorldTexture.GetTextureFromFile("../../Textures/World/RNormal.jpg",false);
    this->Textures[5] = WorldTexture.GetTextureFromFile("../../Textures/World/TextureTerrain.jpg",false);

    this->PositionVector.resize(this->GridLength*this->GridLength);
    this->GenerateTileGrid(glm::vec2(0.0f,0.0f));
    this->SetPositionArray(this->PositionVector);

    this->RockColour    = glm::vec4(120,105,75,255)*1.5f/255.0f;
    this->Power         = 3.0f;
}

void Terrain::Draw(glm::vec3 CameraPosition,glm::mat4 CameraMatrix,unsigned int ShaderA,unsigned int ShaderB)
{
    if (this->Up != 0.0f) {glEnable(GL_CLIP_DISTANCE0);}
    glm::mat4 WorldMatrix   = this->ModelMatrix;
    glm::mat4 ViewProj      = CameraMatrix;

    glm::vec4 PlaneClipping     = glm::vec4(0.0f,1.0f,0.0f,-100.0f);
    glm::vec3 Seed              = this->GenerateRandomSeed();
    std::vector<unsigned int> Shaders;
    Shaders.push_back(ShaderA);
    Shaders.push_back(ShaderB);
    
    for (int ShaderIndex = 0; ShaderIndex < Shaders.size(); ShaderIndex++)
    {
        glUseProgram(Shaders[ShaderIndex]);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"gEyeWorldPos"),CameraPosition.x,CameraPosition.y,CameraPosition.z);
        glUniformMatrix4fv(glGetUniformLocation(Shaders[ShaderIndex],"gWorld"),1,GL_FALSE,glm::value_ptr(WorldMatrix));
        glUniformMatrix4fv(glGetUniformLocation(Shaders[ShaderIndex],"gVP"),1,GL_FALSE,glm::value_ptr(ViewProj));
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"gDispFactor"),this->Displacement);

        glUniform4f(glGetUniformLocation(Shaders[ShaderIndex],"clipPlane"),PlaneClipping.x*Up,PlaneClipping.y*Up,PlaneClipping.z*Up,PlaneClipping.w*Up);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"u_LightColor"),0.0f,1.0f,0.0f);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"u_LightPosition"),0.0f,0.0f,0.0f);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"u_ViewPosition"),CameraPosition.x,CameraPosition.y,CameraPosition.z);

        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"fogColor"),1.0f,1.0f,1.0f);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"rockColor"),this->RockColour.x,this->RockColour.y,this->RockColour.z);
        glUniform3f(glGetUniformLocation(Shaders[ShaderIndex],"seed"),Seed.x,Seed.y,Seed.z);

        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"octaves"),this->Octaves);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"freq"),this->Frequency);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"u_grassCoverage"),this->GrassCoverage);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"waterHeight"),100.0f);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"tessMultiplier"),this->Mutliplier);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"fogFalloff"),this->FogFallout*1.e-6);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"power"),this->Power);

        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"normals"),1);
        glUniform1f(glGetUniformLocation(Shaders[ShaderIndex],"drawFog"),this->DrawFog);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,this->Textures[0]); 
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"sand"),1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,this->Textures[1]); 
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"grass"),2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,this->Textures[2]); 
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"rock"),3);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,this->Textures[3]); 
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"snow"),4);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D,this->Textures[5]);
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"grass1"),5);

        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D,this->Textures[4]);
        glUniform1i(glGetUniformLocation(Shaders[ShaderIndex],"rockNormal"),6);
    }

    int Indices = this->PositionVector.size();
    this->DrawVertices(Indices,ShaderA,ShaderB);

    glDisable(GL_CLIP_DISTANCE0);
    this->Up = 0.0f;
}

void Terrain::DrawVertices(int Instances,unsigned int ShaderA,unsigned int ShaderB)
{
    glBindVertexArray(this->VertexArray);
    glUseProgram(ShaderA);
    glUseProgram(ShaderB);
    glDrawElementsInstanced(0x000E,(this->Resolution-1)*(this->Resolution-1)*2*3,GL_UNSIGNED_INT,0,Instances);
    glBindVertexArray(0);
}

void Terrain::UpdateTilePosition(glm::vec3 CameraPosition)
{
    glm::vec2 CurrentTiles;
    glm::vec2 CameraPosition2D = glm::vec2(CameraPosition.x,CameraPosition.z);
    int TileIndex   = -1;
    int TileCount   = 0;

    if (this->TileCameraPosition(CameraPosition,CurrentTiles))
    {
        glm::vec2 Center = this->PositionVector[this->GridLength/2+this->GridLength/2*this->GridLength];
        for (int PositionIndex = 0; PositionIndex < this->PositionVector.size(); PositionIndex++)
        {this->PositionVector[PositionIndex] += CurrentTiles - Center;}
        this->SetPositionArray(this->PositionVector);
    }

}

void Terrain::GenerateTileGrid(glm::vec2 Offset)
{
    float Screen        = this->TileWidth;
    glm::vec2 IndexA    = glm::vec2(1.0f,0.0f)*Screen;
    glm::vec2 IndexB    = glm::vec2(0.0f,1.0f)*Screen;

    for (int IndexX = 0; IndexX < this->GridLength; IndexX++)
    {
        for (int IndexY = 0; IndexY < this->GridLength; IndexY++)
        {
            glm::vec2 Position = (float)(IndexY-this->GridLength/2)*glm::vec2(IndexA)+(float)(IndexX-this->GridLength/2)*glm::vec2(IndexB);
            this->PositionVector[IndexY+IndexX*this->GridLength] = Position+Offset;
        }
    }
}

void Terrain::SetPositionArray(std::vector<glm::vec2> &Position)
{
    if (this->PositionBuffer)
    {
        glDeleteBuffers(1,this->PositionBuffer);
        PositionBuffer = 0;
    }

    glGenBuffers(1,this->PositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,*this->PositionBuffer);
    glBufferData(GL_ARRAY_BUFFER,Position.size()*sizeof(glm::vec2),&Position[0],GL_STATIC_DRAW);

    glBindVertexArray(this->VertexArray);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec2),(void*)0);
    glVertexAttribDivisor(3,1);
    glBindVertexArray(0);
}