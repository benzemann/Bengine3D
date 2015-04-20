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
	virtual void setLighPosUniform(vec4 lightPos[], int size){
		glUniform4fv(lightPosUniform, size, (const GLfloat*)&lightPos[0]);
	};
	virtual void setLightIntensityUniform(vec3 intensity[], int size){
		glUniform3fv(lightIntensityUniform, size, (const GLfloat*)&intensity[0]);
	};
	virtual void setLightDirUniform(vec3 dir[], int size){
		glUniform3fv(lightDirUniform, size, (const GLfloat*)&dir[0]);
	};
	virtual void setLightConeAngleUniform(float angles[], int size){
		glUniform1fv(lightConeAngleUniform, size, (const GLfloat*)&angles[0]);
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
	virtual void setShadowTextureUniform(GLuint depthTextureID, int id){
		if (id == 0){
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthTextureID);
			glUniform1i(shadowTextureUniform, 1);
		}
		else {
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, depthTextureID);
			glUniform1i(shadowTextureUniform2, 2);
		}
		
	}
	virtual void setTextureUniform(GLuint tex){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(textureUniform, 0);
	}
	virtual void setdepthVPUniform(mat4 vp[], int size){
		glUniformMatrix4fv(depthVPUniform, size, GL_TRUE, (const GLfloat*)&vp[0]);
	}
	virtual void setDepthBiasUniform(mat4 bias[], int size){
		glUniformMatrix4fv(depthBiasUniform, size, GL_TRUE, (const GLfloat*)&bias[0]);
	}
	virtual void setTexScaleUniform(vec2 ts){
		glUniform2fv(texScaleUniform, 1, ts);
	}
	virtual void setIsTexturedUniform(int ts){
		glUniform1i(isTexturedUniform, ts);
	}
	virtual GLuint getPositionAttribute(){ return positionAttribute; };
	virtual GLuint getNormalAttribute(){ return normalAttribute; };
	virtual GLuint getTexCoordAttribute(){ return texCoordAttribute; };
	virtual bool isUsed(){ return is_used; };
private:
	char* name_of_vertex_shader, *name_of_fragment_shader;
	GLuint shaderProgram;
	GLuint projectionUniform, modelUniform, viewUniform, normalMatUniform, lightPosUniform, lightIntensityUniform, lightDirUniform, lightConeAngleUniform, ambientUniform, shadowTextureUniform, shadowTextureUniform2, depthVPUniform, depthBiasUniform, textureUniform;
	GLuint diffuseColorUniform, specularColorUniform, shininessUniform, userPosUniform, texScaleUniform, isTexturedUniform;
	GLuint positionAttribute, normalAttribute, texCoordAttribute;
	bool is_used;
};

