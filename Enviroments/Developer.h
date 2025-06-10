#ifndef __DEVELOPER_HEADER__
#define __DEVELOPER_HEADER__

#include <vector>
#include "Shader.h"
#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Dependancies/glm/glm.hpp"

class Bounds
{
    public:
        void GenerateBoundViews(glm::vec3 Minimum,glm::vec3 Maximum,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,glm::mat4 Matrix,glm::vec4 Colour);
        void RenderBoundViews(glm::mat4 CameraMatrix);
        void GenerateEnviromentTriangles(glm::vec3 PointA,glm::vec3 PointB);
        void RenderTriangleView(glm::mat4 CameraMatrix);
    private:
        std::vector<float>          BoundVertices;
        std::vector<unsigned int>   BoundIndices;
        GLuint                      VertexArray; 
        GLuint                      VertexBuffer;
        GLuint                      ElementBuffer;
        Shader                      NewShader;

        glm::mat4                   Model;
        glm::mat4                   PredefinedMatrix;
        glm::vec4                   Colour;
};

#endif