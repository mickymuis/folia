#version 330 core

/* Engine default input layout */
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_color;
layout(location = 3) in vec2 in_texCoords;
layout(location = 4) in vec3 in_tangent;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

out VS_OUT {
	vec3 normal;
	vec4 color;
	vec2 texCoords;
	vec3 tangent;
} vs_out;

void main() {
	vs_out.normal =in_normal;
	vs_out.color =in_color;
	vs_out.texCoords =in_texCoords;
	vs_out.tangent =in_tangent;
	gl_Position = mat_projection * mat_view * mat_model * vec4(in_position.x,in_position.y, in_position.z, 1.0);
}