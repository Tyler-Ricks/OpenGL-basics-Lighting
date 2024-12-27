#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 1) in vec2 aTexCoord;

out vec3 position;
out vec3 normal;
//out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    position = vec3(model * vec4(aPos, 1.0f));
    normal = normalMatrix * aNormal;
    //normal = mat3(model) * aNormal;
    gl_Position = projection * view * vec4(position, 1.0f);
}