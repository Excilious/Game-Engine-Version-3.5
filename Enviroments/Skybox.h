#ifndef __SKYBOX_HEADER__
#define __SKYBOX_HEADER__
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <string>
#include <vector>

#include "../Dependancies/glm/glm.hpp"
#include "../Dependancies/glm/gtc/matrix_transform.hpp"
#include "../Dependancies/glm/gtc/type_ptr.hpp"
#include "../Dependancies/glm/gtx/rotate_vector.hpp"
#include "../Dependancies/glm/gtx/vector_angle.hpp"

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"
#include "../Enviroments/Texture.h"

#include "Buffers/ElementBuffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"

class Skybox
{
    public:
        std::string FaceCubemap[6] = {
            "../../Textures/Skybox/right.jpg",
            "../../Textures/Skybox/left.jpg",
            "../../Textures/Skybox/top.jpg",
            "../../Textures/Skybox/bottom.jpg",
            "../../Textures/Skybox/front.jpg",
            "../../Textures/Skybox/back.jpg"
        };

        unsigned int SkyboxVertexArrayObject;
        unsigned int SkyboxVertexBufferObject;
        unsigned int SkyboxVertexElementObject;
        unsigned int CubemapTexture;

        Skybox(int WindowWidth,int WindowHeight,float SkyNear,float SkyFar);
        void RenderSkybox(glm::vec3 Position,glm::vec3 Orientation,glm::vec3 UpPosition,Shader* SkyboxShader);
    private:
        VertexArrayObject VertexArray;
        int     Width;
        int     Height;
        float   Near;
        float   Far;
};

#endif