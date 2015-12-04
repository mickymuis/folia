#version 330 core

in GS_OUT {
    vec3 normal;
    vec2 texCoords;
} fs_in;

/* Engine default fragment pass output */

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out;


void main() {
	
	normal_out =normalize(fs_in.normal);
  diffuse_spec_out =vec4( 0.0, 1.0, 0.0, 0.2 );
	
}
