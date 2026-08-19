#version 330 core

in vec2 vertexTexturePosition;
out vec4 fragmentColor;
uniform sampler2D currentTexture;

void main() {
    fragmentColor = texture(currentTexture, vertexTexturePosition);
}