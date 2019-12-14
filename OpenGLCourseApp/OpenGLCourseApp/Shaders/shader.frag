#version 330

out vec4 colour;
in vec2 texCoord;

in vec4 vCol; //will get passed in from the vertex shader

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
};

uniform DirectionalLight directionalLight;

uniform sampler2D theTexture; //connected to the texture through the texture unit in GL_TEXTURE0

void main()
{
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    colour = texture(theTexture, texCoord) * ambientColor; //essentially this ambient color is a percentage of the color from the texture
}