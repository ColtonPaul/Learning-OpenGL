#pragma once

#include <GL\glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void createMesh(GLfloat* vertices, unsigned int *indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
    void renderMesh();
    void clearMesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

