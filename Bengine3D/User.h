#pragma once
#include "Angel.h"

class User
{
public:
	User() :
		position(vec3(0.0f)), dir(vec3(0.0, 0.0, -1.0)), up(vec3(0.0f, 1.0f, 0.0f)), angle(3.14f,0.0f)
	{};
	User(vec3 pos) :
		position(pos), dir(position + vec3(0.0, 0.0, -1.0)), up(vec3(0.0f, 1.0f, 0.0f)), angle(3.14f, 0.0f)
	{};
	User(vec3 pos, vec3 dir) :
		position(pos), dir(dir), up(vec3(0.0f, 1.0f, 0.0f)), angle(3.14f, 0.0f)
	{};
	User(vec3 pos, vec3 dir, vec3 up) :
		position(pos), dir(dir), up(up), angle(3.14f, 0.0f)
	{};

	virtual void setDir(vec3 d){ dir = d; };
	virtual vec3 getDir(){ return dir; };
	virtual mat4 getViewMatrix();
	virtual void moveForward(float speed);
	virtual void flyForward(float speed);
	virtual void strafe(float speed);
	virtual void rotateWithMouse(int x, int y, int windowWidth, int windowHeight);
	virtual vec3 getPosition(){ return position; };
	virtual void setPostion(vec3 v){ position = v; };
	virtual void setUp(vec3 v){ up = v; };
	virtual vec3 getUp(){ return up; };
private:
	vec3 position;
	vec3 dir;
	vec3 up;
	vec2 mousePos;
	vec2 angle;
};

