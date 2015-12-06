#version 330 core

out vec4 fragColor;
//in vec2 texCoords;

uniform sampler2D g_depth;
uniform sampler2D g_diffusespec;
uniform sampler2D g_normal;

uniform vec3 view_position;

uniform vec3 light_position;
uniform vec3 light_intensity;
uniform vec3 light_direction;
uniform vec3 light_attenuation;

uniform mat4 mat_view;
uniform mat4 mat_windowtoview;

vec2 ndc_position;

vec3 positionFromDepth( ) {

	vec2 screenpos =ndc_position;
	float depth =texture(g_depth, screenpos).r * 2.0 - 1.0;
	vec4 pos = mat_windowtoview * vec4( screenpos, depth, 1.0 );
	return pos.xyz;
}

void main()
{             

	ivec2 screensize =textureSize( g_depth, 0 );
	ndc_position = ( gl_FragCoord.xy / screensize ) * vec2(1) - vec2(1);

  // Retrieve data from gbuffer
  vec3 f_position = vec3(mat_view*vec4(positionFromDepth( ),1));
  vec3 f_normal = mat3(mat_view)*texture(g_normal, ndc_position).rgb;
	vec3 f_diffuse = texture(g_diffusespec, ndc_position).rgb;
	float f_specular = texture(g_diffusespec, ndc_position).a;
    
 

	float attenuation = 1.0;
	vec3 diffuse;
	vec3 specular;
	vec3 lighting  = f_diffuse * 0.01; // hard-coded ambient component
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

	fragColor = vec4(lighting, 1.0);
}