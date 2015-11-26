#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <GL/gl3w.h>
#include "GL.h"

/*
 * This class represents the OpenGL's 'shader object'.
 */

class Shader {
private:
	//We never want to (can) have two instances of the same shader
	Shader();
	Shader(const Shader &);
	Shader & operator=(const Shader &);
	bool parseFile(const std::string & filename, std::string & buffer, bool versionDefined = false);
public:
	unsigned shader; //OpenGL's shader name
	unsigned type;

	Shader(const std::string & shaderFile, const unsigned shaderType);
	bool isOK();
	bool isOK() const;
	virtual ~Shader();
};

#endif /* SHADER_H_ */
