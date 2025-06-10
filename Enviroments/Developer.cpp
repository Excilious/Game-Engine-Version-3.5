#include "Developer.h"
#include "Console.h"

void Bounds::GenerateBoundViews(glm::vec3 Minimum,glm::vec3 Maximum,glm::vec3 Position,glm::vec3 Scale,glm::quat Rotation,glm::mat4 Matrix,glm::vec4 Colour)
{
    SysPrint("Generating static bounds...",1);
    this->BoundVertices = {
        Minimum.x, Minimum.y, Minimum.z, // Corner 0
        Maximum.x, Minimum.y, Minimum.z, // Corner 1
        Maximum.x, Maximum.y, Minimum.z, // Corner 2
        Minimum.x, Maximum.y, Minimum.z, // Corner 3
        Minimum.x, Minimum.y, Maximum.z, // Corner 4
        Maximum.x, Minimum.y, Maximum.z, // Corner 5
        Maximum.x, Maximum.y, Maximum.z, // Corner 6
        Minimum.x, Maximum.y, Maximum.z  // Corner 7
    };

    this->BoundIndices = {
        0, 1, 1, 2, 2, 3, 3, 0, // Bottom face
        4, 5, 5, 6, 6, 7, 7, 4, // Top face
        0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges
    };

    glm::mat4 PositionMatrix = glm::mat4(1.0f);
    glm::mat4 RotationMatrix = glm::mat4(1.0f);
    glm::mat4 ScaleMatrix = glm::mat4(1.0f);

    PositionMatrix  = glm::translate(PositionMatrix,Position);
    RotationMatrix  = glm::mat4_cast(Rotation);
    ScaleMatrix     = glm::scale(ScaleMatrix,Scale);

    this->NewShader.CreateNewShader("../../Shaders/DebugShader/Debug.vert","../../Shaders/DebugShader/Debug.frag");
    this->Model = PositionMatrix*RotationMatrix*ScaleMatrix;
    this->Colour = Colour;
    this->PredefinedMatrix = Matrix;

    glGenVertexArrays(1,&this->VertexArray);
    glGenBuffers(1,&this->VertexBuffer);
    glGenBuffers(1,&this->ElementBuffer);

    glBindVertexArray(this->VertexArray);

    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,BoundVertices.size() * sizeof(float),BoundVertices.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,BoundIndices.size() * sizeof(unsigned int),BoundIndices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Bounds::GenerateEnviromentTriangles(glm::vec3 PointA,glm::vec3 PointB)
{
    float Vertices[] = {
        PointA.x,PointA.y,PointA.z,
        PointB.x,PointB.y,PointB.z
    };

    glGenBuffers(1,&this->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices,GL_STATIC_DRAW);

    glGenVertexArrays(1,&this->VertexArray);
    glBindVertexArray(this->VertexArray);
    glBindBuffer(GL_ARRAY_BUFFER,this->VertexBuffer);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void Bounds::RenderTriangleView(glm::mat4 CameraMatrix)
{
    this->NewShader.ActivateShader();

    glm::mat4 Model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"camera"),1,GL_FALSE,glm::value_ptr(CameraMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"model"),1,GL_FALSE,glm::value_ptr(Model));
    glUniform4f(glGetUniformLocation(this->NewShader.Program,"color"),1.0f,1.0f,1.0f,1.0f);

    glBindVertexArray(this->VertexArray);
    glDrawArrays(GL_LINES,0,2);
    glBindVertexArray(0);
}

void Bounds::RenderBoundViews(glm::mat4 CameraMatrix)
{
    this->NewShader.ActivateShader();
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"camera"),1,GL_FALSE,glm::value_ptr(CameraMatrix));
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"model"),1,GL_FALSE,glm::value_ptr(this->Model));
    glUniformMatrix4fv(glGetUniformLocation(this->NewShader.Program,"matrix"),1,GL_FALSE,glm::value_ptr(this->PredefinedMatrix));
    glUniform4f(glGetUniformLocation(this->NewShader.Program,"color"),this->Colour.x,this->Colour.y,this->Colour.z,this->Colour.w);

    glBindVertexArray(this->VertexArray);
    glDrawElements(GL_LINES,this->BoundIndices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}