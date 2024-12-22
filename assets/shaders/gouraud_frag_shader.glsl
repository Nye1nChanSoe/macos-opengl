#version 410 core

in vec3 FragColor;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D planetTexture;

void main()
{
    vec3 textureColor = texture(planetTexture, TexCoords).rgb;
    vec3 result = FragColor * textureColor;
    color = vec4(result, 1.0);
}
