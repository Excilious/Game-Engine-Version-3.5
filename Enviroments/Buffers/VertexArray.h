#ifndef __VERTEX_ARRAY_HEADER__
#define __VERTEX_ARRAY_HEADER__

#include "../Dependancies/glad/glad.h"
#include "VertexBuffer.h"
#include <vector>

class VertexArrayObject
{
    public:
        GLuint VertexArray;
        VertexArrayObject();
        void Bind();
        void Unbind();
        void Delete();
        void LinkAttribute(VertexBufferObject& VertexBuffer,GLuint Layout,GLuint Components,GLenum Type,GLsizeiptr Stride,void* Offset);
};

#endif