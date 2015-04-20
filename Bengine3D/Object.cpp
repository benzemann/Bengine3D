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
	if (material.IsTextured()){
		shader.setTextureUniform(material.getTexture());
		shader.setIsTexturedUniform(1);
	}
	else {
		shader.setIsTexturedUniform(0);
	}
	shader.setTexScaleUniform(material.getTexScale());
	glDrawArrays(GL_TRIANGLES, 0, getNumberOfVertices());
}