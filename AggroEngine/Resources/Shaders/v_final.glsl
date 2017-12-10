#version 430

uniform mat4 modelViewProjectionMatrix;
in vec3 v_vertex;
in vec2 v_texture;
out vec2 texCoord;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(v_vertex, 1.0);
	texCoord = v_texture;
}