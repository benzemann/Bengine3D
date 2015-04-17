#pragma once
#include "Angel.h"
#include "Shader.h"

class Material
{
public:
	Material(){};
	Material(vec4 diffuse):
		diffuseColor(diffuse)
	{};
	Material(vec4 diffuse, vec4 specular, float s) :
		diffuseColor(diffuse), specularColor(specular), shininess(s)
	{};
	virtual void setDiffuseColor(vec4 c){ diffuseColor = c; };
	virtual vec4 getDiffuseColor(){ return diffuseColor; };
	virtual void setSpecularColor(vec4 c){ specularColor = c; };
	virtual vec4 getSpecularColor(){ return specularColor; };
	virtual void setShininess(float s){ shininess = s; };
	virtual float getShininess(){ return shininess; };
	virtual void createTexture(int width, int height);
	virtual void makeCheckImage();
	virtual void useTexture(Shader shader){
		shader.setTextureUniform(texture);
	};
private:
	GLubyte image[64][64][4];
	GLuint texture;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};

