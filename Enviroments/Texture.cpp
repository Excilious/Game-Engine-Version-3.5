#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "Console.h"

Texture::Texture(){}

unsigned int Texture::GetTextureFromFile(std::string Path,bool Gamma)
{
    unsigned int TextureID;
    glGenTextures(1,&TextureID);
    
    int Width,Height,Components;
    unsigned char *Data = stbi_load(Path.c_str(),&Width,&Height,&Components,0);
    if (Data)
    {
        GLenum Format;
        if (Components == 1){Format = GL_RED;}
        if (Components == 3){Format = GL_RGB;}
        if (Components == 4){Format == GL_RGBA;}

        glBindTexture(GL_TEXTURE_2D,TextureID);
        glTexImage2D(GL_TEXTURE_2D,0,Format,Width,Height,0,Format,GL_UNSIGNED_BYTE,Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    stbi_image_free(Data);
    return TextureID;
}

void Texture::CreateNormalTexture(const char* Image,const char* TextureType,GLuint Slot)
{
    int WidthImage,HeightImage,ColourNumberChannel;
    unsigned char* Bytes = stbi_load(Image,&WidthImage,&HeightImage,&ColourNumberChannel,0);
    stbi_set_flip_vertically_on_load(true);
    this->Type = TextureType;

    glGenTextures(1,&this->TextureID);
    glActiveTexture(GL_TEXTURE0+Slot);
    
    this->Unit = Slot;
    glBindTexture(GL_TEXTURE_2D,this->TextureID);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    if (ColourNumberChannel == 4) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RGBA,GL_UNSIGNED_BYTE,Bytes);} 
    else if (ColourNumberChannel == 3) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RGB,GL_UNSIGNED_BYTE,Bytes);} 
    else if (ColourNumberChannel == 1) {glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,WidthImage,HeightImage,0,GL_RED,GL_UNSIGNED_BYTE,Bytes);} 
    else {SysPrint("Cannot find texture type.",2);}

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(Bytes);

    glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::TextureUnit(Shader* NewShader,const char* Uniform,GLuint Unit)
{
    GLuint TextureUnit = glGetUniformLocation(NewShader->Program,Uniform);
    NewShader->ActivateShader();
    glUniform1i(TextureUnit,Unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0+this->Unit);
    glBindTexture(GL_TEXTURE_2D,this->TextureID);
}

void Texture::Unbind()
{glBindTexture(GL_TEXTURE_2D,0);}

void Texture::Delete()
{glDeleteTextures(1,&this->TextureID);}

