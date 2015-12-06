#version 330 core

layout(triangles) in;
layout (triangle_strip, max_vertices=113) out;

/* Engine default uniforms */
uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

uniform vec3 wind_1;
uniform vec3 wind_2;
 
in VS_OUT {
    vec3 param;
} gs_in[3];
 
out GS_OUT {
    vec3 normal;
    vec2 texCoords;
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
	gs_out.texCoords = t;
	gl_Position = mat_projection * mat_view * mat_model * vec4( v, 1 );
	EmitVertex();
}

vec3
displace( vec3 v, float seed ) {
	float factor = pow( length( v ) * 0.02, 2 );
	vec3 v1 = wind_1 * factor;
	vec3 v2 = wind_2 * factor;
	
	return v + ( v1 * (1.0 - seed) + v2 * abs(seed) );
}

void main() {

	/* Layout:
			BASE_X,		BASE_Y,		BASE_Z,
			WIDTH,		CONCAVITY,SEED,
			TIP_X,		TIP_Y,		TIP_Z,
			NUM_LEAFS,ROUNDESS, RESERVED,
			UP_X,			UP_Y,			UP_Z,
			RESERVED,	RESERVED,	RESERVED */
		
	float width = gs_in[0].param.x;
	float concavity = gs_in[0].param.y;
	float seed = gs_in[0].param.z;
	
	vec3 base = displace( gl_in[0].gl_Position.xyz, seed );
	vec3 tip = displace( gl_in[1].gl_Position.xyz, seed );
	vec3 up = normalize( displace( gl_in[2].gl_Position.xyz, seed ) );
	
	float num_leafs = gs_in[1].param.x;
	float roundness = gs_in[1].param.y;
	
	for( float s =-1.0; s <= 1.0; s += 2.0 ) {
	
		vec3 curve = (base + (tip-base) / 2.0 * roundness ) +  s * normalize( cross( up-tip, up-base ) ) * width + up * (concavity+0.01);
		vec2 t1 = vec2(0);
	
		for( float t =0; t < 1.0; t += 0.2 ) {
			vec3 v1 = bezier2( t, base, base, tip );
			vec3 v2 = bezier2( t, base, curve, tip );
		
			emitAndMult( v1, s * cross( v1, v2), t1 );
			emitAndMult( v2, s * cross( v1, v2 ), t1 );
		
		}
	
		emitAndMult( tip, up, t1 );
	}

}