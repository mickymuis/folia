#version 330 core

float RING_SECTIONS = 6.0;
float RADIUS = 0.05;
float T_STEP = 0.15;
float T_MAX = 24.0;
float T_BEGIN = 0;

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

mat4 MAT_RING;
vec4 RING_LAST;

void 
emitRing( vec3 pivot1, vec3 pivot2, float taper ) {

	mat4 mat_ring2 =rotationMatrix( pivot2-pivot1, radians( 360 / RING_SECTIONS ) );
	
	vec4 v1,v2;
	
	v1 = RING_LAST;
	v2 = vec4( normalize( cross( pivot2-pivot1, vec3(0.1,0.1,0.1) ) ) * RADIUS / taper, 1 );
	RING_LAST =v2;

	for( int i =0; i < RING_SECTIONS + 1; i++ ) {

		emitAndMult ( v1.xyz + pivot1 , v1.xyz );
		
		emitAndMult ( v2.xyz + pivot2, v2.xyz);
		
		v2 = mat_ring2 * v2;
		v1 = MAT_RING * v1;
		
	}
	EndPrimitive();
	
	MAT_RING =mat_ring2;
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

vec3 bezier3( float t, const vec3 v0, const vec3 v1, const vec3 v2, const vec3 v3 )
{
    vec3 p;
    float OneMinusT = 1.0 - t;
    float b0 = OneMinusT*OneMinusT*OneMinusT;
    float b1 = 3.0*t*OneMinusT*OneMinusT;
    float b2 = 3.0*t*t*OneMinusT;
    float b3 = t*t*t;
    return b0*v0 + b1*v1 + b2*v2 + b3*v3;
}
 
void 
main()
{
  /*for(int i = 0; i < gl_in.length(); i++)
  {  
    emitAndMult( gl_in[i].gl_Position.xyz, gs_in[i].normal );
  }
  EndPrimitive();*/
  
  vec3 base =gl_in[0].gl_Position.xyz;
  vec3 shoulder =gl_in[1].gl_Position.xyz;
  vec3 head =gl_in[2].gl_Position.xyz;
  
  up = normalize(base - head);
  
  
  /* Determine radius based on overall size */
  
  RADIUS = length( base - head ) * 0.02;
  
  /* Determine LOD based on perspective */
  
  float lod = (matprojection * matmodelview * vec4(( base + shoulder + head ) / 3, 1)).w;
    
  float t;
  vec3 v1, v2;
  
  /* Curve section */
  
  MAT_RING =rotationMatrix( up, radians( 360 / RING_SECTIONS ) );
  
  RING_LAST.xyz = v2 = base;
  
  for( t =0; t <= 1.0 ; t+=0.05 ) {
  
  	v1 = v2;
  	//v2 = bezier3( t, base, shoulder, shoulder + vec3(0.05, 0.05, 0.05), head );
  	v2 = bezier2( t, base, shoulder, head );
  	emitRing( v1, v2, 1 );
  	
  }
  
  /* Curl section */
  
  vec3 curve_end =v2;
  
  up = normalize(v2 - v1);
  
  v1 =vec3( sin(T_BEGIN), cos(T_BEGIN), 1 );
  v2 =vec3( sin(T_BEGIN+T_STEP)/(T_BEGIN+T_STEP+1), cos(T_BEGIN+T_STEP)/(T_BEGIN+T_STEP+1), 1 );
  
  vec3 curl_up =normalize(
  	v2-v1
	);
  
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
  

  vec3 offset =curve_end - rot * v1;
   
  emitRing( curve_end, (rot*vec3(v2)).xyz + offset, 1 );
  
  for( t =T_BEGIN+T_STEP*2; t < T_MAX; t+=2*T_STEP ) {
  
  	v1 = v2;
  	v2 = vec3( sin(t)/(t+1), cos(t)/(t+1), 1 ); 

  	emitRing( (rot*vec3(v1)).xyz + offset, (rot*vec3(v2)).xyz + offset, 1 + pow(t,2) / T_MAX );
  	
  }
  
  EndPrimitive();
}