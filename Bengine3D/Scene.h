#pragma once
#include "Angel\Object.h"
#include <vector>
#include "Material.h"
#include "Light.h"

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
	virtual bool loadOBJ(const char * path, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals);
	virtual void createOBJ(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation, const char * path);
	virtual void createPlane(vec3 position, vec3 scale, Shader shader, Material mat);
	virtual void createBox(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation = vec3(0.0f));
	virtual void createLightSource(vec3 pos, vec3 intensity, int type){
		Light l(pos, intensity, type, 25.0f, vec3(0.0f, 0.0f, 0.0f));
		lights.push_back(l);
	};
	virtual void setLightUniforms(Shader shader){
		int size = lights.size();
		vec4* lPos = new vec4[size];
		vec3* lInt = new vec3[size];
		vec3* lDir = new vec3[size];
		float* angles = new float[size];
		for (int i = 0; i < size; i++){
			if (getLight(i).getType() == 0){
				lPos[i] = vec4(lights.at(i).getPosition(), 0.0f);
			}
			else {
				lPos[i] = vec4(lights.at(i).getPosition(), 1.0f);
			}
			lInt[i] = lights.at(i).getIntensity();
			lDir[i] = lights.at(i).getDir();
			angles[i] = lights.at(i).getConeAngle();
		}
		shader.setLightDirUniform(lDir, size);
		shader.setLighPosUniform(lPos, size);
		shader.setLightIntensityUniform(lInt, size);
		shader.setLightConeAngleUniform(angles, size);
		delete[] angles;
		delete[] lPos;
		delete[] lInt;
		delete[] lDir;

	};
	virtual void setShadowUniforms(Shader shader);
	virtual int getNumberOfObjects(){ return objects.size(); };
	virtual vec3 getLightPos(int id){ return lights[id].getPosition(); };
	virtual void setLightPos(vec3 pos, int id){ lights[id].setPosition(pos); };
	virtual vec3 getLightDir(int id){ return lights[id].getDir(); };
	virtual void setLightDir(vec3 dir, int id){ lights[id].setDir(dir); };
	virtual vec3 getLightInt(int id){ return lights[id].getIntensity(); };
	virtual void setLightInt(vec3 intensity, int id){ lights[id].setIntensity(intensity);};
	virtual void drawObjects(Shader shader,mat4 view);
	virtual Light getLight(int id){ return lights.at(id); };
	virtual void setLight(int id, Light l){ lights.at(id) = l; };
	virtual int getNumberOfLights(){ return lights.size(); };
	virtual GLuint loadBufferData(Vertex* vertices, int vertexcount, Shader shader);
private:
	vector <Object> objects;
	vector <Light> lights;
};

