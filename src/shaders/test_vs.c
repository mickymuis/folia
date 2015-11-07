#version 330 core

/* Engine default input layout */
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

/* Engine default uniforms */
uniform mat4 matmodelview;
uniform mat4 matprojection;

/* Engine default output */
out vec3 position;
out vec3 normal;

void main() {

	mat4 mvp = (matprojection * matmodelview);
	position =vec3(matmodelview * vec4(in_position.x,in_position.y, in_position.z, 1.0));
	normal =mat3(matmodelview) * in_normal;
	
	gl_Position = mvp * vec4(in_position.x,in_position.y, in_position.z, 1.0);

}