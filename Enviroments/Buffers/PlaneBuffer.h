#ifndef __PLANE_BUFFER_H__
#define __PLANE_BUFFER_H__

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"

class PlaneBuffer
{
    public:
        PlaneBuffer();
        void CreatePlaneBuffer(int Resolution,int Width,unsigned int *VertexArray,unsigned int *VertexBuffer,unsigned int *ElementBuffer);
    private:
};

#endif
