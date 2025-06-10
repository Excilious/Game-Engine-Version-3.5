#include "Interface.h"
#include "../Texture.h"
#include "../Console.h"

Frame::Frame(glm::vec2 Position,glm::vec2 Scale,int Width,int Height,glm::vec3 Colour,float Rotation,std::string FileName)
{   
    this->Rotation      = Rotation;
    this->Scale         = Scale;
    this->Position      = Position;
    this->Colour        = Colour;
    this->WindowWidth   = Width;
    this->WindowHeight  = Height;
    this->FileName      = FileName;

    float Vertices[ ] = { 
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    this->NewShader.CreateNewShader("../../Shaders/Interface2D/Interface.vert","../../Shaders/Interface2D/Interface.frag");
    glGenVertexArrays(1,&this->FrameVertexArray);
    glGenBuffers(1,&this->FrameVertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER,this->FrameVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices,GL_STATIC_DRAW);

    glBindVertexArray(this->FrameVertexArray);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //this->GenerateTextures();
    Texture Diffuse;
    glBindTexture(GL_TEXTURE_2D,0);
    SysPrint(this->FileName.c_str(),1);
    Diffuse.CreateNormalTexture(this->FileName.c_str(),"diffuse",4);
    Diffuse.TextureUnit(&this->NewShader,"TextureID",4);
    Diffuse.Bind();

    this->NewShader.ActivateShader();
}

TextLabel::TextLabel(std::string Text,float Scale,glm::vec2 Position,glm::vec3 Colour)
{
    if (!gltInit()) {glfwTerminate();}

    this->Position  = Position;
    this->Scale     = Scale;
    this->Text      = Text;
    this->Colour    = Colour;
    this->TextInstance = gltCreateText();
    gltSetText(this->TextInstance,this->Text.c_str());
}

void Frame::GenerateTextures()
{
    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &this->TextureID);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int Width, Height, Channel;
    unsigned char* Data = stbi_load(this->FileName.c_str(), &Width, &Height, &Channel, 0);
    if (Data)
    {
        if (Channel == 4) {
            SysPrint("Channel 4",1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        }
        else if (Channel == 3) {
            SysPrint("Channel 3",1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        }
        else if (Channel == 1) {
            SysPrint("Channel 1",1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Width, Height, 0, GL_RED, GL_UNSIGNED_BYTE, Data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        SysPrint("FAILED",3);
        std::cerr << "Failed to load texture: " << this->FileName << std::endl;
    }
    SysPrint("Done",1);
    stbi_image_free(Data);
}

void Frame::RenderInstance()
{
    glDisable(GL_DEPTH_TEST);
    this->NewShader.ActivateShader();
    this->ModelMatrix = glm::mat4(1.0f);
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(this->Position,0.0f));
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(0.5f*this->Scale.x,0.5f*this->Scale.y,0.0f));
    this->ModelMatrix = glm::rotate(this->ModelMatrix,glm::radians(this->Rotation),glm::vec3(0.0f,0.0f,1.0f));
    this->ModelMatrix = glm::translate(this->ModelMatrix,glm::vec3(-0.5f*this->Scale.x,-0.5f*this->Scale.y,0.0f));
    this->ModelMatrix = glm::scale(this->ModelMatrix,glm::vec3(this->Scale,1.0f)); 

    this->ProjectionMatrix = glm::ortho(0.0f,this->WindowWidth,this->WindowHeight,0.0f,-1.0f,1.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"model"),1,GL_FALSE,glm::value_ptr(this->ModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"projection"),1,GL_FALSE,glm::value_ptr(this->ProjectionMatrix));
    glUniform3f(glGetUniformLocation(this->NewShader.Program,"spriteColor"),this->Colour.x,this->Colour.y,this->Colour.z);
  
    glBindVertexArray(this->FrameVertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void TextLabel::RenderText()
{
    gltBeginDraw();
    gltColor(this->Colour.x,this->Colour.y,this->Colour.z,1.0f);
    gltDrawText2D(this->TextInstance,this->Position.x,this->Position.y,this->Scale);
    gltEndDraw();
}

void Frame::DestroyInstance()
{
    glDeleteVertexArrays(1,&this->FrameVertexArray);
}


void TextLabel::DestroyText()
{
    gltDeleteText(this->TextInstance);
    gltTerminate();
}