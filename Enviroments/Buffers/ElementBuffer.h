#ifndef __ELEMENT_BUFFER_HEADER__
#define __ELEMENT_BUFFER_HEADER__

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"
#include <vector>

class ElementBufferObject
{
    public:
        GLuint ElementBuffer;
        ElementBufferObject(std::vector<GLuint>& Indices);
        void Bind();
        void Unbind();
        void Delete();
};

#endif