#ifndef GL_H_
#define GL_H_

#include <string>
#include <iostream>
#include <gl3/gl3w.h>
#include <gl/glfw.h>
#include <boost/lexical_cast.hpp>

namespace GL {
#define toString(x) boost::lexical_cast<std::string>(x)
inline std::string getVersion() {
	//Note: glGetString() MUST be called after creating the GL context - otherwise, it will segfault.
	int major, minor, revision;
	glfwGetGLVersion(&major, &minor, &revision);
	return ("\nOpenGL version: " + toString(major) + "." +  toString(minor) + " rev. " + toString(revision)
			+ "\nGLSL version: " + toString(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

inline bool check(const char* file, int line) {
	//Check for OpenGL errors and print if any of them occured
	std::string msg = "\nOpenGL error detected!";
	GLenum error = glGetError();
	switch(error) {
	case GL_NO_ERROR:
		return true;
		break;
	case GL_INVALID_ENUM:
		msg += "\nInvalid Enum ";
		break;
	case GL_INVALID_VALUE:
		msg += "\nInvalid Value ";
		break;
	case GL_INVALID_OPERATION:
		msg += "\nInvalid Operation ";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		msg += "\nInvalid Framebuffer Operation ";
		break;
	case GL_INVALID_INDEX:
		msg += "\nInvalid Index ";
		break;
	default:
		msg += "\nOther Error (?) ";
		break;
	}
	msg += "in file: " + toString(file) + ", at line: " + toString(line);
	std::cout << msg << std::endl;
	return false;
}
#undef toString
};

#define checkgl GL::check(__FILE__, __LINE__);

#endif /* GL_H_ */
