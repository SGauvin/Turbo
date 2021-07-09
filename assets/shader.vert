#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec4 a_color;

out vec3 v_position;
out vec3 v_normal;
out vec4 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(a_position, 1.0);
    v_position = vec3(model * vec4(a_position, 1.0));
    v_color = a_color;
    v_normal = mat3(transpose(inverse(model))) * a_normal;
}