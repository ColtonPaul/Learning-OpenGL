#version 330

out vec4 colour;
in vec2 texCoord;

in vec4 vCol; //will get passed in from the vertex shader

uniform sampler2D theTexture; //connected to the texture through the texture unit in GL_TEXTURE0

void main()
{
     colour = texture(theTexture, texCoord);
}