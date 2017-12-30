#version 430

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform mat4 textureMatrix;
in vec3 v_vertex;
in vec2 v_texture;
in vec3 v_normal;
out vec3 normal;
out vec2 texCoord;
out float normal_length;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(v_vertex, 1.0);
	texCoord = vec2(textureMatrix * vec4(v_texture,0.0,1.0));
	normal = normalMatrix * normalize(v_normal);
	normal_length = length(v_normal);
}