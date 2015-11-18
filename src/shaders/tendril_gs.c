#version 330 core

#define RING_SECTIONS 6.0
#define RADIUS 0.05
#define PI 3.141592653589793238462643383279
#define T_STEP 0.2
#define T_MAX 14.0
#define T_BEGIN 1

layout(triangles) in;
layout (triangle_strip, max_vertices=1024) out;

/* Engine default uniforms */
uniform mat4 matmodelview;
uniform mat4 matprojection;
 
in VS_OUT {
    vec3 normal;
} gs_in[3];
 
out GS_OUT {
    vec3 normal;
} gs_out;

vec3 up = vec3( 0, 1, 0 );

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


void 
emitAndMult( vec3 v, vec3 n ) {

	gs_out.normal =mat3(matmodelview) * n;
	
	gl_Position =matprojection * matmodelview * vec4(v.x,v.y, v.z, 1.0);
	EmitVertex();
}

mat4 mat_ring;

void 
emitRing( vec3 pivot1, vec3 pivot2 ) {

	mat4 mat_ring2 =rotationMatrix( pivot1-pivot2, radians( 360 / RING_SECTIONS ) );
	
	vec4 v1,v2;
	
	v1 = v2 = vec4( normalize( cross( pivot1, pivot2 ) ) * RADIUS, 1 );

	for( int i =0; i < RING_SECTIONS + 1; i++ ) {

		emitAndMult ( v1.xyz + pivot1 , v1.xyz );
		
		emitAndMult ( v2.xyz + pivot2, v2.xyz);
		
		v2 = mat_ring * v2;
		v1 = mat_ring2 * v1;
		
	}
	EndPrimitive();
	
	mat_ring =mat_ring2;
}

// It calculates a point in the bezier curve at the
// time t. c1, c2 and c3 are the control points
vec3 
bezier2(
  float t,
  const vec3 c1,
  const vec3 c2,
  const vec3 c3)
{
  return  pow( 1.0-t, 2 ) * c1 + 2*(1.0-t)*t*c2 + pow( t, 2 ) * c3;
}
 
void 
main()
{
  for(int i = 0; i < gl_in.length(); i++)
  {  
    emitAndMult( gl_in[i].gl_Position.xyz, gs_in[i].normal );
  }
  EndPrimitive();
  
  vec3 base =gl_in[0].gl_Position.xyz;
  vec3 shoulder =gl_in[1].gl_Position.xyz;
  vec3 head =gl_in[2].gl_Position.xyz;
  
  up = normalize(base - head);
  
/*  emitRing( base, shoulder );
  EndPrimitive();
  
  emitRing( shoulder, head );
  EndPrimitive();*/
  
  float t;
  vec3 v1, v2;
  
  /* Curve section */
  
  mat_ring =rotationMatrix( up, radians( 360 / RING_SECTIONS ) );
  
  //v1 =vec3( sin(T_STEP)/T_STEP, cos(T_STEP)/T_STEP, 0 );
  v1 =bezier2( 0, base, shoulder, head );
  
  ///for( t =T_STEP*2; t < T_MAX; t+=T_STEP ) {
  for( t =0.1; t <= 1.0 ; t+=0.1 ) {
  
  	v2 = v1;
  	//v1 = vec3( sin(t)/t, cos(t)/t, 0 );
  	v1 = bezier2( t, base, shoulder, head );
  	emitRing( v1, v2 );
  	
  }
  
  /* Curl section */
  
  vec3 curve_end =v1;
  
  up = normalize(v2 - v1);
  mat_ring =rotationMatrix( up, radians( 360 / RING_SECTIONS ) );
  
  v1 =vec3( sin(T_BEGIN)/T_BEGIN, cos(T_BEGIN)/T_BEGIN, 0 );
  v2 =vec3( sin(T_BEGIN+T_STEP)/(T_BEGIN+T_STEP), cos(T_BEGIN+T_STEP)/(T_BEGIN+T_STEP), 0 );
  
  vec3 curl_up =normalize( v1 - v2);
  
  //float theta = acos(dot( up, vec3( 0,1,0 ) ));
  //vec3 axis =cross( up, vec3( 0,1,0 ) );
  //vec3 axis =up-vec3(0,1,0);
  
  vec3 v = cross( up, curl_up );
  float s = length( v );
  float c = dot( up, curl_up );
  
  mat3 I =mat3(1);
  mat3 vx = mat3( 0, 		-v.z, v.y,
  								v.z,	0		, -v.x,
  								-v.y, v.x, 0 );
  								
  mat3 rot = I + vx + vx * vx * ((1-c)/pow(s,2));
  
//  mat4 rot =rotationMatrix( axis, -theta ); 
  
  

  vec3 offset =curve_end - rot * v1;
   
  emitRing( (rot*vec3(v2)).xyz + offset, curve_end );
  
  for( t =T_BEGIN+T_STEP; t < T_MAX; t+=T_STEP ) {
  
  	v2 = v1;
  	v1 = vec3( sin(t)/t, cos(t)/t, 0 ); 

  	emitRing( (rot*vec3(v1)).xyz + offset, (rot*vec3(v2)).xyz + offset );
  	
  }
  
  EndPrimitive();
}