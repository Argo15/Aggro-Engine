#version 430

struct Material
{
	vec3 color;
	sampler2D tex;
};

uniform vec3 objId;
uniform bool lightingEnabled;
uniform Material material;
in vec3 normal;
in float normal_length;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 albedoBuffer;
out vec4 selectionBuffer;

vec4 normalColor(vec3 normal, float normal_length, bool lightingEnabled) {
	vec4 normalCol;
	if (normal_length < 0.01 || !lightingEnabled) {
		normalCol = vec4(0, 0, 0, 1.0);
	} else {
		normalCol = vec4(normalize(normal)*vec3(0.5)+vec3(0.5), 1.0);
	}
	return normalCol;
}

void main() {
	vec4 texcolor = texture2D(material.tex, texCoord);
	normalBuffer = normalColor(normal, normal_length, lightingEnabled);
	albedoBuffer = vec4(texcolor.rgb * material.color, 1.0);
	selectionBuffer = vec4(objId,1.0);
}