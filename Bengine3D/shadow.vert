#version 150
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
in vec3 position;
void main (void)
{
	mat4 MVP = projection * view * model;
	gl_Position = MVP * vec4(position, 1.0f);
}