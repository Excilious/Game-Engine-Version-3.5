#ifndef __VERTEX_BUFFER_HEADER__
#define __VERTEX_BUFFER_HEADER__

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/glm/glm.hpp"
#include <vector>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Colour;
    glm::vec2 TextureUV;
};

class VertexBufferObject
{
    public:
        GLuint VertexBuffer;
        VertexBufferObject(std::vector<Vertex>& Vertices);
        VertexBufferObject(std::vector<float>& Vertices);
        void Bind();
        void Unbind();
        void Delete();
};

#endif
