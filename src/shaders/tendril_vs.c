#version 330 core

/* Engine default input layout */
layout(location = 0) in vec3 in_position;
layout(location = 6) in vec3 in_param;

/* Engine default uniforms */
uniform mat4 matmodelview;
uniform mat4 matprojection;

out VS_OUT {
	vec3 param;
} vs_out;

//out vec3 position;
//out vec3 normal;

void main() {

	//mat4 mvp = (matprojection * matmodelview);
	//position =vec3(matmodelview * vec4(in_position.x,in_position.y, in_position.z, 1.0));
	vs_out.param =in_param;
	
	gl_Position = /*matmodelview **/ vec4(in_position.x,in_position.y, in_position.z, 1.0);

}