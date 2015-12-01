#version 330 core

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out; 

in VS_OUT {
	vec3 normal;
} fs_in;

void main() {
	/*vec3 light = vec3(0.5, 0.0, 0.0);
	float intensity = abs(normal.x*0.3+ normal.z*0.1 + normal.y *0.6); //"stupid" shading
	
	
	intensity = intensity + 0.1; //ambient lighting
	fragcolor = vec4(intensity*light, 1.0);*/
	
	
	normal_out = normalize( fs_in.normal );
	diffuse_spec_out = vec4( 0.7, 0.7, 0.7, 1.0 );
	
}