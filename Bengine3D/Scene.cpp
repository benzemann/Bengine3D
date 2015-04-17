#include "Scene.h"

// Loads the buffer data and creates a vertexArrayObject
GLuint Scene::loadBufferData(Vertex* vertices, int vertexCount, Shader shader) {
	GLuint vertexArrayObject;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shader.getPositionAttribute());
	glEnableVertexAttribArray(shader.getNormalAttribute());
	glEnableVertexAttribArray(shader.getColorAttribute());
	glEnableVertexAttribArray(shader.getTexCoordAttribute());
	glVertexAttribPointer(shader.getPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
	glVertexAttribPointer(shader.getNormalAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
	glVertexAttribPointer(shader.getColorAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)* 2));
	glVertexAttribPointer(shader.getTexCoordAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)* 3));

	return vertexArrayObject;
}

void Scene::createBox(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation){
	const int boxSize = 36;
	Vertex boxData[boxSize] = {
		// front
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		// back
		{ vec3(0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		// right
		{ vec3(0.5, 0.5, 0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		// left
		{ vec3(-0.5, 0.5, -0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		// Top
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		// Bottom
		{ vec3(-0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(0.0, 0.0) }
	};
	Object box = Object(position, scale, mat, rotation);
	box.setVertexArrayBuffer(loadBufferData(boxData, boxSize, shader), boxSize);
	addObject(box);
}

void Scene::createPlane(vec3 position, vec2 scale, Shader shader, Material mat){
	const int planeSize = 6;
	Vertex rectangleData[planeSize] = {
		{ vec3(-1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 1.0) },
		{ vec3(-1.0, 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 0.0) },
		{ vec3(-1., 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 0.0) },
		{ vec3(1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 1.0) },
		{ vec3(1.0, 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 1.0) },
	};
	Object plane = Object(position, vec3(scale.x, 1.0f, scale.y), mat);
	plane.setVertexArrayBuffer(loadBufferData(rectangleData, planeSize, shader), planeSize);
	addObject(plane);
}

void Scene::drawObjects(Shader shader, mat4 view){
	for each (Object o in objects)
	{
		o.draw(shader, view);
	}
}


