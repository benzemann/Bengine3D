#version 150

uniform float lightIntensity;
uniform vec4 ambient;
uniform vec4 lightPos;
uniform sampler2DShadow shadowTexture;
uniform sampler2D tex;
uniform mat4 lightViewProjection;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

in vec2 texCoordV;
in vec3 normalV;
in vec3 lightDir;
in vec3 eyeV;
in vec4 shadowCoord;
out vec4 fragColor;

void main(void)
{
	vec3 N = normalize(normalV);
	vec3 L = normalize(lightDir);
	vec3 E = normalize(eyeV);
	float lambertian = max(dot(N, L),0.0f);
	float intensity = lightIntensity;
	vec3 diffuse = intensity * lambertian * vec3(1.0f);
	vec3 R = reflect(-L,E);
	vec3 specular = specularColor.xyz * pow(max(dot(R, E), 0.0f), shininess);

	float visibility = 1.0f;
	vec4 sCoord = shadowCoord;
	sCoord.z -= 0.005f;
	visibility = texture(shadowTexture, sCoord.xyz);

	vec3 temp = texture2D(tex, texCoordV).rgb;

	vec3 finalColor = diffuseColor.xyz * (ambient.xyz + (visibility*(diffuse + specular)));
    fragColor = vec4(finalColor,1.0f);
}