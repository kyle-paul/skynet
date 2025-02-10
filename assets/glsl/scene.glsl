#type vertex
#version 330 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 projview;
uniform mat4 model;

out vec3 v_normal;
out vec2 v_texcoord;

void main() 
{
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
uniform vec3 viewPos;

void main() 
{
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light - vec3(0.0)); // Assuming object is at (0,0,0)

    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * vec3(1.0);

    float diffuseStrength = 0.7;
    float diffuse = max(dot(norm, lightDir), 0.4); // Min diffuse to avoid full darkness

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vec3(0.0));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0);

    vec3 lighting = ambient + diffuseStrength * diffuse + specular;
    render = vec4(lighting, 1.0) * color;
}