#version 330

#line 4 4

// Translation
mat4 translate(float x, float y, float z) {
	return 
	//transpose(	// (??)
		mat4(
			vec4(1, 0, 0, x),
			vec4(0, 1, 0, y),
			vec4(0, 0, 1, z),
			vec4(0, 0, 0, 1)
		//)
	);
}
mat4 translate(vec3 translationVector) {
	return translate(translationVector.x, translationVector.y, translationVector.z);
}

// Scaling
mat4 scale(float x, float y, float z) {
	return transpose(
		mat4(
			vec4(x, 0, 0, 0),
			vec4(0, y, 0, 0),
			vec4(0, 0, z, 0),
			vec4(0, 0, 0, 1)
		)
	);
}
mat4 scale(vec3 scaleFactor) {
	return scale(scaleFactor.x, scaleFactor.y, scaleFactor.z); 
}
mat4 scale(float scaleFactor) {
	return scale(scaleFactor, scaleFactor, scaleFactor);
}

// Rotation around arbitrary axis
mat4 rotate(float angle, float x, float y, float z) {
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	
	return transpose(
		mat4(
			vec4(t*x*x + c, t*x*y + s*z, t*x*z - s*y, 0),
			vec4(t*x*y - s*z, t*y*y + c, t*y*z + s*x, 0),
			vec4(t*x*z + s*y, t*y*z - s*x, t*z*z + c, 0),
			vec4(0, 0, 0, 1)
		)
	);
}
mat4 rotate(float angle, vec3 rotationAxis) {
	return rotate(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

// Shear
mat4 shear(float Hxy, float Hxz, float Hyz, float Hyx, float Hzx, float Hzy) {
	return transpose(
		mat4(
			vec4(1, Hxy, Hxz, 0),
			vec4(Hyx, 1, Hyz, 0),
			vec4(Hzx, Hzy, 1, 0),
			vec4(0, 0, 0, 1)
		)
	);
}