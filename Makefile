include_dirs = -I../gl3w/include/ -I../boost/ -I../glfw/include/ -I../assimp/include/
libs_dirs    = -L../gl3w/lib/ -L../glfw/lib-mingw/ -L../assimp/bin/
libs         = -lglfw -lgl3w -lopengl32 -lassimp
flags        = -std=c++0x -Wall

DEFAULT: sample

sample: object3d.o main.o GL/program.o GL/shader.o
	g++ object3d.o main.o GL/program.o GL/shader.o ${libs_dirs} ${libs} ${flags} -o sample

main.o: main.cpp
	g++ -c main.cpp ${include_dirs} ${flags} -o main.o

object3d.o: Object3d.cpp Object3d.h
	g++ -c Object3d.cpp ${include_dirs} ${flags} -o object3d.o

GL/shader.o: GL/Shader.cpp GL/Shader.h
	g++ -c GL/Shader.cpp ${include_dirs} ${flags} -o GL/shader.o

GL/program.o: GL/Program.cpp GL/Program.h
	g++ -c GL/Program.cpp ${include_dirs} ${flags} -o GL/program.o

clean:
	rm -rf sample.exe object3d.o main.o GL/program.o GL/shader.o

.PHONY: clean
