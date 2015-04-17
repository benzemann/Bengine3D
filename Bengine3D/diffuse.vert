#version 150

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMat;
uniform vec4 lightPos;
uniform mat4 depthVP;
uniform mat4 bias;
uniform vec3 userPos;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 normalV;
out vec3 lightDir;
out vec3 eyeV;
out vec4 shadowCoord;
out vec2 texCoordV;

void main (void)
{
	texCoordV = texCoord;
    normalV = mat3(model) * normal;
	eyeV = vec3(model * vec4(position,1.0f)) - userPos;
	
	mat4 modelView = view * model;
	lightDir = lightPos.xyz;
	mat4 shadowMVP = depthVP * model;
	shadowMVP = bias * shadowMVP;
	shadowCoord =  shadowMVP * vec4(position,1.0f);
	gl_Position = projection * modelView * vec4(position, 1.0f);
}