#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
uniform mat4 projview;
uniform mat4 transform;

void main() 
{
    gl_Position = projview * transform * vec4(a_position, 1.0);
}

#type fragment
#version 330 core
layout(location=0) out vec4 render;

uniform vec4 color;

void main() 
{
    render = color;
}