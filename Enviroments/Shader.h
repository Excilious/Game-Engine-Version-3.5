#ifndef __SHADER_HEADER_FILE__
#define __SHADER_HEADER_FILE__

#define VERTEX_REPORT_ID        0x000001
#define FRAGMENT_REPORT_ID      0x000002
#define PROGRAM_REPORT_ID       0x000003

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

#include "../Dependancies/glad/glad.h"
#include "../Dependancies/GLFW/glfw3.h"

std::string GetShaderContents(const char* ShaderName);
void ReportErrors(unsigned int Shaders,int Type);

class Shader
{
    public:
        Shader();
        void    CreateNewShader(const char* VertexShaderFile,const char* FragmentShaderFile);
        void    ActivateShader();
        void    DestroyShader();
        GLuint  Program;
};


#endif