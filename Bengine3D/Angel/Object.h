#pragma once 
#ifndef OBJECT_H
#define OBJECT_H
#include "Angel.h"
#include "Shader.h"
#include "Material.h"

class Object 
{
public:
	Object() :
		position(vec3(0.0f)), scale(vec3(1.0f)), rotAngles(vec3(0.0f))
	{}
	Object(vec3 position, vec3 scale) :
		position(position), scale(scale), rotAngles(vec3(0.0f))
	{}
	Object(vec3 position, vec3 scale, Material mat) :
		position(position), scale(scale), material(mat)
	{}
	Object(vec3 position, vec3 scale, Material mat, vec3 rot) :
		position(position), scale(scale), material(mat), rotAngles(rot)
	{}

	virtual void setPos(vec3 v){
		position = v;
	};
	virtual vec3 getPos(){ return position; };
	virtual void scaleObj(vec3 v){
		scale = v;
	};
	virtual vec3 getScale(){ return scale; };
	virtual void setScale(vec3 s){ scale = s; };
	virtual vec3 getRotation(){ return rotAngles; };
	virtual void setRotation(vec3 r){ rotAngles = r; };
	virtual void rotate(char axis, float angle){
		switch (axis)
		{
		case 'X':
			rotAngles.x = angle;
			if (rotAngles.x > 360){
				rotAngles.x -= 360;
			}
			break;
		case 'Y':
			rotAngles.y = angle;
			if (rotAngles.y > 360){
				rotAngles.y -= 360;
			}
			break;
		case 'Z' :
			rotAngles.z = angle;
			if (rotAngles.z > 360){
				rotAngles.z -= 360;
			}
			break;
		default:
			break;
		}
	}
	virtual void setVertexArrayBuffer(GLuint vao_input, int _number_of_vertices){
		vao = vao_input;
		number_of_vertices = _number_of_vertices;
	};
	virtual GLuint getVertexArrayBuffer(){ return vao; };
	virtual int getNumberOfVertices(){
		return number_of_vertices;
	};
	virtual mat4 getModelMatrix(){
		mat4 model = mat4(1.0f);
		mat4 rotation = RotateX(rotAngles.x) * RotateY(rotAngles.y) * RotateZ(rotAngles.z);
		return Translate(position) * Scale(scale) * rotation;
	};
	virtual void setMaterial(Material m){ material = m; };
	virtual Material getMaterial(){ return material; };
	virtual void draw(Shader& shader, mat4 view);

private:
	vec3 position;
	vec3 scale;
	vec3 rotAngles;
	Material material;
	GLuint vao;
	int number_of_vertices;
};
#endif

