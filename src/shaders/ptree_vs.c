#version 330 core

/* Engine default input layout */
layout(location = 0) in vec3 in_position;
layout(location = 6) in vec3 in_param;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

out VS_OUT {
	vec3 param;
} vs_out;

void main() {
	vs_out.param =in_param;
	gl_Position = vec4(in_position.x,in_position.y, in_position.z, 1.0);
}
