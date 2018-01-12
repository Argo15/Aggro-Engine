#version 430

uniform sampler2D normalTex;
uniform sampler2D depthTex;
uniform sampler2D glowTex;
uniform sampler2DShadow shadowMap[4];
uniform mat4 shadowMat[4];
uniform float shadowSize;
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
	float hyperDepth = texture2D(depthTex, texCoord).r;
	float depth = (2.0 * near) / (far + near - (hyperDepth*2.0-1.0) * (far - near));
	vec4 normalColor = texture2D(normalTex, texCoord);
	if (length(normalColor.rgb) < 0.01 || depth > 0.999)
	{
		lightBuffer = vec4(1.0);
		glowBuffer = vec4(0, 0, 0, 1.0);
		return;
	}
	
	vec4 screenPos = vec4(texCoord.s*2.0-1.0, texCoord.t*2.0-1.0, hyperDepth*2.0-1.0, 1.0);
	vec4 worldPos = inverseMVPMatrix * screenPos;
	worldPos /= worldPos.w;

	// Shadow
	float sfilter=4.0;
	float xOff=0.0;
	float yOff=0.0;
	float shadowij=((sfilter-0.99)/2.0);
	float shadow=0;
	int shadowIdx=0;
	vec4 shadowCoord = shadowMat[shadowIdx]*worldPos;
	while (shadowCoord.x < 0.0 || shadowCoord.x > 1.0 || shadowCoord.y < 0.0 || shadowCoord.y > 1.0) {
	  shadowIdx++;
	  shadowCoord = shadowMat[shadowIdx]*worldPos;
	}
	if (shadowIdx >= 4) {
		shadow = 1.0;
	} else {
		for (float i=-shadowij; i<=shadowij; i++) {
			for (float j=-shadowij; j<=shadowij; j++) {
				xOff=i/shadowSize;
				yOff=j/shadowSize;
				shadow += textureProj(shadowMap[shadowIdx], shadowCoord + vec4(xOff, yOff, -0.001, 0.0));
			}
		}
		shadow /= sfilter*sfilter;
	}

	// Lighting
	vec3 normalDir = normalColor.xyz * -2 + vec3(1.0);
	float diffuse = max(0, dot(normalDir, lightDir) * (1 - ambient));
	lightBuffer = vec4(vec3(ambient + shadow * diffuse) * color, 1.0);

	// Specular
	vec3 lookDir = normalize(cameraPos - worldPos.xyz);
	vec3 reflectedLight = normalize(reflect(lightDir, normalDir));
	float lightDotLook = dot(reflectedLight, lookDir);
	vec4 glowColor = texture2D(glowTex, texCoord);
	float specIntensity = glowColor.a;
	float shininess = 100.0 * normalColor.a;
	if (shininess > 1 && specIntensity > 0)
	{
		float specular = specIntensity * pow(lightDotLook, shininess);
		glowBuffer = vec4(glowColor.rgb + clamp(shadow * color * vec3(specular), 0.0, 1.0), 1.0);
	}
	else
	{
		glowBuffer = vec4(glowColor.rgb, 1.0);
	}
} 