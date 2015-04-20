#version 150

uniform vec3 lightIntensity[2];
uniform vec4 ambient;
uniform sampler2D tex;
uniform sampler2DShadow shadowTexture;
uniform sampler2DShadow shadowTexture2;
uniform mat4 lightViewProjection;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;
uniform vec2 texScale;
uniform int isTextured;
uniform float lightConeAngle[2];
uniform vec3 lightDirection[2];
uniform vec4 lightPos[2];

in vec2 texCoordV;
in vec3 worldPos;
in vec3 normalV;
in vec4 lightDir[2];
in vec3 eyeV;
in vec4 shadowCoord[2];
out vec4 fragColor;

void main(void)
{
	vec3 N = normalize(normalV);
	vec3 E = normalize(eyeV);
	vec3 lightSummed = vec3(0.0f);
	for(int i = 0; i < 2; i++){
		vec3 L = normalize(lightDir[i].xyz);
		float lambertian = max(dot(N, L),0.0f);
		vec3 diffuse = lightIntensity[i] * lambertian;
		vec3 R = reflect(-L,N);
		float specularCoefficient = 0.0f;
		if(lambertian > 0.0f)
			specularCoefficient = pow(max(dot(E, R), 0.0f), shininess);
		vec3 specular = specularCoefficient * specularColor.xyz * lightIntensity[i];
		float visibility = 1.0f;
		vec4 sCoord = shadowCoord[i];
		sCoord.z -= 0.005f;
		float attenuation = 1.0f;
		if(lightDir[i].w == 0.0f){
			float distanceToLight = length(worldPos-lightPos[i].xyz);
			attenuation = 1.0f / (1.0f + 0.1f * pow(distanceToLight, 2));
			vec3 coneDir = normalize(lightDirection[i] - lightPos[i].xyz);
			float lightToSurfaceAngle = degrees(acos(dot(-L,coneDir)));
			if(lightToSurfaceAngle > lightConeAngle[i]){
				attenuation = 0.0f;
			}
			if(i == 0){
				visibility = texture(shadowTexture, sCoord.xyz/sCoord.w);
			} else {
				visibility = texture(shadowTexture2, sCoord.xyz/sCoord.w);
			}
		} else {
			if(i == 0){
				visibility = texture(shadowTexture, sCoord.xyz);
			} else {
				visibility = texture(shadowTexture2, sCoord.xyz);
			}
		}
		lightSummed += attenuation*visibility*(diffuse + specular);
	}
	vec2 tcoords = texCoordV;
	tcoords = vec2(tcoords.x * texScale.x,tcoords.y * texScale.y) ;
	vec3 texture = vec3(1.0f);
	if(isTextured != 0)
		texture = texture2D(tex, tcoords).rgb;
	
	vec3 finalColor = texture * diffuseColor.xyz * (ambient.xyz + ((lightSummed)));
	
    fragColor = vec4(finalColor,1.0f);
}