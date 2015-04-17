#include "Shader.h"
#include <iostream>

using namespace std;
using namespace Angel;

void Shader::loadShader(){
	shaderProgram = Angel::InitShader(name_of_vertex_shader, name_of_fragment_shader, "fragColor");
	projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	if (projectionUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'projection' uniform." << endl;
	}
	modelUniform = glGetUniformLocation(shaderProgram, "model");
	if (modelUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'model' uniform." << endl;
	}
	viewUniform = glGetUniformLocation(shaderProgram, "view");
	if (viewUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'view' uniform." << endl;
	}
	normalMatUniform = glGetUniformLocation(shaderProgram, "normalMat");
	if (normalMatUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'normalMat' uniform." << endl;
	}
	lightPosUniform = glGetUniformLocation(shaderProgram, "lightPos");
	if (lightPosUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'lightPos' uniform." << endl;
	}
	lightIntensityUniform = glGetUniformLocation(shaderProgram, "lightIntensity");
	if (lightIntensityUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'lightIntensity' uniform." << endl;
	}
	shadowTextureUniform = glGetUniformLocation(shaderProgram, "shadowTexture");
	if (shadowTextureUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'shadowTexture' uniform." << endl;
	}
	textureUniform = glGetUniformLocation(shaderProgram, "tex");
	if (textureUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'tex' uniform." << endl;
	}
	ambientUniform = glGetUniformLocation(shaderProgram, "ambient");
	if (ambientUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'ambient' uniform." << endl;
	}
	depthVPUniform = glGetUniformLocation(shaderProgram, "depthVP");
	if (depthVPUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'depthVP' uniform." << endl;
	}
	depthBiasUniform = glGetUniformLocation(shaderProgram, "bias");
	if (depthBiasUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'bias' uniform." << endl;
	}
	diffuseColorUniform = glGetUniformLocation(shaderProgram, "diffuseColor");
	if (diffuseColorUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'diffuseColor' uniform." << endl;
	}
	specularColorUniform = glGetUniformLocation(shaderProgram, "specularColor");
	if (specularColorUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'specularColor' uniform." << endl;
	}
	userPosUniform = glGetUniformLocation(shaderProgram, "userPos");
	if (userPosUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'userPos' uniform." << endl;
	}
	shininessUniform = glGetUniformLocation(shaderProgram, "shininess");
	if (shininessUniform == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'shininess' uniform." << endl;
	}
	colorAttribute = glGetAttribLocation(shaderProgram, "color");
	if (colorAttribute == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'color' attribute." << endl;
	}
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	if (positionAttribute == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'position' attribute." << endl;
	}
	normalAttribute = glGetAttribLocation(shaderProgram, "normal");
	if (normalAttribute == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'normal' attribute." << endl;
	}
	texCoordAttribute = glGetAttribLocation(shaderProgram, "texCoord");
	if (texCoordAttribute == GL_INVALID_INDEX) {
		cerr << name_of_vertex_shader << " did not contain the 'texCoord' attribute." << endl;
	}
}
