#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include <string>
#include <vector>
#include <iostream>
#include <GL3/gl3w.h>
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <boost/foreach.hpp>

#include "GL/GL.h"

#define foreach BOOST_FOREACH

/*
 * This class imports the 3D model from a file and loads its contents into GPU memory.
 * Also, it is responsible for drawing of this imported object.
 */
class Object3d {
private:
	Object3d();
	Object3d(const Object3d &);
	Object3d & operator=(const Object3d &);
	bool setUpBuffers();
	bool loadModel(const std::string & filename);

public:
	std::vector< float > vertexPositions;
	std::vector< float > vertexNormals;
	std::vector< unsigned > indices;

	unsigned vboVertexPositions;	//Buffer names, for OpenGL use
	unsigned vboVertexNormals;
	unsigned vboIndices;
	unsigned numberOfIndices;

	Object3d(const std::string filename);
	void draw();
	void updateBuffers();
	virtual ~Object3d();
};

#endif /* OBJECT3D_H_ */
