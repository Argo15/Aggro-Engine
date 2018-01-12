
uniform mat4 modelViewProjectionMatrix;
in vec3 v_vertex;

void main() {
	gl_Position = modelViewProjectionMatrix * vec4(v_vertex, 1.0);
}