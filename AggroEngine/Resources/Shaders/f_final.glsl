#version 430

uniform sampler2D albedoTex;
uniform sampler2D lightingTex;
uniform sampler2D glowTex;
in vec2 texCoord;
out vec4 finalBuffer;

void main() {
	vec4 albedoColor = texture2D(albedoTex, texCoord);
	vec4 lightingColor = texture2D(lightingTex, texCoord);
	vec4 glowColor = texture2D(glowTex, texCoord);
	finalBuffer = vec4(albedoColor.rgb * lightingColor.rgb + glowColor.rgb, 1.0);
} 