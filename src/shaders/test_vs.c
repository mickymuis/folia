#version 330 core

/* Engine default input layout */
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

out VS_OUT {
	vec3 normal;
} vs_out;

void main() {

	vs_out.normal =mat3(mat_model) * in_normal;
	
	gl_Position = mat_projection * mat_view * mat_model * vec4(in_position.x,in_position.y, in_position.z, 1.0);

}