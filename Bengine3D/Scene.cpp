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
	glEnableVertexAttribArray(shader.getTexCoordAttribute());
	glVertexAttribPointer(shader.getPositionAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
	glVertexAttribPointer(shader.getNormalAttribute(), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)));
	glVertexAttribPointer(shader.getTexCoordAttribute(), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vec3)* 2));

	return vertexArrayObject;
}

void Scene::createOBJ(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation, const char * path){
	vector<vec3> vertices;
	vector<vec2> uvs;
	vector<vec3> normals;
	if (loadOBJ(path, vertices, uvs, normals)){
		int size = vertices.size();
		Vertex* objData = new Vertex[size];
		for (int i = 0; i < size; i++){
			objData[i] = { vertices[i], normals[i], uvs[i] };
		}
		Object obj = Object(position, scale, mat, rotation);
		obj.setVertexArrayBuffer(loadBufferData(objData, size, shader), size);
		addObject(obj);
		delete [] objData;
	}
}
// Loads an OBJ file and gets the vertices, uvs, and normals
bool Scene::loadOBJ(const char * path, vector<vec3> & out_vertices, vector<vec2> & out_uvs, vector<vec3> & out_normals){
	// Indices arrays and temp arrays
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;
	// Open the file and return false if something whent wrong
	FILE * file = fopen(path, "r");
	if (file == NULL){
		printf("  Cannot open file !\n");
		return false;
	}
	// Reading the file
	while (1){
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		// Check the different characters, v = vertex, vt = uvs, vn = normals, f =	indices
		if (strcmp(lineHeader, "v") == 0){
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);

		}
		else if (strcmp(lineHeader, "vt") == 0){
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	// Indexing the vertices
	for (unsigned int i = 0; i < vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	// Indexing the uvs
	for (unsigned int i = 0; i < uvIndices.size(); i++){
		unsigned int uvIndex = uvIndices[i];
		vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	// Indexing the normals
	for (unsigned int i = 0; i < normalIndices.size(); i++){
		unsigned int normalIndex = normalIndices[i];
		vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	return true;
}

void Scene::createBox(vec3 position, vec3 scale, Shader shader, Material mat, vec3 rotation){
	const int boxSize = 36;
	Vertex boxData[boxSize] = {
		// front
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 1.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(1.0, 0.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(0.0, 1.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, 0, 1), vec2(1.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 0, 1), vec2(1.0, 1.0) },
		// back
		{ vec3(0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(1.0, 1.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(1.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(1.0, 1.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 0, -1), vec2(0.0, 1.0) },
		// right
		{ vec3(0.5, 0.5, 0.5), vec3(1, 0, 0), vec2(1.0, 1.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(1, 0, 0), vec2(0.0, 1.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(1, 0, 0), vec2(1.0, 1.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(1, 0, 0), vec2(1.0, 0.0) },
		// left
		{ vec3(-0.5, 0.5, -0.5), vec3(-1, 0, 0), vec2(1.0, 0.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(-1, 0, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(-1, 0, 0), vec2(0.0, 1.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(-1, 0, 0), vec2(1.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(-1, 0, 0), vec2(0.0, 1.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(-1, 0, 0), vec2(1.0, 1.0) },
		// Top
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(-0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(0.0, 1.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(1.0, 1.0) },
		{ vec3(-0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, 0.5, 0.5), vec3(0, 1, 0), vec2(1.0, 1.0) },
		{ vec3(0.5, 0.5, -0.5), vec3(0, 1, 0), vec2(1.0, 0.0) },
		// Bottom
		{ vec3(-0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(0.0, 1.0) },
		{ vec3(-0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(0.0, 0.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(1.0, 0.0) },
		{ vec3(-0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(0.0, 1.0) },
		{ vec3(0.5, -0.5, -0.5), vec3(0, -1, 0), vec2(1.0, 0.0) },
		{ vec3(0.5, -0.5, 0.5), vec3(0, -1, 0), vec2(1.0, 1.0) }
	};
	Object box = Object(position, scale, mat, rotation);
	box.setVertexArrayBuffer(loadBufferData(boxData, boxSize, shader), boxSize);
	addObject(box);
}

void Scene::createPlane(vec3 position, vec3 scale, Shader shader, Material mat){
	const int planeSize = 6;
	Vertex rectangleData[planeSize] = {
		{ vec3(-1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 0.0) },
		{ vec3(1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 1.0) },
		{ vec3(-1.0, 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 0.0) },
		{ vec3(-1., 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 0.0) },
		{ vec3(1.0, 0.0, 1.0), vec3(0, 1, 0), vec2(0.0, 1.0) },
		{ vec3(1.0, 0.0, -1.0), vec3(0, 1, 0), vec2(1.0, 1.0) },
	};
	Object plane = Object(position, vec3(scale), mat);
	plane.setVertexArrayBuffer(loadBufferData(rectangleData, planeSize, shader), planeSize);
	addObject(plane);
}

void Scene::drawObjects(Shader shader, mat4 view){
	for each (Object o in objects)
	{
		o.draw(shader, view);
	}
}

void Scene::setShadowUniforms(Shader shader){
	mat4* biasMatrix = new mat4[getNumberOfLights()];
	mat4* depthVP = new mat4[getNumberOfLights()];
	for (int i = 0; i < getNumberOfLights(); i++){
		shader.setShadowTextureUniform(getLight(i).getShadowTexture(), i);
		// Calculate depth view projection matrix and the shadow bias matrix and send it to the shader
		mat4 lightView = LookAt(getLightPos(i), getLight(i).getDir(), vec3(0.0f, 1.0f, 0.0f));
		mat4 lightProjection;
		if (getLight(i).getType() == 1){
			lightProjection = Ortho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 60.0);
		} else {
			lightProjection = Perspective(53.13f, 1, 0.1f, 60.0f);
		}
		mat4 depthVP_temp = lightProjection * lightView;
		mat4 biasMatrix_temp = Translate(vec3(0.5f));
		biasMatrix_temp *= Scale(vec3(0.5f));
		biasMatrix[i] = biasMatrix_temp;
		depthVP[i] = depthVP_temp;
	}
	shader.setdepthVPUniform(depthVP, getNumberOfLights());
	shader.setDepthBiasUniform(biasMatrix, getNumberOfLights());
	delete[] biasMatrix;
	delete[] depthVP;
}


