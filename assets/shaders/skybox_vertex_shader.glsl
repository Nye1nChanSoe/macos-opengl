#version 410 core
layout(location = 0) in vec3 a_Position;

uniform mat4 projection;
uniform mat4 view;

out vec3 v_TexCoords;

void main()
{
    v_TexCoords = a_Position;
    gl_Position = projection * view * vec4(a_Position, 1.0);
}
