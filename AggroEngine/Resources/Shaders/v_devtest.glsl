#version 430

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
layout(location = 0) in vec3 v_a_vertex; // alphabetize, better safe than sorry
layout(location = 1) in vec2 v_b_texcoord;
layout(location = 2) in vec3 v_c_normal;
out vec2 texCoord;
out vec3 normal;

void main() {
    texCoord = v_b_texcoord;
    normal = normalMatrix * v_c_normal;
	gl_Position = modelViewProjectionMatrix * vec4(v_a_vertex,1.0);
}