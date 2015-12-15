/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#version 330 core

uniform mat4 mat_windowtoview;

layout (location = 0) in vec3 in_position;

void main()
{
    gl_Position = vec4(in_position, 1.0f);
}
