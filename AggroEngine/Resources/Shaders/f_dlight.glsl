#version 430

uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform sampler2D glowTex;
uniform mat4 inverseMVPMatrix;
uniform vec3 lightDir;
uniform vec3 cameraPos;
uniform float near;
uniform float far;
uniform vec3 color;
uniform float ambient;
in vec2 texCoord;
out vec4 lightBuffer;
out vec4 glowBuffer;

void main() {
	// Lighting
	float hyperDepth = texture2D(depthTex,texCoord).r;
	float depth = (2.0 * near) / (far + near - (hyperDepth*2.0-1.0) * (far - near));
	vec4 normalColor = texture2D(normalTex, texCoord);
	if (length(normalColor.rgb) < 0.01 || depth > 0.999)
	{
		lightBuffer = vec4(1.0);
		glowBuffer = vec4(0, 0, 0, 1.0);
		return;
	}
	vec3 normalDir =  normalColor.xyz * -2 + vec3(1.0);
	float diffuse = max(0, dot(normalDir, lightDir) * (1 - ambient));
	lightBuffer = vec4(vec3(ambient + diffuse) * color, 1.0);

	// Specular
	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;
	vec3 lookDir = normalize(cameraPos - worldPos.xyz);
	vec3 reflectedLight = normalize(reflect(lightDir, normalDir));
	float lightDotLook = dot(reflectedLight, lookDir);
	vec4 glowColor = texture2D(glowTex, texCoord);
	float specIntensity = glowColor.a;
	float shininess = 100.0 * normalColor.a;
	if (shininess > 1 && specIntensity > 0)
	{
		float specular = specIntensity * pow(lightDotLook, shininess);
		glowBuffer = vec4(glowColor.rgb + clamp(color * vec3(specular), 0.0, 1.0), 1.0);
	}
	else
	{
		glowBuffer = vec4(glowColor.rgb, 1.0);
	}
} 