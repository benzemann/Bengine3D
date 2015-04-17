#include "Object.h"

void Object::draw(Shader& shader, mat4 view){
	if (!shader.isUsed()){
		shader.useShader();
	}
	glBindVertexArray(getVertexArrayBuffer());
	mat4 model = getModelMatrix();
	shader.setViewUniform(view);
	shader.setModelUniform(model);
	mat3 normalMat = Normal(model);
	shader.setNormalMatUniform(normalMat);
	shader.setDiffuseColorUniform(material.getDiffuseColor());
	shader.setSpecularColorUniform(material.getSpecularColor());
	shader.setShininessUniform(material.getShininess());
	glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());
}