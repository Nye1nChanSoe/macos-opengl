#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D planetTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int isSun;

void main()
{
    vec3 textureColor = texture(planetTexture, TexCoords).rgb;

    if (isSun == 1)
    {
        // Blend the Sun's texture with lightColor to give it a glowing effect
        vec3 glowColor = textureColor * lightColor;
        FragColor = vec4(glowColor, 1.0);
        return;
    }

    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess factor
    vec3 specular = specularStrength * spec * lightColor;

    // Combine all components
    vec3 lighting = ambient + diffuse + specular;
    FragColor = vec4(lighting * textureColor, 1.0);
}
