#version 430

struct Material
{
	vec3 color;
	sampler2D tex;
	sampler2D alpha;
	float specIntensity;
	float shininess;
	sampler2D specMap;
	vec3 emission;
	sampler2D emissionMap;
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
out vec4 glowBuffer;

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
	vec4 alphaColor = texture2D(material.alpha, texCoord);
	if (alphaColor.x < 0.1)
	{
		discard;
	}
	vec4 texColor = texture2D(material.tex, texCoord);
	vec4 normCol = normalColor(normal, normal_length, lightingEnabled);
	normalBuffer = vec4(normCol.rgb, material.shininess / 100.0);
	albedoBuffer = vec4(texColor.rgb * material.color, 1.0);
	selectionBuffer = vec4(objId, 1.0);
	vec4 specColor = texture2D(material.specMap, texCoord);
	vec4 emissionColor = texture2D(material.emissionMap, texCoord);
	glowBuffer = vec4(material.emission * emissionColor.rgb, material.specIntensity * specColor.r);
}