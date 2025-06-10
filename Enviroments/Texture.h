#ifndef __TEXTURE_HEADER__
#define __TEXTURE_HEADER__

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/stb/stb_image.h"
#include "Shader.h"

class Texture
{
    public: 
        Texture();
        void TextureUnit(Shader* NewShader,const char* Uniform,GLuint Unit);
        void CreateNormalTexture(const char* Image,const char* TextureType,GLuint Slot);
        void Bind();
        void Unbind();
        void Delete();

        unsigned int GetTextureFromFile(std::string Path,bool Gamma);

        GLuint TextureID;
        GLuint Unit;
        const char* Type;
};

#endif 