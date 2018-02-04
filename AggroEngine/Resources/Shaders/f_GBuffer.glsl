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
	bool hasNormals;
	sampler2D normalMap;
};

uniform vec3 objId;
uniform bool lightingEnabled;
uniform bool hasTangents;
uniform Material material;
uniform mat4 textureMatrix;
uniform mat3 texRotateMatrix;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;
out vec4 normalBuffer;
out vec4 albedoBuffer;
out vec4 selectionBuffer;
out vec4 glowBuffer;

void main() {
	vec4 alphaColor = texture2D(material.alpha, texCoord);
	if (alphaColor.x < 0.1)
	{
		discard;
	}
	vec4 texColor = texture2D(material.tex, texCoord);

	// Normal map
	vec4 normCol = vec4(0, 0, 0, 1.0);
	if (lightingEnabled && length(normal) > 0.01) {
		if (hasTangents && material.hasNormals) {
			mat3 tangmat;
			tangmat[0] = normalize(tangent);
			tangmat[1] = normalize(bitangent);
			tangmat[2] = normalize(normal);
			vec3 normalcolor = texture2D(material.normalMap, texCoord).rgb;
			normalcolor = normalcolor*2.0-vec3(1.0);
			vec3 _normal = normalize(tangmat*texRotateMatrix*normalcolor);
			normCol = vec4(_normal*vec3(0.5)+vec3(0.5), 1.0);
		} else {
			normCol = vec4(normalize(normal)*vec3(0.5)+vec3(0.5), 1.0);
		}
	}
	normalBuffer = vec4(normCol.rgb, material.shininess / 100.0);
	albedoBuffer = vec4(texColor.rgb * material.color, 1.0);
	selectionBuffer = vec4(objId, 1.0);
	vec4 specColor = texture2D(material.specMap, texCoord);
	vec4 emissionColor = texture2D(material.emissionMap, texCoord);
	glowBuffer = vec4(material.emission * emissionColor.rgb, material.specIntensity * specColor.r);
}