#version 330 core

out vec4 fragColor;
//in vec2 texCoords;

uniform sampler2D g_depth;
uniform sampler2D g_diffusespec;
uniform sampler2D g_normal;

uniform vec3 view_position;
uniform vec3 light_position;
uniform vec3 light_intensity;

uniform mat4 mat_screentoworld;

vec2 ndc_position;

vec3 positionFromDepth( ) {

//	vec2 screenpos = texCoords;
	vec2 screenpos =ndc_position;
	float depth =texture(g_depth, screenpos).r * 2.0 - 1.0;
	vec4 pos = mat_screentoworld * vec4( screenpos, depth, 1.0 );
	return pos.xyz;
}

void main()
{             

	ivec2 screensize =textureSize( g_depth, 0 );
	ndc_position = ( gl_FragCoord.xy / screensize ) * vec2(1) - vec2(1);

    // Retrieve data from gbuffer
  vec3 f_position = positionFromDepth( );
  vec3 f_normal = texture(g_normal, ndc_position).rgb;
	vec3 f_diffuse = texture(g_diffusespec, ndc_position).rgb;
	float f_specular = texture(g_diffusespec, ndc_position).a;
    
//	fragColor = vec4( f_normal, 1 );    
	//fragColor = vec4( 1, 0, 0, 1 );    

	vec3 lighting  = f_diffuse * 0.1; // hard-coded ambient component
	vec3 viewDir  = normalize(view_position - f_position);

	// Diffuse
	vec3 lightDir = normalize(light_position - f_position);
	vec3 diffuse = max(dot(f_normal, lightDir), 0.0) * f_diffuse * light_intensity;
	// Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	float spec = pow(max(dot(f_normal, halfwayDir), 0.0), 16.0);
	vec3 specular = light_intensity * spec * f_specular;
	// Attenuation
	float distance = length(light_position - f_position);
//	float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
	//diffuse *= attenuation;
	//specular *= attenuation;
	lighting += diffuse + specular;

	fragColor = vec4(lighting, 1.0);
}