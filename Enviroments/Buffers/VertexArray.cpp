#include "VertexArray.h"

VertexArrayObject::VertexArrayObject()
{glGenVertexArrays(1,&this->VertexArray);}

void VertexArrayObject::Bind()
{glBindVertexArray(this->VertexArray);}

void VertexArrayObject::Unbind()
{glBindVertexArray(0);}

void VertexArrayObject::Delete()
{glDeleteVertexArrays(1,&this->VertexArray);}

void VertexArrayObject::LinkAttribute(VertexBufferObject& VertexBuffer,GLuint Layout,GLuint Components,GLenum Type,GLsizeiptr Stride,void* Offset)
{
    VertexBuffer.Bind();
    glVertexAttribPointer(Layout,Components,Type,GL_FALSE,Stride,Offset);
    glEnableVertexAttribArray(Layout);
    VertexBuffer.Unbind();
}