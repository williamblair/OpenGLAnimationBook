#version 330 core

in vec3 norm;
in vec3 fragPos;
in vec2 uv;

uniform vec3 light;
uniform sampler2D tex0;

out vec4 FragColor;

void main()
{
    vec4 diffuse = texture(tex0, uv);
    vec3 n = normalize(norm);
    vec3 l = normalize(light);
    float diffIntensity = clamp(dot(n,l), 0, 1);
    FragColor = diffuse * diffIntensity;
}

