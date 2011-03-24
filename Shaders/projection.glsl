#version 330

#line 4 3

// Matrix for orthogonal projection operations (referring to screen coordinates)
mat4 orthogonal(float left, float right, float bottom, float top, float near, float far) {
	float tx = -(right+left)/(right-left);
	float ty = -(top+bottom)/(top-bottom);
	float tz = -(far+near)/(far-near);
	return transpose(
		mat4(
			vec4(2/(right-left), 0, 0, tx),
			vec4(0, 2/(top-bottom), 0, ty),
			vec4(0, 0, -2/(far-near), tz),
			vec4(0, 0, 0, 1)
		)
	);
}

// Matrix for perspective projection operations (3d environment)
mat4 perspective(float fovy, float aspectRatio, float near, float far) {
	float f = 1.0/tan(fovy/2.0);
	return transpose(
		mat4(
			vec4(f/aspectRatio, 0, 0, 0),
			vec4(0, f, 0, 0),
			vec4(0, 0, (far+near)/(near-far), 2.0*far*near/(near-far)),
			vec4(0, 0, -1, 0)
		)
	);
}