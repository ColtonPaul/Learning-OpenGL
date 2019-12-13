#include "Mesh.h"

Mesh::Mesh()
{
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

Mesh::~Mesh()
{
    clearMesh();
}

void Mesh::createMesh(GLfloat * vertices, unsigned int * indices, unsigned int numberOfVertices, unsigned int numberOfIndices)
{
    indexCount = numberOfIndices;

    glGenVertexArrays(1, &VAO); //we want to create 1 array and store it in the ID VAO
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO); //create 1 buffer object (inside the bound VAO) and give it the ID VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, 0); //Our stride of 5 is saying that the start of one vertex to the start of the next vertex has 5 numbers.
    glEnableVertexAttribArray(0);

    //attribute 0 is our position, attribute 1 is our texture. Each texture attribute has two values, and it starts at an offset of 3 (there are three values before the first texture starts)
    //The stride is still 5 because there are 5 total values (inclusive) between the start of a texture attribute and the start of the next texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (void*)(sizeof(vertices[0] * 3)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer VBO
    glBindVertexArray(0); //unbind array VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //unbind the IBO
}

void Mesh::renderMesh()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); //similar to DrawArrays, except now we're drawing it by the element ids instead of the vertices
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::clearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO); //Deletes the buffer of graphics card memory to make more room; we need to manage our own memory.
        IBO = 0;
    }
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}
