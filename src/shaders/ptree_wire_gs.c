#version 330 core

layout(lines_adjacency) in;
layout (line_strip, max_vertices=78) out;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

uniform vec3 wind_1;
uniform vec3 wind_2;
const vec3 up = vec3( 0, 1, 0 );
 
in VS_OUT {
    vec3 param;
} gs_in[4];
 
out GS_OUT {
    vec3 normal;
    vec3 texCoords;
    vec3 tangent;
} gs_out;

mat3 
rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c
               );
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

void
emitAndMult( vec3 v, vec3 n, vec2 t ) {
	gs_out.normal = mat3(mat_model) * n;
	gs_out.texCoords = vec3(t, 0.0);
	gs_out.tangent = vec3(0,1,0);
	gl_Position = mat_projection * mat_model * mat_view * vec4( v, 1 );
	EmitVertex();
}

vec3
displace( vec3 v, float seed ) {
	float factor = pow( length( v ) * 0.02, 2 );
	vec3 v1 = wind_1 * factor;
	vec3 v2 = wind_2 * factor;
	
	return v + ( v1 * (1.0 - seed) + v2 * abs(seed) );
}

void 
main() {

	/* Extract all constants from the input data
		 Layout: 
		  PREV_X,			PREV_Y,				PREV_Z,
			BASE_RADIUS,BASE_ROUGH,		BASE_SEED,
			BASE_X,			BASE_Y,				BASE_Z,
			HEAD_RADIUS,HEAD_ROUGH,		HEAD_SEED,
			HEAD_X,			HEAD_Y,				HEAD_Z,
			CURVE_X,		CURVE_Y,			CURVE_Z,
			NEXT_X,			NEXT_Y,				NEXT_Z,
			SECTIONS,		STEPSIZE,			PREV_SEED
			*/
	
	float base_radius = gs_in[0].param.x;
	float base_rough = gs_in[0].param.y;
	float base_seed = gs_in[0].param.z;
	float prev_seed = gs_in[3].param.z;
	
	float head_radius = gs_in[1].param.x;
	float head_rough = gs_in[1].param.y;
	float head_seed = gs_in[1].param.z;
	
	vec3 prev = displace( gl_in[0].gl_Position.xyz, prev_seed );
	vec3 base = displace( gl_in[1].gl_Position.xyz, base_seed );
	vec3 head = displace( gl_in[2].gl_Position.xyz, head_seed );
	
	vec3 next = gl_in[3].gl_Position.xyz;
	
	vec3 curve = (base+head) / 2 + gs_in[2].param;
	
	float l_step = 1.0 / gs_in[3].param.y;
	
	vec3 v1, v2;
	v2 = base;
		
	for( float t =l_step; t <= 1.0; t += l_step ) {
	
		v1 = v2;
		
		if( t == 1.0 ) {
			v2 = head;
		} else {
			v2 = bezier2( t, base, curve, head );
		}	
				
		emitAndMult( v1, vec3(0,0,1), vec2(0) );
		emitAndMult( v2, vec3(0,0,1), vec2(0) );
		
		EndPrimitive();
	}
	
}
