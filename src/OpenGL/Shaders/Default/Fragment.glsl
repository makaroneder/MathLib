#version 330 core

in vec2 vertexTexturePosition;
uniform sampler2D currentTexture;
out vec4 fragmentColor;

void main() {
    fragmentColor = texture(currentTexture, vertexTexturePosition);
}