#version 330 core
layout (location = 0) in vec2 vertex; // <vec2 position>

uniform mat4 model;
uniform mat4 proj;

void main()
{
    gl_Position = proj * model * vec4(vertex, 0.0, 1.0);
}