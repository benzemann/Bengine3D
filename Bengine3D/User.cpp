#include "User.h"

mat4 User::getViewMatrix(){
	return LookAt(vec4(position, 1.0f), vec4(dir, 1.0f), vec4(up,0.0f));
}

void User::moveForward(float speed){
	vec3 viewVector = dir - position;
	viewVector.y = 0.0f;
	viewVector = normalize(viewVector);
	position.x = position.x + (viewVector.x * speed);
	position.z = position.z + (viewVector.z * speed);
	dir.x = dir.x + (viewVector.x * speed);
	dir.z = dir.z + (viewVector.z * speed);
}

void User::flyForward(float speed){
	vec3 viewVector = dir - position;
	viewVector = normalize(viewVector);
	position = position + (viewVector * speed);
	dir = dir + (viewVector * speed);
}

void User::strafe(float speed){
	vec3 viewVector = dir - position;
	viewVector.y = 0.0f;
	viewVector = normalize(viewVector);
	vec3 rightVector;
	rightVector.x = -viewVector.z;
	rightVector.z = viewVector.x;
	position.x = position.x + (rightVector.x * speed);
	position.z = position.z + (rightVector.z * speed);
	dir.x = dir.x + (rightVector.x * speed);
	dir.z = dir.z + (rightVector.z * speed);
}

void User::rotateWithMouse(int x, int y, int windowWidth, int windowHeight){
	if (x > windowWidth - 10){
		glutWarpPointer(10, y);
	}
	else if (x < 10){
		glutWarpPointer(windowWidth-10, y);
	}
	else if (y > windowHeight - 10){
		glutWarpPointer(x, windowHeight - 10);
	}
	else if (y < 10){
		glutWarpPointer(x, 10);
	}

	angle.x = -(2*M_PI * (x/(float)windowWidth));
	angle.y = ((1.5f*M_PI) * (y/(float)windowHeight));
	if (angle.y > 4.6f){
		angle.y = 4.6f;
	}
	else if (angle.y < 1.5f){
		angle.y = 1.5f;
	}
	vec3 newDir = vec3(cosf(angle.y) * sinf(angle.x), sinf(angle.y), cosf(angle.y) * cos(angle.x));
	dir = newDir + position;
}