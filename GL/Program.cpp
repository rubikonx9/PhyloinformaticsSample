#include "Program.h"

Program::Program(std::string vertexShaderFile, std::string fragmentShaderFile) {
	//Shading program requires both Vertex and Fragment Shaders
	Shader vertexShader(vertexShaderFile, GL_VERTEX_SHADER);
	Shader fragmentShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

	if(!checkShader(vertexShader) || !checkShader(fragmentShader)) {
		std::cerr << "\nError creating program: { '" << vertexShaderFile << "', '" << fragmentShaderFile << "' }";
		program = 0;
		return;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexShader.shader);
	glAttachShader(program, fragmentShader.shader);
	glLinkProgram(program);

	//Check if the linking of the program succeeded
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	std::clog << "\nProgram link status: " << (status ? "OK" : "FAIL" );
	int length;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
	char infoLog[length];
	glGetProgramInfoLog(program, length, 0, infoLog);
	if(infoLog[0]) std::clog << "\n*-----\n" << infoLog << "*-----";
	if(!status) program = 0;
}

bool Program::checkShader(const Shader & shader) {
	bool status = shader.isOK();
	if(!status) {
		std::cerr << "\nError creating " << (shader.type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader";
	}
	return status;
}

bool Program::isOK() {
	glValidateProgram(program);
	int status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if(!status) {
		std::clog << "\nProgram validation failed: ";
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char infoLog[length];
		glGetProgramInfoLog(program, length, 0, infoLog);
		if(infoLog[0]) std::clog << "\n*-----\n" << infoLog << "*-----";
		return false;
	}
	return glIsProgram(program);
}

void Program::use() {
	glUseProgram(program);
}

void Program::dontUse() {
	glUseProgram(0);
}

int Program::attribute(std::string attributeName) {
	return glGetAttribLocation(program, attributeName.c_str());
}

int Program::uniform(std::string uniformName) {
	return glGetUniformLocation(program, uniformName.c_str());
}

Program::~Program() {
	if(glIsProgram(program)) {
		glDeleteProgram(program);
	}
}
