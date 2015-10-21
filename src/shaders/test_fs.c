#version 330 core

/* Engine default from vertex pass */
in vec3 position;
in vec3 normal;

/* Engine default fragment pass output */
out vec4 fragcolor;

void main() {
	vec3 light = vec3(0.5, 0.0, 0.0);
	float intensity = abs(normal.x*0.3+ normal.z*0.1 + normal.y *0.6); //"stupid" shading
	
	
	intensity = intensity + 0.1; //ambient lighting
	fragcolor = vec4(intensity*light, 1.0);
}