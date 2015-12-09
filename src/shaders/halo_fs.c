#version 330 core

in GS_OUT {
    vec2 texCoords;
} fs_in;

/* Engine default fragment pass output */
//layout (location = 0) 

out vec4 fragColor; 


void main() {

	float intensity = 1.0 - length( vec2(0.5) - fs_in.texCoords ) * 2.0;
	
	fragColor =vec4( intensity );	
	if( intensity == 0 )
		discard;
}