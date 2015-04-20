#version 150

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMat;
uniform vec4 lightPos[2];
uniform mat4 depthVP[2];
uniform mat4 bias[2];
uniform vec3 userPos;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 normalV;
out vec4 lightDir[2];
out vec3 eyeV;
out vec4 shadowCoord[2];
out vec2 texCoordV;
out vec3 worldPos;

void main (void)
{
	texCoordV = texCoord;
    normalV = mat3(model) * normal;
	vec3 worldSurface = vec3(model * vec4(position,1.0f));
	worldPos = worldSurface;
	eyeV = userPos - worldSurface;
	mat4 modelView = view * model;
	for(int i = 0; i < 2; i++){
		if(lightPos[i].w == 0){
			lightDir[i] = vec4(lightPos[i].xyz - worldSurface,0.0f);
		}else{
			lightDir[i] = lightPos[i];
		}
		mat4 shadowMVP = depthVP[i] * model;
		shadowMVP = bias[i] * shadowMVP;
		shadowCoord[i] =  shadowMVP * vec4(position,1.0f);
	}
	gl_Position = projection * modelView * vec4(position, 1.0f);
}