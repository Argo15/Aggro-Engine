#version 430

uniform sampler2D tex;
uniform vec3 objId;
uniform bool lightingEnabled;
in vec3 normal;
in float normal_length;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 albedoBuffer;
out vec4 selectionBuffer;

void main() {
	vec4 texcolor = texture2D(tex,texCoord);
	float lighting = 1.0;
	if (normal_length < 0.01 || !lightingEnabled)
	{
		normalBuffer = vec4(0, 0, 0, 1.0);
	}
	else
	{
		normalBuffer = vec4(normalize(normal)*vec3(0.5)+vec3(0.5), lighting);
	}
	albedoBuffer = vec4(texcolor.rgb,1.0);
	selectionBuffer = vec4(objId,1.0);
} 