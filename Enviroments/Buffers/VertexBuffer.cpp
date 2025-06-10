#include "VertexBuffer.h"

VertexBufferObject::VertexBufferObject(std::vector<Vertex>& Vertices)
{
    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(Vertex),Vertices.data(),GL_STATIC_DRAW);
}

VertexBufferObject::VertexBufferObject(std::vector<float>& Vertices)
{
    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Vertices.size()*sizeof(float),Vertices.data(),GL_STATIC_DRAW);
}

void VertexBufferObject::Bind()
{glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);}

void VertexBufferObject::Unbind()
{glBindBuffer(GL_ARRAY_BUFFER,0);}

void VertexBufferObject::Delete()
{glDeleteBuffers(1,&this->VertexBuffer);}