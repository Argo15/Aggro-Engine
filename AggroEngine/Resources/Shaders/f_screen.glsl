#version 410

uniform sampler2D texture;
in vec2 texCoord;

void main() {
	vec4 texColor = texture2D(texture, texCoord);
	gl_FragColor = vec4(texColor.rgb, 1.0);
} 