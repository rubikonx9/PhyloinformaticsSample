#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <map>
#include <ostream>
#include "Object3d.h"
#include "GL/Program.h"

/*
 * By creating this global instance of Init struct,
 * we guarantee execution of it's ctor before main(),
 * and, appropriately, it's dtor after main().
 * Hence, functions such as glfwInit() or glfwTerminate()
 * are guaranteed to be called when necessary.
 */
struct Init {
	Init() {
		if(!glfwInit()) {
			std::cerr << "\nCould not initialize GLFW!";
			exit(1);
		}
		glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
		if(!glfwOpenWindow(600, 600, 8, 8, 8, 8, 8, 8, GLFW_WINDOW)) {
			std::cerr << "\nCould not open the window!";
			glfwTerminate();
			exit(1);
		}

		if(gl3wInit() != 0) {
			std::cerr << "\nCould not initialize GL3W!";
			glfwTerminate();
			exit(1);
		}
	}
	~Init() {
	}
} _init;

/*
 * Scans the model and pushes the data into the vector, in the order STL file requires
 * What actually happens here, is the reordering of the vertex positions according to the indices
 */
void reorderVertexData(Object3d * model, std::vector< float > & stlVertexData, float offset = 0) {
	for(unsigned i = 0; i < model->indices.size(); i+=3) {
		unsigned index1 = 3*model->indices[i];	//Three indices per one face - model is triangulated
		unsigned index2 = 3*model->indices[i+1];
		unsigned index3 = 3*model->indices[i+2];

		//For each index we get a full vertex, this is 3 position floats
		aiVector3D vp1(model->vertexPositions[index1], model->vertexPositions[index1+1], model->vertexPositions[index1+2]);
		aiVector3D vp2(model->vertexPositions[index2], model->vertexPositions[index2+1], model->vertexPositions[index2+2]);
		aiVector3D vp3(model->vertexPositions[index3], model->vertexPositions[index3+1], model->vertexPositions[index3+2]);

		stlVertexData.push_back(vp1.x + offset);
		stlVertexData.push_back(vp1.y);
		stlVertexData.push_back(vp1.z);

		stlVertexData.push_back(vp2.x + offset);
		stlVertexData.push_back(vp2.y);
		stlVertexData.push_back(vp2.z);

		stlVertexData.push_back(vp3.x + offset);
		stlVertexData.push_back(vp3.y);
		stlVertexData.push_back(vp3.z);
	}
}

void writeStlFile(std::string filename, std::vector< float > & stlVertexData) {
	std::ofstream stlFile(filename.c_str(), std::ofstream::out);
	stlFile << "solid test\n";
	for(unsigned i = 0; i < stlVertexData.size(); i+=9) {
		stlFile << "  facet normal 0 0 0\n";
		stlFile << "    outer loop\n";
		stlFile << "      vertex " << stlVertexData[i+0] << " " << stlVertexData[i+1] << " " << stlVertexData[i+2] << "\n";
		stlFile << "      vertex " << stlVertexData[i+3] << " " << stlVertexData[i+4] << " " << stlVertexData[i+5] << "\n";
		stlFile << "      vertex " << stlVertexData[i+6] << " " << stlVertexData[i+7] << " " << stlVertexData[i+8] << "\n";
		stlFile << "    endloop\n";
		stlFile << "  endfacet\n";
	}
	stlFile << "endsolid";
	stlFile.close();
}

void getRotation(float & x, float & y, float & z) {
	static float step = 0.03;
	if(glfwGetKey('W')) z+=step;
	if(glfwGetKey('S')) z-=step;
	if(glfwGetKey('A')) y+=step;
	if(glfwGetKey('D')) y-=step;
	if(glfwGetKey('Q')) x+=step;
	if(glfwGetKey('E')) x-=step;
}

int main() {
	std::map< char, Object3d * > letters = {
			/*
			 * This map shall contain the reference to each letter model.
			 * Hence, we can easily manipulate each letters' combination using only these predefined models.
			 * Actually, it could (should?) be done with lazy loading of the models.
			 */
		std::make_pair('D', new Object3d("Models/Letter_D.3ds")),
		std::make_pair('r', new Object3d("Models/Letter_r.3ds")),
		std::make_pair('o', new Object3d("Models/Letter_o.3ds")),
		std::make_pair('s', new Object3d("Models/Letter_s.3ds")),
		std::make_pair('p', new Object3d("Models/Letter_p.3ds")),
		std::make_pair('h', new Object3d("Models/Letter_h.3ds")),
		std::make_pair('i', new Object3d("Models/Letter_i.3ds")),
		std::make_pair('l', new Object3d("Models/Letter_l.3ds")),
		std::make_pair('a', new Object3d("Models/Letter_a.3ds"))
	};

	//Program for OpenGL/GLSL usage
	Program program("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	program.use();

	//Initial value set so that we can see the output on the screen
	glUniform3f(program.uniform("cameraOffset"), 0.0, 4.0, -1.0);

	//String to be printed - can only contain letters present in 'letters' map!
	std::string s = "Drosophila";

	//Data-to-export buffer, will contain vertex positions
	std::vector< float > stlVertexData;

	float assumedLetterWidth = 0.6;	//should dynamically detect letter width
	unsigned objectCounter = 0;
	foreach(char c, s) {
		Object3d * model = letters.find(c)->second;	//for each letter, get it's model
		float offset = assumedLetterWidth * objectCounter;	//this offset affects data-to-be-exported only
		reorderVertexData(model, stlVertexData, offset);
		++objectCounter;
	}

	//load the model of a cube - it will connect the letters, so that they are not disjoint
	Object3d cube("Models/cube.3ds");
	//we need to reshape it - it shall be long and thin
	for(unsigned i = 0; i < cube.vertexPositions.size(); ++i) {
		if(i % 3 == 0) {		//x
			cube.vertexPositions[i] += 1.0;	//to center it
			cube.vertexPositions[i] *= s.size() * assumedLetterWidth/2;
		} else if(i % 3 == 1) { //y
			cube.vertexPositions[i] *= 0.05; //0.05 is the thickness of the letters, actually
		} else {				//z
			cube.vertexPositions[i] *= 0.05;
		}
	}
	cube.updateBuffers();

	reorderVertexData(&cube, stlVertexData);

	writeStlFile("out.stl", stlVertexData);

	/*
	 * In the loop, just draw what's been imported
	 */
	while(glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC)) {
		glClear(GL_COLOR_BUFFER_BIT);	//clear the screen

		static float x=1.3, y, z=-0.48;	//initial rotation, set to look at the inscription
		getRotation(x, y, z);
		glUniform3f(program.uniform("rotation"), x, y, z);

		unsigned letterCounter = 0;
		foreach(char c, s) {
			glUniform3f(program.uniform("objectOffset"), (letterCounter++)*assumedLetterWidth, 0.0, 0.0);
			letters.find(c)->second->draw();
		}

		glUniform3f(program.uniform("objectOffset"), 0.0, 0.0, 0.0);
		cube.draw();

		glfwSwapBuffers();
	}

	foreach(auto pair, letters) {
		if(pair.second) {
			delete pair.second;
		}
	}

	return 0;
}
