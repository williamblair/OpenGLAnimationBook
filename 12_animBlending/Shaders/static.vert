#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 norm;
out vec3 fragPos;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    fragPos = vec3(model * vec4(position, 1.0)); // world space position
    norm = vec3(model * vec4(normal, 0.0f)); // world space normal
    uv = texCoord;
}

