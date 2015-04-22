#version 150
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

noperspective out vec3 dist;

void main(void){
	vec2 WIN_SCALE = vec2(800/2.0f,600/2.0f);
	vec2 vert0 = WIN_SCALE * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
    vec2 vert1 = WIN_SCALE * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
    vec2 vert2 = WIN_SCALE * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;
    vec2 v0 = vert2-vert1;
    vec2 v1 = vert2-vert0;
    vec2 v2 = vert1-vert0;
    float area = abs(v1.x*v2.y - v1.y * v2.x);
    dist = vec3(area/length(v0),0,0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    dist = vec3(0,area/length(v1),0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    dist = vec3(0,0,area/length(v2));
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
}