#version 330 core

layout(lines_adjacency) in;
layout (triangle_strip, max_vertices=113) out;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;
 
in VS_OUT {
    vec3 param;
} gs_in[4];
 
out GS_OUT {
    vec3 normal;
    vec2 texCoords;
} gs_out;

int sections = 6;

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

float roughness( float r_step, int i, float radius, float rough ) {
	return 1.0 + cos( r_step * i * sections * 0.25) * 0.50 * radius * rough;
}

void
emitAndMult( vec3 v, vec3 n, vec2 t ) {
	gs_out.normal = mat3(mat_model) * n;
	gs_out.texCoords = t;
	gl_Position = mat_projection * mat_view * mat_model * vec4( v, 1 );
	EmitVertex();
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
			SECTIONS,		STEPSIZE,			RESERVED
			*/
	
	vec3 prev = gl_in[0].gl_Position.xyz;
	vec3 base = gl_in[1].gl_Position.xyz;
	vec3 head = gl_in[2].gl_Position.xyz;
	vec3 next = gl_in[3].gl_Position.xyz;
	
	float base_radius = gs_in[0].param.x;
	float base_rough = gs_in[0].param.y;
	float base_seed = gs_in[0].param.z;
	
	float head_radius = gs_in[1].param.x;
	float head_rough = gs_in[1].param.y;
	float head_seed = gs_in[1].param.z;
	
	vec3 curve = (base+head) / 2 + gs_in[2].param;
	
	sections = int(gs_in[3].param.x);
	float r_step = radians( 360.0 / float(sections) );
	float l_step = 1.0 / gs_in[3].param.y;
	float reserved = gs_in[3].param.z;
	
	float radius_step = head_radius - base_radius;
	float radius1, radius2;
	
	float rough_step = head_rough - base_rough;
	float rough1, rough2;
	
	vec3 v1, v2, n1, n2, n1a, n2a;
	mat3 rot1, rot2;
	vec2 t1 =vec2(0), t2;
	
	v2 = base;
	rot2 = rotationMatrix( base - prev, r_step );
	n2a = normalize( cross( base - prev, vec3(0.1,0.1,0.1) ) ) * base_radius;
	radius2 = base_radius;
	rough2 = base_rough;
		
	for( float t =l_step; t <= 1.0; t += l_step ) {
	
		v1 = v2;
		rot1 = rot2;
		n1a = n1 = n2a;
		radius1 = radius2;
		rough1 = rough2;
		
		if( t == 1.0 ) {
			v2 = head;
			rough2 = head_rough;
			radius2 = head_radius;
			rot2 = rotationMatrix( head - base, r_step );
			n2a = n2 = normalize( cross( head - base, vec3(0.1,0.1,0.1) ) ) * head_radius;		
		} else {
			v2 = bezier2( t, base, curve, head );
			radius2 = base_radius + pow(t,2) * radius_step;
			rough2 = base_rough + t * rough_step;
			rot2 = rotationMatrix( v2 - v1, r_step );
			n2a = n2 = normalize( cross( v2 - v1, vec3(0.1,0.1,0.1) ) ) * radius2;
		}	
	
		for( int i =0; i < sections; i++ ) {
		
		
			emitAndMult( v1 + n1 * roughness( r_step, i, radius1, rough1), n1, t1 );
			emitAndMult( v2 + n2 * roughness( r_step, i, radius2, rough2), n2, t1 );
			
			n1 = rot1 * n1;
			n2 = rot2 * n2;
		}
			
		emitAndMult( v1 + n1a * roughness( r_step, 0, radius1, rough1), n1a, t1 );
		emitAndMult( v2 + n2a * roughness( r_step, 0, radius2, rough2), n2a, t1 );
		
		EndPrimitive();
	}
	
/*	v1 = v2;
	rot1 = rot2;
	n1a = n1 = n2a;
	radius1 = radius2;
	rough1 = rough2;
	
	

	for( int i =0; i < sections; i++ ) {
	
		emitAndMult( v1 + n1 * roughness( r_step, i, radius1, rough1), n1, t1 );
		emitAndMult( v2 + n2 * roughness( r_step, i, radius2, rough2), n2, t1 );
		
		n1 = rot1 * n1;
		n2 = rot2 * n2;
	}
	
	emitAndMult( v1 + n1a * roughness( r_step, 0, radius1, rough1), n1a, t1 );
	emitAndMult( v2 + n2a * roughness( r_step, 0, radius2, rough2), n2a, t1 );
	
	EndPrimitive();*/
}