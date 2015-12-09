#version 330 core

layout(points) in;
layout (triangle_strip, max_vertices=4) out;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

in VS_OUT {
    vec3 param;
} gs_in[1];
 
out GS_OUT {
    vec2 texCoords;
} gs_out;

void main() {
	vec3 pos = gl_in[0].gl_Position.xyz;
	
	vec3 toCamera = normalize(-pos);
  vec3 up = vec3(0.0, 1.0, 0.0);
  vec3 right = cross(toCamera, up);
  
  float radius =gs_in[0].param.x;
  
  //vec3 v = pos - (right * 0.5);
  vec3 v = pos - vec3( 0.5, 0.5, 0.0 ) * radius;
	gl_Position = mat_projection * vec4(v, 1.0);
	gs_out.texCoords = vec2(0.0, 0.0);
	EmitVertex();

	//v.y += 1.0;
	v = pos + vec3( -0.5, 0.5, 0.0 ) * radius;
	gl_Position = mat_projection * vec4(v, 1.0);
	gs_out.texCoords = vec2(0.0, 1.0);
	EmitVertex();

	//v.y -= 1.0;
	//v += right;
	v = pos + vec3( 0.5, -0.5, 0.0 ) * radius;
	gl_Position = mat_projection * vec4(v, 1.0);
	gs_out.texCoords = vec2(1.0, 0.0);
	EmitVertex();

//	v.y += 1.0;
	v = pos + vec3( 0.5, 0.5, 0.0 ) * radius;
	gl_Position = mat_projection * vec4(v, 1.0);
	gs_out.texCoords = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
    
}
