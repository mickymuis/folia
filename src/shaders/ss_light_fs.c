/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */


#version 330 core

#define NUM_SAMPLES 2
#define MULTISAMPLE

out vec4 fragColor;

#ifdef MULTISAMPLE
uniform sampler2DMS g_depth;
uniform sampler2DMS g_diffusespec;
uniform sampler2DMS g_normal;
uniform sampler2DMS g_position;
#else
uniform sampler2D g_depth;
uniform sampler2D g_diffusespec;
uniform sampler2D g_normal;
uniform sampler2D g_position;
#endif

uniform vec3 view_position;

uniform vec3 light_position;
uniform vec3 light_intensity;
uniform vec3 light_direction;
uniform vec3 light_attenuation;

uniform mat4 mat_view;
uniform mat4 mat_windowtoview;

vec2 ndc_position;

void main()
{             

#ifdef MULTISAMPLE
	ivec2 screensize =textureSize( g_depth /*,0*/ );
#else
	ivec2 screensize =textureSize( g_depth, 0 );
#endif
	ndc_position = ( gl_FragCoord.xy / screensize ) * vec2(1) - vec2(1);

  // Retrieve data from gbuffer
#ifndef MULTISAMPLE
  //vec3 f_position = vec3(vec4(positionFromDepth( 0 ),1));
  
  vec4 fragCoord =texture( g_position, ndc_position).rgba;
  vec3 f_normal = mat3(mat_view)*texture(g_normal, ndc_position).rgb;
	vec3 f_diffuse = texture(g_diffusespec, ndc_position).rgb;
	float f_specular = texture(g_diffusespec, ndc_position).a;
#endif
	
	vec3 color =vec3(0);

#ifdef MULTISAMPLE
	/* For MS, we average over all the samples */
  for( int i=0; i < NUM_SAMPLES; i++ ) {
  
  	vec4 fragCoord =texelFetch( g_position, ivec2(gl_FragCoord.xy), i).rgba;
  	vec3 f_normal = mat3(mat_view) * texelFetch( g_normal, ivec2(gl_FragCoord.xy), i).rgb;
		vec3 f_diffuse = texelFetch( g_diffusespec, ivec2(gl_FragCoord.xy), i).rgb;
		float f_specular = texelFetch( g_diffusespec, ivec2(gl_FragCoord.xy), i).a;
#endif

		/* Calculate the NDC from the stored fragCoord,
			 then reconstruct the view-space position using the inverse projection matrix */
		vec4 ndcPos;
		ndcPos.xy = ((2.0 * fragCoord.xy) - (2.0 * vec2(0))) / (screensize) - 1;
		ndcPos.z = (2.0 * fragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
				(gl_DepthRange.far - gl_DepthRange.near);
		ndcPos.w = 1.0;
 
		vec4 clipPos = ndcPos / fragCoord.w;
	
		vec3 f_position = vec3(mat_windowtoview * clipPos);

		float attenuation = 1.0;
		vec3 diffuse;
		vec3 specular;
		vec3 lighting  = f_diffuse * 0.001; // hard-coded ambient component
		vec3 viewDir  = normalize( - f_position);
	
		if( light_direction.xyz == vec3(0) ) { // assumes point-light
	
			// Diffuse
			vec3 lightDir = normalize(light_position - f_position);
			diffuse = max(dot(f_normal, lightDir), 0.0) * f_diffuse * light_intensity;

			// Specular
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(f_normal, halfwayDir), 0.0), 16.0);
			specular = light_intensity * spec * f_specular;

			// Attenuation
			float distance = length(light_position - f_position);
			attenuation = 1.0 / ( 
				light_attenuation.x +  // constant
				light_attenuation.y * distance +  // linear
				light_attenuation.z * distance * distance); // quadratic
		
		} else { // directional light

			lighting  = f_diffuse * 0.1; // DELETE ME
			vec3 lightDir = normalize(-light_direction);  
			float diff = max(dot(f_normal, lightDir), 0.0);
			diffuse = light_intensity * diff * f_diffuse;  
		
			// Specular
			vec3 reflectDir = reflect(-lightDir, f_normal);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0 );
			specular = light_intensity * spec * f_specular;
		}
		
		diffuse *= attenuation;
		specular *= attenuation;
		lighting += diffuse + specular;
		
#ifdef MULTISAMPLE
		color += (1.0 / NUM_SAMPLES) * lighting;
	}
#else
	color =lighting;
#endif
		
	fragColor = vec4(color, 1.0);
}
