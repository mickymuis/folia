#version 330 core

layout(lines_adjacency) in;
layout (triangle_strip, max_vertices=78) out;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

uniform vec3 wind_1;
uniform vec3 wind_2;
const vec3 up = vec3( 0, 1, 0 );
 
in VS_OUT {
    vec3 param;
   // mat3 mat_normal;
} gs_in[4];
 
out GS_OUT {
    vec3 normal;
    vec3 texCoords;
    vec3 tangent;
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
emitAndMult( vec3 v, vec3 n, vec3 uv, vec3 t ) {
	gs_out.normal = mat3(mat_model) * n;
	gs_out.texCoords = uv;
	gl_Position = mat_projection * mat_view * mat_model * vec4( v, 1 );
	gs_out.tangent =mat3(mat_model) * t;
	EmitVertex();
}

vec3
displace( vec3 v, float seed ) {
	float factor = pow( length( v ) * 0.02, 2 );
	vec3 v1 = wind_1 * factor;
	vec3 v2 = wind_2 * factor;
	
	return v + ( v1 * (1.0 - seed) + v2 * abs(seed) );
}

float blendFactor( float radius ) {
	return clamp( radius * 8, 0.0, 1.0 );
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
	
	float base_radius = max( gs_in[0].param.x, 0.01);
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
	
	sections = int(gs_in[3].param.x);
	float r_step = radians( 360.0 / float(sections) );
	float l_step = 1.0 / gs_in[3].param.y;
	//float reserved = gs_in[3].param.z;
	
	float radius_step = head_radius - base_radius;
	float radius1, radius2;
	
	float rough_step = head_rough - base_rough;
	float rough1, rough2;
	
	vec3 v1, v2, n1, n2, n1a, n2a;
	mat3 rot1, rot2;
	vec3 t1, t2;
	float tex_u, tex_v1, tex_v2 =1.0;
	float blend1, blend2;
	
	v2 = base;
	rot2 = rotationMatrix( base - prev, r_step );
	n2a = normalize( cross( base - prev, vec3(0.1,0.1,0.1) ) ) * base_radius;
	radius2 = base_radius;
	rough2 = base_rough;
	t2 =base - prev;
	blend2 = blendFactor( base_radius );
		
	for( float t =l_step; t <= 1.0; t += l_step ) {
	
		v1 = v2;
		rot1 = rot2;
		n1a = n1 = n2a;
		radius1 = radius2;
		rough1 = rough2;
		t1 = t2;
		tex_v1 = tex_v2;
		blend1 = blend2;
		
		tex_v2 = 1.0 - t;
		
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
	
		t2 = v2 - v1;
		blend2 = blendFactor( radius2 );
		
		for( int i =0; i < sections; i++ ) {
		
			tex_u = 1.0 - float(i) / float(sections);
		
			emitAndMult( 
				v1 + n1 * roughness( r_step, i, radius1, rough1), 
				n1, 
				vec3( tex_u, tex_v1, blend1 ),
				t1 );
			emitAndMult( 
				v2 + n2 * roughness( r_step, i, radius2, rough2), 
				n2, 
				vec3( tex_u, tex_v2, blend2 ), 
				t2 );
			
			
			
			n1 = rot1 * n1;
			n2 = rot2 * n2;
		}
			
		emitAndMult( 
			v1 + n1a * roughness( r_step, 0, radius1, rough1), 
			n1a, 
			vec3( 0.0, tex_v1, blend1 ),
			t1 );
		emitAndMult( 
			v2 + n2a * roughness( r_step, 0, radius2, rough2), 
			n2a, 
			vec3( 0.0, tex_v2, blend2 ),
			t2 );
		
		EndPrimitive();
	}

}