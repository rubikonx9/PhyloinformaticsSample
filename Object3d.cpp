#include "Object3d.h"

Object3d::Object3d(const std::string filename) {
	if(!loadModel(filename)) {
		std::cerr << "\nError loading model: " << filename;
		return;
	}
}

bool Object3d::loadModel(const std::string & filename) {
	/*
	 * Loading is done by the AssImp library. It automagically loads all the data
	 * from the model and gives free access to its hierarchical layout.
	 */

	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(filename.c_str(),
			aiProcess_CalcTangentSpace       |
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType
	);

	if(!scene) {
		std::cerr << "\nError importing model: " << filename;
		return false;
	}

	//Access the vertices (positions, normals) data
	const aiNode * root = scene->mRootNode;
	if(root->mNumChildren > 1) {
		//Only good for a single node as it is now
		std::clog << "\nWarning: model " << filename << " contains more than one node!";
	}
	const aiNode * node = root->mChildren[0];
	unsigned rootMeshIndex = node->mMeshes[0];

	unsigned numberOfVertices = scene->mMeshes[rootMeshIndex]->mNumVertices;
	aiVector3D * rawPositions = scene->mMeshes[rootMeshIndex]->mVertices;
	aiVector3D * rawNormals = scene->mMeshes[rootMeshIndex]->mNormals;

	//Push the data into the vectors
	foreach(aiVector3D position, std::make_pair(rawPositions, rawPositions + numberOfVertices)) {
		vertexPositions.push_back(position.x);
		vertexPositions.push_back(position.y);
		vertexPositions.push_back(position.z);
	}
	foreach(aiVector3D normal, std::make_pair(rawNormals, rawNormals + numberOfVertices)) {
		vertexNormals.push_back(normal.x);
		vertexNormals.push_back(normal.y);
		vertexNormals.push_back(normal.z);
	}

	//Set up the indices
	unsigned numberOfFaces = scene->mMeshes[rootMeshIndex]->mNumFaces;
	aiFace * rawFaces = scene->mMeshes[rootMeshIndex]->mFaces;
	//mNumIndices is the same for all faces, thus 0
	unsigned numberOfFaceIndices = rawFaces[0].mNumIndices;

	foreach(aiFace face, std::make_pair(rawFaces, rawFaces + numberOfFaces)) {
		foreach(unsigned index, std::make_pair(face.mIndices, face.mIndices + numberOfFaceIndices)) {
			indices.push_back(index);
		}
	}

	numberOfIndices = indices.size();

	return setUpBuffers();	//Send the data to be loaded into GPU memory
}

bool Object3d::setUpBuffers() {
	/*
	 * This function creates Video Buffer Objects (VBO) on the GPU memory,
	 * and loads the model data into it.
	 */
	glGenBuffers(1, &vboVertexPositions);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexPositions);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), &vertexPositions[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vboVertexNormals);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexNormals);
	glBufferData(GL_ARRAY_BUFFER, vertexNormals.size() * sizeof(float), &vertexNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);


	//Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void Object3d::updateBuffers() {
	/*
	 * This function uploads the vertices data to the GPU once more.
	 * Use when the model vertex positions have been updated
	 */
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexPositions);
	glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(float), &vertexPositions[0], GL_STATIC_DRAW);
}

void Object3d::draw() {
	/*
	 * This function accesses previously loaded data to draw the model.
	 */
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexPositions);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboVertexNormals);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, true,  0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	//Unbind buffers
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glDisableVertexAttribArray(glslPositionIndex);
//	glDisableVertexAttribArray(glslNormalIndex);
}

Object3d::~Object3d() {
	//Release the buffers
	if(glIsBuffer(vboVertexPositions))
		glDeleteBuffers(1, &vboVertexPositions);
	if(glIsBuffer(vboVertexNormals))
		glDeleteBuffers(1, &vboVertexNormals);
	if(glIsBuffer(vboIndices))
		glDeleteBuffers(1, &vboIndices);
}
