#version 330 core

in VS_OUT {
	vec3 normal;
	vec4 color;
	vec2 texCoords;
	vec3 tangent;
} fs_in;

/* Engine default fragment pass output */

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out;
layout (location = 2) out vec4 position_out;


void main() {
	
	normal_out =normalize(fs_in.normal);
  diffuse_spec_out =vec4( fs_in.color.rgb, 0.5 );
	position_out =gl_FragCoord;
	
//  gl_FragDepth = 5.0;

	
}