#version 330 core

in GS_OUT {
    vec3 normal;
  //  vec3 color;
} fs_in;

/* Engine default fragment pass output */
//out vec4 fragcolor;

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out;


void main() {
/*	vec3 light = vec3(1.0, 1.0, 1.0);
	vec3 n =normalize( fs_in.normal );
	float intensity = abs(n.x*0.3+ n.z*0.4 + n.y *0.6); //"stupid" shading
	
	
	intensity = intensity + 0.1; //ambient lighting
	fragcolor = vec4(intensity*light*fs_in.color, 1.0);*/
	
	normal_out =normalize(fs_in.normal);
//	diffuse_spec_out =vec4( fs_in.color, 0.2 );
  diffuse_spec_out =vec4( 0.0, 1.0, 0.0, 0.2 );
	
}
