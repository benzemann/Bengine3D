#pragma once
#include "Angel.h"
#include "Shader.h"

class Material
{
public:
	Material():
		diffuseColor(vec4(0.9f, 0.9f, 0.9f, 1.0f)), specularColor(vec4(0.0f)), shininess(1.0f), texScale(vec2(1.0f)), isTextured(false)
	{};
	Material(vec4 diffuse):
		diffuseColor(diffuse), specularColor(vec4(0.0f)), shininess(1.0f), texScale(vec2(1.0f)), isTextured(false)
	{};
	Material(vec4 diffuse, vec4 specular, float s) :
		diffuseColor(diffuse), specularColor(specular), shininess(s), texScale(vec2(1.0f)), isTextured(false)
	{};
	Material(int width, int height, const char * imagePath) :
		diffuseColor(vec4(1.0f)), specularColor(vec4(0.0f)), shininess(1.0f), texScale(vec2(1.0f)), isTextured(true)
	{
		createTexture(width, height, imagePath);
	};
	Material(int width, int height, const char * imagePath, vec2 ts) :
		diffuseColor(vec4(1.0f)), specularColor(vec4(0.0f)), shininess(1.0f), texScale(ts), isTextured(true)
	{
		createTexture(width, height, imagePath);
	};
	virtual void setDiffuseColor(vec4 c){ diffuseColor = c; };
	virtual vec4 getDiffuseColor(){ return diffuseColor; };
	virtual void setSpecularColor(vec4 c){ specularColor = c; };
	virtual vec4 getSpecularColor(){ return specularColor; };
	virtual void setShininess(float s){ shininess = s; };
	virtual float getShininess(){ return shininess; };
	virtual bool createTexture(int width, int height, const char * imagePath);
	virtual GLuint getTexture(){ return texture; };
	virtual void setTexScale(vec2 ts){ texScale = ts; };
	virtual vec2 getTexScale(){ return texScale; };
	virtual bool IsTextured(){ return isTextured; };
	virtual void useTexture(Shader shader){
		shader.setTextureUniform(texture);
		isTextured = true;
	};
private:
	GLuint texture;
	bool isTextured;
	vec2 texScale;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
};

