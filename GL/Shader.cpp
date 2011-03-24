#include "Shader.h"

Shader::Shader(const std::string & shaderFile, const unsigned shaderType)
: type(shaderType) {
	//The shader creation requires it to be in a C-style string format.
	//Hence, we need to parse the files manually.
	std::string buffer;
	if(!parseFile(shaderFile, buffer)) {
		std::cerr << "\nError parsing file: '" << shaderFile << "'";
		shader = 0;
		return;
	}
	shader = glCreateShader(type);
	const char * string = buffer.c_str();
	const int len = buffer.length();
	glShaderSource(shader, 1, &string, &len);
	glCompileShader(shader);

	//Check if the shader has been created correctly
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	std::clog << "\n" << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
			<< " shader compile status: " << (status ? "OK" : "FAIL" );
	int length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	char infoLog[length];
	glGetShaderInfoLog(shader, length, 0, infoLog);
	if(infoLog[0]) std::clog << "\n*-----\n" << shaderFile << "\n*-----\n" << infoLog << "*-----";
	if(!status) shader = 0;
}

bool Shader::parseFile(const std::string & filename, std::string & buffer, bool versionDefined) {
	//This function is responsible for parsing the shader text file.
	//It can be called recursively if the source file contains the directive '#include'.
	//Note: TODO: There exists GLSL Extension allowing automatic #including - check it out!
	std::ifstream file;
	file.open(filename.c_str());
	if(!file.is_open()) {
		std::cerr << "\nCannot open file: '" << filename << "'";
		return false;
	}
	std::string line;
	while(std::getline(file, line)) {
		if(line.substr(0, 9) == "#include ") {
			std::string nextFilename;
			try {
				nextFilename = line.substr(9, std::string::npos);
				nextFilename = nextFilename.substr(1, nextFilename.size() - 2);	//get rid of ""
			} catch(std::out_of_range & e) {
				std::cerr << "\nSyntax error at '#include' in file: '" << filename << "' (no file specified?)";
				file.close();
				return false;
			}
			if(!parseFile(nextFilename, buffer, versionDefined)) {
				std::cerr << "\nError parsing file: '" << nextFilename << "'";
				file.close();
				return false;
			}
		} else if(line.substr(0, 8) == "#version") {	//GLSL version can be defined once only
			if(!versionDefined) {
				buffer.append(line);
				buffer.append("\n");
				versionDefined = true;
			}
		} else {
			buffer.append(line);
			buffer.append("\n");
		}
	}
	file.close();
	return true;
}

bool Shader::isOK() {
	return glIsShader(shader);
}

bool Shader::isOK() const {
	return glIsShader(shader);
}

Shader::~Shader() {
	if(glIsShader(shader)) {
		glDeleteShader(shader);
	}
}
