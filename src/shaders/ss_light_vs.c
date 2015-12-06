#version 330 core

uniform mat4 mat_windowtoview;

layout (location = 0) in vec3 in_position;
//layout (location = 1) in vec2 in_texCoords;

//out vec2 texCoords;

void main()
{
    gl_Position = vec4(in_position, 1.0f);
    //texCoords = in_texCoords;
}
