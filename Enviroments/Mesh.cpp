#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& Vertices,std::vector<GLuint>& Indices,std::vector<Texture>& Textures)
{
    this->Vertices  = Vertices;
    this->Indices   = Indices;
    this->Textures  = Textures;

    this->VertexArray.Bind();
    VertexBufferObject VertexBuffer(Vertices);
    ElementBufferObject ElementBuffer(Indices);

    this->VertexArray.LinkAttribute(VertexBuffer,0,3,GL_FLOAT,sizeof(Vertex),(void*)0);
    this->VertexArray.LinkAttribute(VertexBuffer,1,3,GL_FLOAT,sizeof(Vertex),(void*)(3*sizeof(float)));
    this->VertexArray.LinkAttribute(VertexBuffer,2,3,GL_FLOAT,sizeof(Vertex),(void*)(6*sizeof(float)));
    this->VertexArray.LinkAttribute(VertexBuffer,3,2,GL_FLOAT,sizeof(Vertex),(void*)(9*sizeof(float)));

    this->VertexArray.Unbind();
    VertexBuffer.Unbind();
    ElementBuffer.Unbind();
}

void Mesh::Draw(Shader* ShaderInstance,Camera* CameraInstance,bool UseRawData,glm::mat4 ModelMatrix,float FieldOfView,float Near,float Far,glm::vec3 Translation,glm::quat Rotation,glm::vec3 Scale,bool UseMatrix,glm::mat4 Matrix)
{
    unsigned int NumberDiffuse  = 0;
    unsigned int NumberSpecular = 0;
    ShaderInstance->ActivateShader();
    this->VertexArray.Bind();

    if (this->Textures.size() > 0)
    {
        for (unsigned int x = 0; x < this->Textures.size(); x++)
        {
            std::string Number;
            std::string Type = this->Textures[x].Type;
            
            if (Type == "diffuse")       {Number = std::to_string(NumberDiffuse++);}
            else if (Type == "specular") {Number = std::to_string(NumberSpecular++);};

            this->Textures[x].TextureUnit(ShaderInstance,(Type+Number).c_str(),x);
            this->Textures[x].Bind();
        }
    }

    glUniform3f(glGetUniformLocation(ShaderInstance->Program,"CameraPosition"),CameraInstance->Position.x,CameraInstance->Position.y,CameraInstance->Position.z);
    CameraInstance->UpdateMatrix(FieldOfView,Near,Far);
    CameraInstance->SetMatrix(ShaderInstance,"CameraMatrix");

    glm::mat4 NewTransformation     = glm::mat4(1.0f);
    glm::mat4 NewRotation           = glm::mat4(1.0f);
    glm::mat4 NewScale              = glm::mat4(1.0f);

    NewTransformation               = glm::translate(NewTransformation,Translation);
    NewRotation                     = glm::mat4_cast(Rotation);
    NewScale                        = glm::scale(NewScale,Scale);

    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Translation"),1,GL_FALSE,glm::value_ptr(NewTransformation));
    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Rotation"),1,GL_FALSE,glm::value_ptr(NewRotation));
    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"Scale"),1,GL_FALSE,glm::value_ptr(NewScale));
    glUniformMatrix4fv(glGetUniformLocation(ShaderInstance->Program,"AnimationMatrix"),1,GL_FALSE,glm::value_ptr(ModelMatrix));
    glDrawElements(GL_TRIANGLES,Indices.size(),GL_UNSIGNED_INT,0);
}   

void Mesh::Destroy()
{this->VertexArray.Delete();}