#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
uniform mat4 projview;

void main() 
{
    gl_Position = projview * vec4(a_position, 1.0);
}

#type fragment
#version 330 core
layout(location=0) out vec4 render;

void main() 
{
    render = vec4(1.0, 0.0, 0.0, 1.0);
}