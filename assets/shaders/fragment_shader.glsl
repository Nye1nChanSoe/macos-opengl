#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D cubeTex;

void main()
{
    FragColor = texture(cubeTex, TexCoords);
}
