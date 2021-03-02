#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec4 weights;
in ivec4 joints;

uniform mat4 pose[120]; // arbitrary length
uniform mat4 invBindPose[120];

out vec3 norm;
out vec3 fragPos;
out vec2 uv;

void main() {

    // create the skinning matrix, combining all possible 4 weighed joint poses
    mat4 skin = (pose[joints.x] * invBindPose[joints.x]) * weights.x;
         skin += (pose[joints.y] * invBindPose[joints.y]) * weights.y;
         skin += (pose[joints.z] * invBindPose[joints.z]) * weights.z;
         skin += (pose[joints.w] * invBindPose[joints.w]) * weights.w;
         
    // notice the addition of the skin matrix first
    gl_Position = projection * view * model * skin * vec4(position, 1.0);
    
    fragPos = vec3(model * skin * vec4(position, 1.0));
    norm = vec3(model * skin * vec4(normal, 0.0));
    uv = texCoord;
}
