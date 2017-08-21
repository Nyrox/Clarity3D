#version 410 core

layout(location = 0) out vec4 color;

in vec3 worldNormal;
in vec3 fragPos;

uniform vec3 fillColor;

void main() {
	vec3 lightPos = vec3(2, 1.5, 2);
	
	vec3 N = normalize(worldNormal);
	vec3 L = fragPos - lightPos;
	
	float diff = max(dot(N, L), 0.0) + 0.10;
	float d = distance(fragPos, lightPos);
	float attenuation = 1 / (d*d);
	
	color = vec4(vec3(diff * 2.0 * (fillColor * 0.5) * attenuation), 1.0);
}