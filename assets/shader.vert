#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec3 v_position;
out vec4 v_color;

void main()
{
    v_position = position;
    v_color = color;
    gl_Position = vec4(position, 1.0);
}