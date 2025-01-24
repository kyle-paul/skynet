#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 projection_view;
uniform mat4 model;

out vec3 v_normal;
out vec2 v_texcoord;

void main() {
    gl_Position = projection_view * model * vec4(a_position, 1.0);
    v_normal = a_normal;
    v_texcoord = a_texcoord;
}

#type fragment
#version 330 core
layout(location=0) out vec4 render;

uniform vec3 color;

in vec3 v_normal;
in vec2 v_texcoord;

void main() {
    render = vec4(color, 1.0);
}