#version 330 core

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out; 

/*in VS_OUT {
	vec3 normal;
} fs_in;*/

in GS_OUT {
    vec3 normal;
    vec3 color;
} fs_in;

void main() {
	
	normal_out = normalize( fs_in.normal );
	diffuse_spec_out = vec4( 0.7, 0.7, 0.7, 1.0 );
	
}
