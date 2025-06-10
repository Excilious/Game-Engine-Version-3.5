#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#define GLT_IMPLEMENTATION

#include <vector>
#include <map>
#include <string>
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/gltext/gltext.h"

#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Shader.h"

class Frame
{
    public:
        Frame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FileName);
        void RenderInstance();
        void DestroyInstance();

        float           WindowWidth;
        float           WindowHeight;
        float           Rotation;
        glm::vec2       Position;
        glm::vec2       Scale;
        glm::vec3       Colour;
        glm::mat4       OrthoMatrix;
        std::string     FileName;
        Shader          NewShader;

    private:
        void                GenerateTextures();
        unsigned int        TextureID;
        unsigned int        FrameVertexArray;
        unsigned int        FrameVertexBuffer;
        glm::mat4           ProjectionMatrix;
        glm::mat4           ModelMatrix;
};

class TextLabel
{
    public:
        TextLabel(std::string Text,float Scale,glm::vec2 Position,glm::vec3 Colour);
        void RenderText();
        void DestroyText();
        glm::vec2   Position;
        std::string Text;
        
    private:
        
        GLTtext     *TextInstance;
        float       Scale;
        glm::vec3   Colour;

        unsigned int TextVertexArray;
        unsigned int TextVertexBuffer;
};

#endif