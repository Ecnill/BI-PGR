//----------------------------------------------------------------------------------------
/**
* \file       spline.cpp
* \author     Miroslav Miksik & Jaroslav Sloup & Petr Felkel
* \date       2013
* \brief      Utility functions and stuff tasks concerning animation curves.
*/
//----------------------------------------------------------------------------------------
#include <list>
#include "../headers/spline.h"
using namespace std;

//**************************************************************************************************
/// Checks whether vector is zero-length or not.
bool isVectorNull(glm::vec3 &vect) {
	return !vect.x && !vect.y && !vect.z;
}
//**************************************************************************************************
/// Align (rotate and move) current coordinate system to given parameters.
glm::mat4 alignObject(glm::vec3& position, const glm::vec3& front, const glm::vec3& up) {
	glm::vec3 z = -glm::normalize(front);
	if (isVectorNull(z)) {
		z = glm::vec3(0.0, 0.0, 1.0);
	}
	glm::vec3 x = glm::normalize(glm::cross(up, z));

	if (isVectorNull(x)) {
		x = glm::vec3(1.0, 0.0, 0.0);
	}
	glm::vec3 y = glm::cross(z, x);
	//mat4 matrix = mat4(1.0f);
	glm::mat4 matrix = glm::mat4(
		x.x, x.y, x.z, 0.0,
		y.x, y.y, y.z, 0.0,
		z.x, z.y, z.z, 0.0,
		position.x, position.y, position.z, 1.0
	);
	return matrix;
}
//**************************************************************************************************
/// Evaluates a position on Catmull-Rom curve segment.
glm::vec3 evaluateCurveSegment(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, const float t) {

	glm::vec4 tVector(0.5f*(t*t*t), 0.5f*(t*t), 0.5f*(t), 0.5f);
	glm::mat4 constMatrix(
		-1.0f, 2.0f, -1.0f, 0.0f,
		3.0f, -5.0f, 0.0f, 2.0f,
		-3.0f, 4.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f
	);
	glm::mat3x4 pTMatrix(P0.x, P1.x, P2.x, P3.x, P0.y, P1.y, P2.y, P3.y, P0.z, P1.z, P2.z, P3.z /* 0.0f, 0.0f, 0.0f, 0.0f*/);

	return tVector*constMatrix*pTMatrix;
}
//**************************************************************************************************
/// Evaluates a first derivative of Catmull-Rom curve segment.
glm::vec3 evaluateCurveSegment_1stDerivative(glm::vec3& P0, glm::vec3& P1, glm::vec3& P2, glm::vec3& P3, const float t) {

	glm::vec4 tVector((0.5f)*(3.0f)*(t*t), (0.5f)*(2.0f)*(t), (0.5f)*(1.0f), 0.0f);
	glm::mat4 constMatrix(
		-1, 2, -1, 0,
		3, -5, 0, 2,
		-3, 4, 1, 0,
		1, -1, 0, 0
	);
	glm::mat3x4 pTMatrix(P0.x, P1.x, P2.x, P3.x, P0.y, P1.y, P2.y, P3.y, P0.z, P1.z, P2.z, P3.z /* 0.0f, 0.0f, 0.0f, 0.0f*/);

	return tVector*constMatrix*pTMatrix;
}
//**************************************************************************************************
/// Evaluates a position on a closed curve composed of Catmull-Rom segments.
glm::vec3 evaluateClosedCurve(glm::vec3 * points, const size_t count, const float t) {

	glm::vec3 result(0.0, 0.0, 0.0);

	float param = cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = evaluateCurveSegment(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);
	return result;
}
//**************************************************************************************************
/// Evaluates a first derivative of a closed curve composed of Catmull-Rom segments.
glm::vec3 evaluateClosedCurve_1stDerivative(glm::vec3 points[], const size_t count, const float t) {

	glm::vec3 result(1.0, 0.0, 0.0);

	float param = cyclic_clamp(t, 0.0f, float(count));
	size_t index = size_t(param);

	result = evaluateCurveSegment_1stDerivative(
		points[(index - 1 + count) % count],
		points[(index) % count],
		points[(index + 1) % count],
		points[(index + 2) % count],
		param - floor(param)
	);
	return result;
}
//**************************************************************************************************
glm::vec3 interpolateShow(const glm::vec3 start, const glm::vec3 end, float alpha) {
	return glm::vec3(
		(float)(((start.x * (0.2 - 1)) + end.x) * alpha),
		(float)(((start.y * (0.2 - 1)) + end.y) * alpha),
		(float)(((start.z * (0.2 - 1)) + end.z) * alpha)
	);
}