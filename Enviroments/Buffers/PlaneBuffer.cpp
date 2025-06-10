#include "PlaneBuffer.h"

PlaneBuffer::PlaneBuffer() {}

void PlaneBuffer::CreatePlaneBuffer(int Resolution,int Width,unsigned int *VertexArray,unsigned int *VertexBuffer,unsigned int *ElementBuffer)
{
    int Points      = Resolution*Resolution;
    int Size        = Points*3+Points*3+Points*2;
    float *Vertices  = new float[Size];
    for (int IndexX = 0; IndexX < Resolution; IndexX++)
    {
        for (int IndexY = 0; IndexY < Resolution; IndexY++)
        {
            float X = IndexY*(float)Width/(Resolution-1)-Width/2.0f;
            float Y = 0.0f;
            float Z = -IndexX*(float)Width/(Resolution-1)+Width/2.0f;

            Vertices[(IndexX+IndexY*Resolution)*8]      = X;
            Vertices[(IndexX+IndexY*Resolution)*8+1]    = Y;
            Vertices[(IndexX+IndexY*Resolution)*8+2]    = Z;

            float XNormal = 0.0f;
            float YNormal = 0.0f;
            float ZNormal = 0.0f;

            Vertices[(IndexX+IndexY*Resolution)*8+3]    = XNormal;
            Vertices[(IndexX+IndexY*Resolution)*8+4]    = YNormal;
            Vertices[(IndexX+IndexY*Resolution)*8+5]    = ZNormal;
            Vertices[(IndexX+IndexY*Resolution)*8+6]    = (float)IndexY/(Resolution-1);
            Vertices[(IndexX+IndexY*Resolution)*8+7]    = (float)(Resolution-IndexX-1)/(Resolution-1);
        }
    }

    int Triangles   = (Resolution-1)*(Resolution-1)*2;
    int *Indices    = new int[Triangles*3];

    for (int IndexX = 0; IndexX < Triangles; IndexX++)
    {
        int TrianglesRow = 2*(Resolution-1);
        for (int IndexY = 0; IndexY < TrianglesRow; IndexY++)
        {
            int IndexK          = IndexX*3;
            int TriangleIndex   = IndexX%TrianglesRow;
            int Row             = IndexX/TrianglesRow;
            int Column          = TriangleIndex/2;

            if (!(IndexX%2))
            {
                Indices[IndexK]     = Row*Resolution+Column;
                Indices[IndexK+1]   = ++Row*Resolution+Column;
                Indices[IndexK+2]   = --Row*Resolution+++Column;
            }
            else 
            {
                Indices[IndexK]     = Row*Resolution+++Column;
                Indices[IndexK+1]   = ++Row*Resolution+--Column;
                Indices[IndexK+2]   = Row*Resolution+++Column;
            }
        }
    }

    glGenVertexArrays(1,VertexArray);
    glGenBuffers(1,VertexBuffer);
    glGenBuffers(1,ElementBuffer);

    glBindVertexArray(*VertexArray);
    glBindBuffer(GL_ARRAY_BUFFER,*VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,Size*sizeof(float),Vertices,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,*ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,Triangles*3*sizeof(unsigned int),Indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    
    delete[] Vertices;
}