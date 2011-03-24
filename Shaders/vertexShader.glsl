#version 330

#include "Shaders/projection.glsl"
#include "Shaders/transformations.glsl"

#line 7 0

layout(location = 0)
in vec3 vertexPosition;
layout(location = 1)
in vec3 vertexNormal;

uniform vec3 rotation;
uniform vec3 objectOffset;
uniform vec3 cameraOffset;

void main() {
	float perspectiveZNear = 0.001;
	float perspectiveZFar = 1000.0;
	float fovy = 45.0;
//	mat4 projection = orthogonal(0.0, screenSizeX, 0.0, screenSizeY, perspectiveZNear, perspectiveZFar);
	mat4 projection = perspective(fovy, 600/600, perspectiveZNear, perspectiveZFar);

	vec4 modelPosition = vec4(vertexPosition, 1.0)
				* translate(objectOffset)
				* translate(cameraOffset)
				;
				
	mat4 cameraView = mat4(1)
				* rotate(rotation.x, 1.0, 0.0, 0.0)
				* rotate(rotation.y, 0.0, 1.0, 0.0)
				* rotate(rotation.z, 0.0, 0.0, 1.0)
				;
	
	vec4 eyespacePosition = cameraView * modelPosition;
	
	gl_Position = projection * eyespacePosition;
}