#version 430

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec4 objectColor;
uniform int useTexture;
uniform sampler2D texture1;

void main() {
    if (useTexture == 1) {
        FragColor = texture(texture1, TexCoord);
    } else {
        FragColor = objectColor;
    }
}