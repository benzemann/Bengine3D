#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "Angel.h"
#include "Shader.h"

class Light
{
public:
	Light() :
		position(vec3(0.0f, 1.0f, 0.0f)), intensity(vec3(1.0f)), dir(vec3(0.0f)), type(1)
	{}
	Light(vec3 pos) :
		position(vec3(pos)), intensity(vec3(1.0f)), dir(vec3(0.0f)), type(1)
	{}
	Light(vec3 pos, vec3 i) :
		position(vec3(pos)), intensity(i), dir(vec3(0.0f)), type(1)
	{}
	Light(vec3 pos, vec3 i, int type) :
		position(vec3(pos)), intensity(i), type(type), dir(vec3(0.0f)), coneAngle(30.0f)
	{}
	Light(vec3 pos, vec3 i, int type, float angle, vec3 d) :
		position(vec3(pos)), intensity(i), type(type), coneAngle(angle), dir(d)
	{}
	virtual void setPosition(vec3 p){ position = p; };
	virtual vec3 getPosition(){ return position; };
	virtual void setDir(vec3 d){ dir = d; };
	virtual vec3 getDir(){ return dir; };
	virtual void setIntensity(vec3 i){ intensity = i; };
	virtual vec3 getIntensity(){ return intensity; };
	virtual float getConeAngle(){ return coneAngle; };
	virtual int getType(){ return type; };
	virtual GLuint getShadowTexture(){ return shadowTexture; };
	virtual GLuint getShadowBufferObject(){ return shadowBufferObject; };
	virtual void setShadowTexture(GLuint st){ shadowTexture = st; };
	virtual void setShadowBufferObject(GLuint sbo){ shadowBufferObject = sbo; };
	virtual int getShadowDims(){ return shadowDims; };
private:
	vec3 position;
	vec3 dir;
	vec3 intensity;
	int type; // 1 is directional ligth and 0 is spotlight
	float coneAngle;
	GLuint shadowTexture, shadowBufferObject;
	int shadowDims = 4096;
};
#endif
