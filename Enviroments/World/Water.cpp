#include "Water.h"

std::vector<float> WaterVertices = {
    -1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

std::vector<unsigned int> WaterIndices = {
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

Water::Water() {}

unsigned int Water::GenerateFrameBuffer()
{
    unsigned int WaterFrameBuffer;
    glGenFramebuffers(1,&WaterFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,WaterFrameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return WaterFrameBuffer;
}

unsigned int Water::GenerateDepthBuffer()
{
    unsigned int WaterDepthBuffer;
    glGenRenderbuffers(1,&WaterDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,WaterDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,this->Width,this->Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,WaterDepthBuffer);
    return WaterDepthBuffer;
}

unsigned int Water::GenerateTextureAttachment()
{
    unsigned int WaterTexture;
    glGenTextures(1,&WaterTexture);
    glBindTexture(GL_TEXTURE_2D,WaterTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,this->Width,this->Height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,WaterTexture,0);
    return WaterTexture;
}

void Water::InitiliseReflection()
{
    this->ReflectionFrameBuffer = this->GenerateFrameBuffer();
    this->ReflectionTexture     = this->GenerateTextureAttachment();
    this->ReflectionDepth       = this->GenerateDepthBuffer();
}

void Water::InitiliseRefraction()
{
    this->RefractionFrameBuffer = this->GenerateFrameBuffer();
    this->RefractionTexture     = this->GenerateTextureAttachment();
    this->RefractionDepth       = this->GenerateDepthBuffer();
}

void Water::BindFrameBuffer(int Width,int Height,unsigned int WaterFrameBuffer)
{
    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER,WaterFrameBuffer);
    glViewport(0,0,Width,Height);
}

void Water::UnBindFrameBuffer(unsigned int WaterFrameBuffer)
{
    glBindFramebuffer(WaterFrameBuffer,0);
    glViewport(0,0,this->Width,this->Height);
}

void Water::GenerateWater(int WindowWidth,int WindowHeight,float WaterNear,float WaterFar,glm::vec3 WaterPosition,glm::vec3 WaterSize,glm::quat WaterRotate)
{
    this->Width     = WindowWidth;
    this->Height    = WindowHeight;
    this->Near      = Near;
    this->Far       = Far;
    this->Position  = WaterPosition;
    this->Size      = WaterSize;
    this->Rotate    = WaterRotate;

    this->VertexArray.Bind();
    VertexBufferObject      VertexBuffer(WaterVertices);
    ElementBufferObject     ElementBuffer(WaterIndices);

    glEnable(GL_CLIP_DISTANCE0);
    this->VertexArray.LinkAttribute(VertexBuffer,0,3,GL_FLOAT,3*sizeof(float),(void*)0);
    VertexBuffer.Unbind();
    this->VertexArray.Unbind();
    ElementBuffer.Unbind();
}

void Water::RenderWater(glm::vec3 CameraPosition,glm::mat4 CameraMatrix,Shader WaterShader)
{
    WaterShader.ActivateShader();
    this->VertexArray.Bind();

    glm::mat4 Model                = glm::mat4(1.0f);
    glm::mat4 TranslateMatrix      = glm::mat4(1.0f);
    glm::mat4 ScaleMatrix          = glm::mat4(1.0f);
    glm::mat4 RotateMatrix         = glm::mat4(1.0f);

    ScaleMatrix       = glm::scale(ScaleMatrix,this->Size);
    TranslateMatrix   = glm::translate(TranslateMatrix,this->Position);
    RotateMatrix      = glm::mat4_cast(this->Rotate);
    Model             = TranslateMatrix*RotateMatrix*ScaleMatrix;

    glUniformMatrix4fv(glGetUniformLocation(WaterShader.Program,"camMatrix"),1,GL_FALSE,glm::value_ptr(CameraMatrix));
    glUniformMatrix4fv(glGetUniformLocation(WaterShader.Program,"scale"),1,GL_FALSE,glm::value_ptr(ScaleMatrix));
    glUniformMatrix4fv(glGetUniformLocation(WaterShader.Program,"translate"),1,GL_FALSE,glm::value_ptr(TranslateMatrix));
    glUniformMatrix4fv(glGetUniformLocation(WaterShader.Program,"rotate"),1,GL_FALSE,glm::value_ptr(RotateMatrix));
    glUniformMatrix4fv(glGetUniformLocation(WaterShader.Program,"model"),1,GL_FALSE,glm::value_ptr(Model));

    glDrawElements(GL_TRIANGLES,WaterIndices.size(),GL_UNSIGNED_INT,0);
}

void Water::Destroy()
{
    this->VertexArray.Delete();
}