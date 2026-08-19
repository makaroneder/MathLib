#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texturePosition;
uniform mat4 transform;
uniform mat4 camera;
uniform mat4 projection;
out vec2 vertexTexturePosition;

void main() {
    gl_Position = projection * camera * transform * vec4(position, 1);
    vertexTexturePosition = texturePosition;
}