#version 330 core

in GS_OUT {
    vec3 normal;
    vec3 texCoords;
    vec3 tangent;
} fs_in;

/* Engine default fragment pass output */

layout (location = 0) out vec4 diffuse_spec_out; 
layout (location = 1) out vec3 normal_out;

uniform sampler2D tex_diffuse1;
uniform sampler2D tex_diffuse2;
uniform sampler2D tex_normal;

void main() {
	
	float blendFactor =fs_in.texCoords.z;
	
//	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 T = normalize( fs_in.tangent );
//  vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	vec3 N = normalize( fs_in.normal );
 vec3 B = cross( T, N );
  mat3 TBN = mat3(T, B, N);
	
	vec3 normal =texture( tex_normal, fs_in.texCoords.xy).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(TBN * normal);
	
	normal_out = normalize( normal *blendFactor + fs_in.normal * (1.0-blendFactor) );
	//normal_out =normal;
	
	//normal_out =normalize(fs_in.normal);
  //diffuse_spec_out =vec4( 0.0, 1.0, 0.0, 0.2 );
  vec3 color = normalize( 
  	texture( tex_diffuse2, fs_in.texCoords.xy ).rgb * blendFactor 
  	+ texture( tex_diffuse1, fs_in.texCoords.xy ).rgb * (1.0-blendFactor) );
  	
  diffuse_spec_out =vec4( color, 0.2 );
	
}
