#pragma once
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "Angel.h"



class Shader
{
public:
	Shader(){};
	Shader(char* nameVert, char* nameFrag):
		name_of_vertex_shader(nameVert), name_of_fragment_shader(nameFrag), is_used(false)
	{};

	virtual void loadShader();
	virtual void useShader(){
		is_used = true;
		glUseProgram(shaderProgram);
	};
	virtual GLuint getShaderProgram(){ return shaderProgram; };
	virtual void setProjectionUniform(mat4 projection){
		glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, projection);
	};
	virtual void setModelUniform(mat4 model){
		glUniformMatrix4fv(modelUniform, 1, GL_TRUE, model);
	};
	virtual void setViewUniform(mat4 view){
		glUniformMatrix4fv(viewUniform, 1, GL_TRUE, view);
	};
	virtual void setNormalMatUniform(mat3 norMat){
		glUniformMatrix3fv(normalMatUniform, 1, GL_TRUE, norMat);
	};
	virtual void setLighPosUniform(vec4 lightPos){
		glUniform4fv(lightPosUniform, 1, lightPos);
	};
	virtual void setLightIntensityUniform(float intensity){
		glUniform1f(lightIntensityUniform, intensity);
	};
	virtual void setAmbientUniform(vec4 ambient){
		glUniform4fv(ambientUniform, 1, ambient);
	};
	virtual void setDiffuseColorUniform(vec4 dColor){
		glUniform4fv(diffuseColorUniform, 1, dColor);
	};
	virtual void setSpecularColorUniform(vec4 sColor){
		glUniform4fv(specularColorUniform, 1, sColor);
	};
	virtual void setUserPosUniform(vec3 pos){
		glUniform3fv(userPosUniform, 1, pos);
	};
	virtual void setShininessUniform(float s){
		glUniform1f(shininessUniform, s);
	};
	virtual void setShadowTextureUniform(GLuint depthTextureID){
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTextureID);
		glUniform1i(shadowTextureUniform, 1);
	}
	virtual void setTextureUniform(GLuint tex){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(textureUniform, 0);
	}
	virtual void setdepthVPUniform(mat4 vp){
		glUniformMatrix4fv(depthVPUniform, 1, GL_TRUE, vp);
	}
	virtual void setDepthBiasUniform(mat4 bias){
		glUniformMatrix4fv(depthBiasUniform, 1, GL_TRUE, bias);
	}
	virtual GLuint getPositionAttribute(){ return positionAttribute; };
	virtual GLuint getColorAttribute(){ return colorAttribute; };
	virtual GLuint getNormalAttribute(){ return normalAttribute; };
	virtual GLuint getTexCoordAttribute(){ return texCoordAttribute; };
	virtual bool isUsed(){ return is_used; };
private:
	char* name_of_vertex_shader, *name_of_fragment_shader;
	GLuint shaderProgram;
	GLuint projectionUniform, modelUniform, viewUniform, normalMatUniform, lightPosUniform, lightIntensityUniform, ambientUniform, shadowTextureUniform, depthVPUniform, depthBiasUniform, textureUniform;
	GLuint diffuseColorUniform, specularColorUniform, shininessUniform, userPosUniform;
	GLuint colorAttribute, positionAttribute, normalAttribute, texCoordAttribute;
	bool is_used;
};

