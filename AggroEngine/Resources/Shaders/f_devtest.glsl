#version 430

uniform sampler2D tex;
in vec2 texCoord;
in vec3 normal;
layout(location = 0) out vec4 fragColor;

void main() {
	vec4 texcolor = texture2D(tex,texCoord);
	vec3 lightDir = vec3(-0.5,-1.0,-0.7);
	vec3 direction = -normalize(lightDir);
	float ambience = 0.5;
	float diffuse = clamp(dot(normal,direction.xyz),0.0,1.0);
	fragColor = vec4((ambience+diffuse)*texcolor.rgb, 1.0);
} 