#ifndef __WATER_HEADER__
#define __WATER_HEADER__

#include <vector>

#include "../../Dependancies/glad/glad.h"
#include "../../Dependancies/GLFW/glfw3.h"
#include "../../Dependancies/glm/glm.hpp"
#include "../../Dependancies/glm/gtc/type_ptr.hpp"

#include "../Buffers/ElementBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Shader.h"

class Water
{
    public:
        Water();
        void Destroy();
        void RenderWater(glm::vec3 CameraPosition,glm::mat4 CameraMatrix,Shader WaterShader);
        void GenerateWater(int WindowWidth,int WindowHeight,float WaterNear,float WaterFar,glm::vec3 WaterPosition,glm::vec3 WaterSize,glm::quat WaterRotate);
    private:
        unsigned int GenerateFrameBuffer();
        unsigned int GenerateDepthBuffer();
        unsigned int GenerateTextureAttachment();

        void InitiliseReflection();
        void InitiliseRefraction();

        void UnBindFrameBuffer(unsigned int WaterFrameBuffer);
        void BindFrameBuffer(int Width,int Height,unsigned int WaterFrameBuffer);

        int Width;
        int Height;
        float Near;
        float Far;
        glm::vec3 Position;
        glm::vec3 Size;
        glm::quat Rotate;

        unsigned int WaterVertexArray;
        unsigned int WaterVertexBuffer;
        unsigned int WaterElementInstance;

        unsigned int ReflectionFrameBuffer;
        unsigned int ReflectionTexture;
        unsigned int ReflectionDepth;

        unsigned int RefractionFrameBuffer;
        unsigned int RefractionTexture;
        unsigned int RefractionDepth;

        VertexArrayObject VertexArray;
};

#endif