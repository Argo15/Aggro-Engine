#version 430

out vec4 testBuffer;

void main() {
	// Lighting
	float depth = gl_FragCoord.z;
	testBuffer = vec4(depth, depth, depth, 1.0);
} 