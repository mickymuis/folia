#version 330 core

layout(triangles) in;
layout (triangle_strip, max_vertices=512) out;

/* Engine default uniforms */
uniform mat4 matmodelview;
uniform mat4 matprojection;
 
in VS_OUT {
    vec3 param;
} gs_in[3];
 
out GS_OUT {
    vec3 normal;
    vec3 color;
} gs_out;


void main() {
  gs_out.normal =vec3(0,1,0);
	gs_out.color =gs_in[0].param;
	gl_Position =matprojection * matmodelview * gl_in[0].gl_Position;
	EmitVertex();
	
	gs_out.normal =vec3(0,1,0);
	gs_out.color =gs_in[0].param;
	gl_Position =matprojection * matmodelview * gl_in[1].gl_Position;
	EmitVertex();
	
	gs_out.normal =vec3(0,1,0);
	gs_out.color =gs_in[0].param;
	gl_Position =matprojection * matmodelview * gl_in[2].gl_Position;
	EmitVertex();
	
	EndPrimitive();
}
