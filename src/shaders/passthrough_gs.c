#version 330 core

layout(triangles) in;
layout (triangle_strip, max_vertices=512) out;

/* Engine default uniforms */
uniform mat4 mat_modelview;
uniform mat4 mat_projection;
 
in VS_OUT {
    vec3 normal;
} gs_in[3];
 
out GS_OUT {
    vec3 normal;
    vec3 color;
} gs_out;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 
bezier2(
  float t,
  const vec3 c1,
  const vec3 c2,
  const vec3 c3)
{
  return  pow( 1.0-t, 2 ) * c1 + 2*(1.0-t)*t*c2 + pow( t, 2 ) * c3;
}

void emitAndMult( vec4 v, vec3 n ) {

  gs_out.color =vec3(0,1,0);
  gs_out.normal =mat3(mat_modelview) * n;
  gl_Position =mat_projection * mat_modelview * v;
  EmitVertex();
	  
}


vec4 add_me = vec4(1,2,3,4);
float constant =1.0;

void emitCube( ) {

  mat4 rot =rotationMatrix( vec3(0,1,0), degrees(5.0) );

  for( int i =0; i < 3; i++ ) {
	    emitAndMult ( rot * (gl_in[i].gl_Position + add_me), gs_in[i].normal );
	
	  }
}


void main() {

  add_me = vec4(0,0,0,0);
  
  vec3 c1 = vec3(0,0,0);
  vec3 c2 = vec3(0,8,-2 );
  vec3 c3 = vec3( 10,10,10 );

  for( float t =0; t < 1.0; t+=0.1 ) {	
    
    add_me = vec4( bezier2( t, c1, c2, c3 ), 0 );
    emitCube();
	  
	  EndPrimitive();
	}
	
	
	
}
