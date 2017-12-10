#version 430

uniform sampler2D normalTex;
uniform vec3 lightDir;
in vec2 texCoord;
out vec4 lightBuffer;

void main() {
	vec4 normalColor = texture2D(normalTex, texCoord);
	if (length(normalColor.rgb) < 0.01 || normalColor.w < 0.01)
	{
		lightBuffer = vec4(1.0);
		return;
	}
	vec3 normalDir =  normalColor.xyz * -2 + vec3(1.0);
	float ambience = 0.5;
	float diffuse = max(0, dot(normalDir, lightDir) * (1 - ambience));
	lightBuffer = vec4(vec3(ambience + diffuse), 1.0);
} 