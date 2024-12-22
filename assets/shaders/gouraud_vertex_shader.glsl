#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragColor; // Pass final color to the fragment shader
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int isSun;

void main()
{
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    vec3 textureColor = vec3(1.0); // Placeholder, texture applied in the fragment shader

    if (isSun == 1)
    {
        // Glow effect for Sun
        FragColor = textureColor * lightColor;
    }
    else
    {
        // Ambient lighting
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;

        // Diffuse lighting
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess factor
        vec3 specular = specularStrength * spec * lightColor;

        // Combine all components
        FragColor = ambient + diffuse + specular;
    }

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
