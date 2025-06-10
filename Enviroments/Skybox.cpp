#include "Skybox.h"
#include "Console.h"

std::vector<float> SkyboxVertices =
{
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

std::vector<unsigned int> SkyboxIndices =
{
	1, 2, 6,
	6, 5, 1,
	0, 4, 7,
	7, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 3, 2,
	2, 1, 0,
	0, 1, 5,
	5, 4, 0,
	3, 7, 6,
	6, 2, 3
};

Skybox::Skybox(int WindowWidth,int WindowHeight,float SkyNear,float SkyFar)
{
    this->Width     = WindowWidth;
    this->Height    = WindowHeight;
    this->Near      = SkyNear;
    this->Far       = SkyFar;

    this->VertexArray.Bind();
    stbi_set_flip_vertically_on_load(false);
    VertexBufferObject  VertexBuffer(SkyboxVertices);
    ElementBufferObject ElementBuffer(SkyboxIndices);
    this->VertexArray.LinkAttribute(VertexBuffer,0,3,GL_FLOAT,3*sizeof(float),(void*)0);

    VertexBuffer.Unbind();
    this->VertexArray.Unbind();
    ElementBuffer.Unbind();

    glGenTextures(1,&this->CubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP,this->CubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,0x2600);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);


    for (unsigned int x = 0; x < 6; x++)
    {
        int TextureWidth,TextureHeight,Channels;
        unsigned char* Data = stbi_load(this->FaceCubemap[x].c_str(),&TextureWidth,&TextureHeight,&Channels,0);
        if (Data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+x,0,GL_RGB,TextureHeight,TextureWidth,0,GL_RGB,GL_UNSIGNED_BYTE,Data);
            stbi_image_free(Data);
        }
        else 
        {
            SysPrint("Failed to load "+this->FaceCubemap[x],3);
            stbi_image_free(Data);
        }
    }
}

void Skybox::RenderSkybox(glm::vec3 Position,glm::vec3 Orientation,glm::vec3 UpPosition,Shader* SkyboxShader)
{
    glDepthFunc(GL_LEQUAL);
    SkyboxShader->ActivateShader();
    glm::mat4 View          = glm::mat4(1.0f);
    glm::mat4 Projection    = glm::mat4(1.0f);
    
    View        = glm::mat4(glm::mat3(glm::lookAt(Position,Position+Orientation,UpPosition)));
    Projection  = glm::perspective(glm::radians(45.0f),(float)this->Width/this->Height,this->Near,this->Far);
    glUniformMatrix4fv(glGetUniformLocation(SkyboxShader->Program,"View"),1,GL_FALSE,glm::value_ptr(View));
    glUniformMatrix4fv(glGetUniformLocation(SkyboxShader->Program,"Projection"),1,GL_FALSE,glm::value_ptr(Projection));

    this->VertexArray.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,this->CubemapTexture);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    this->VertexArray.Unbind();
    glDepthFunc(GL_LESS);
}