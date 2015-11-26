Phyloinformatics Sample
=======================

Basic info
----------

This is the code sample for Phyloinformatics, and concerns GSoC application.
This code's purpose is to create a 3D mesh with a string as an input.

Unfortunately, the application has been rejected.

Dependencies
------------

additional libraris:
* assimp, for importing 3d assets
* GL3W, for OpenGL 3.x core profile usage
* GLFW, for window handling
* boost (BOOST_FOREACH, lexical_cast)
* OpenGL 3.x and GLSL ver. 330
* C++11

Usage
-----

Just compile & run. The string to be 'printed' is hard-coded.
Also, you can rotate the camera with W/S/A/D/Q/E buttons.

Approach
--------

For each letter present in the string, we load it's predefined 3D model.
For the purpose of this project, I created just a few letters' models to
present the idea. I've created them in Blender.
Then we parse the string specified - for each character, we fetch the
corresponding model, and copy the vertex position data to a vector. Hence,
this single vector will store all the vertices' data. Similarily we can do
with, for example, a cube (which has been previously reshaped). This element
will 'hold together' all the letters, so that there are no disjoint parts.
Finally, the vertices' data is transferred from the vector to the specified
output file, which is a .STL text-type format file.
I have tested the output file using the 'MeshLab' software. As to me, it
seems to be correct.

Notes
-----

Due to lack of time - this code has been written just for the demonstration
purposes, and needed to be done quickly - many things are hard-coded, and/or
not optimized. Also, parts of the code, such as Object3d, Program and Shader
classes, have been written by me for another project and just copied here -
hence, it's possible that they contain some elements, which are not relevant
to this project. I hope that the comments in the code are helpful enough.

Platforms
---------

As of now, this project was only tested in MinGW.

For other systems, probably small changes would be necessary, especially concerning the build process.

Support
-------

This project is not developed any more.

Licensing
---------

GNU GPL v2.0.
