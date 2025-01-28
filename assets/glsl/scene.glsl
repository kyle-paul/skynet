#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 projview;
uniform mat4 model;

out vec3 v_normal;
out vec2 v_texcoord;

void main() {
    gl_Position = projview * model * vec4(a_position, 1.0);
    v_normal = a_normal;
    v_texcoord = a_texcoord;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    v_normal = normalMatrix * a_normal;
}

#type fragment
#version 330 core
layout(location=0) out vec4 render;

uniform vec4 color;

in vec3 v_normal;
in vec2 v_texcoord;
uniform vec3 light;

void main() {
    vec3 norm = normalize(v_normal);
    float diffuse = max(dot(norm, normalize(light)), 0.6);
    render = color * diffuse;
}