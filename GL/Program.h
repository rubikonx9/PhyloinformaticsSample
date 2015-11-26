#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <string>
#include <GL/gl3w.h>

#include "Shader.h"

/*
 * This class represents OpenGL 'shader program' object.
 * It takes care of it's creation, and allows to easy access ex. the uniforms.
 */

class Program {
private:
	//We never want to (can) have two instances of the same program
	Program();
	Program(const Program &);
	Program & operator=(const Program &);
	bool checkShader(const Shader & shader);
public:
	unsigned program; //OpenGL program object name

	Program(std::string vertexShaderFile, std::string fragmentShaderFile);
	bool isOK();
	void use();
	void dontUse();
	int uniform(std::string uniformName);
	int attribute(std::string attributeName);
	virtual ~Program();
};

#endif /* PROGRAM_H_ */
