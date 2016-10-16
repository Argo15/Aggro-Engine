#version 430

uniform sampler2D tex;
uniform vec3 objId;
in vec3 normal;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 albedoBuffer;
out vec4 selectionBuffer;

void main() {
	vec4 texcolor = texture2D(tex,texCoord);
	normalBuffer = vec4(normal*vec3(0.5)+vec3(0.5),1.0);
	albedoBuffer = vec4(texcolor.rgb,1.0);
	selectionBuffer = vec4(objId,1.0);
} 