#version 330

out vec4 colour;

in vec4 vCol; //will get passed in from the vertex shader

void main()
{
     colour = vCol; //rgb values
}