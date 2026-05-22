#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float offsetX;
uniform float offsetY;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main() {
    gl_PointSize = 10.0;

    vec3 pos = vec3(aPos.x + offsetX, aPos.y + offsetY, aPos.z);
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
    
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
}