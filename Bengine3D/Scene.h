#pragma once
#include "Angel\Object.h"
#include <vector>
#include "Material.h"

using namespace std;

struct Vertex {
	vec3 position;
	vec3 normal;
	vec2 texcoords;
};

class Scene
{
public:
	Scene(){};
	~Scene(){};
	virtual void removeAllObjects(){
		objects.clear();
	}
	virtual void removeObject(int id){
		objects.erase(objects.begin() + id);
	}
	virtual void addObject(Object obj){
		objects.push_back(obj);
	};
	virtual Object getObject(int id){ return objects.at(id); };
	virtual void setObject(int id, Object obj){ objects.at(id) = obj; };
	virtual void createPlane(vec3 position, vec2 scale, Shader shader, Material mat);
	virtual void createBox(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation = vec3(0.0f));
	virtual void createLightSource(vec3 pos, float intensity){
		lightPos = normalize(pos);
		lightIntensity = intensity;
	};
	virtual vec3 getLightPos(){ return lightPos; };
	virtual void setLightPos(vec3 pos){ lightPos = normalize(pos); };
	virtual float getLightInt(){ return lightIntensity; };
	virtual void setLightInt(float intensity){ lightIntensity = intensity;};
	virtual void drawObjects(Shader shader,mat4 view);
	virtual GLuint loadBufferData(Vertex* vertices, int vertexcount, Shader shader);
private:
	vector <Object> objects;
	vec3 lightPos;
	float lightIntensity;	
};

