#include "../headers/camera.h"

Camera::Camera(float speed) {
	this->speed = speed;
	viewAngleX = CAMERA_ANGLE_X;
	viewAngleY = CAMERA_ANGLE_Y;
	position = CAMERA_POSITION;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = FREE;
}

void Camera::staticFromTopView() {
	position = STATIC_FROM_TOP_VIEW_POSITION;
	viewAngleX = STATIC_FROM_TOP_VIEW_ANGLE_X;
	viewAngleY = STATIC_FROM_TOP_VIEW_ANGLE_Y;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = TOP;
}

void Camera::staticAtTrainView() {
	position = STATIC_TO_TRAIN_VIEW_POSITION;
	viewAngleX = STATIC_TO_TRAIN_VIEW_ANGLE_X;
	viewAngleY = STATIC_TO_TRAIN_VIEW_ANGLE_Y;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = AT_TRAIN;
}

void Camera::dynamicView() {
	actualState = FREE;
}

void Camera::helicopterView(HelicopterObject *helicopter, float elapsedTime) {
	position = interpolateShow(position, helicopter->position, 0.2f);
	float curveParamT = helicopter->speed * (helicopter->currentTime - helicopter->startTime);
	direction = normalize(
		evaluateClosedCurve_1stDerivative(helicopter->curveData.data(), helicopter->curveData.size(), curveParamT));
	actualState = FROM_HELICOPTER;
}

void Camera::goLeft() {
	viewAngleX += speed;
	if (viewAngleX > 360.0f) {
		viewAngleX -= 360.0f;
	}
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::goRight() {
	viewAngleX -= speed;
	if (viewAngleX < 360.0f) {
		viewAngleX += 360.0f;
	}
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::lookUp() {
	viewAngleY += speed;
	if (viewAngleY > 360.0f) {
		viewAngleY -= 360.0f;
	}
	direction.z = viewAngleY / 360;
}

void Camera::lookDown() {
	viewAngleY -= speed;
	if (viewAngleY < -360.0f) {
		viewAngleY += 360.0f;
	}
	direction.z = viewAngleY / 360;
}

void Camera::goForward(float timeDelta) {
	position = checkBorders(timeDelta, 1.0);
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::goBackwards(float timeDelta) {
	position = checkBorders(timeDelta, -1.0);
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::setProjectionMatrix(int windowWidth, int windowHeight) {
	projection = perspective(60.0f, windowWidth / (float)windowHeight, 0.01f, 100.0f);
}

void Camera::setViewMatrix() {
	vec3 cameraUpVector = vec3(0.0f, 0.0f, 1.0f);
	vec3 cameraViewDirection = direction;
	vec3 rotationAxisY = cross(direction, vec3(0.0f, 0.0f, 1.0f));
	mat4 cameraTransform = rotate(mat4(1.0f), viewAngleY, rotationAxisY);
	cameraUpVector = vec3(cameraTransform * vec4(cameraUpVector, 0.0f));
	cameraViewDirection = vec3(cameraTransform * vec4(cameraViewDirection, 0.0f));
	vec3  cameraCenter = position + cameraViewDirection;
	view = lookAt(position, cameraCenter, cameraUpVector);
}

vec3 Camera::checkBorders(float timeDelta, float inver) {
	vec3 newPos = position + inver * (speed * timeDelta * direction);
	if (newPos.x > 0.33 && newPos.x < 0.66 && newPos.y < 0.43 && newPos.y > 0.11) {
		newPos = position;
	}
	newPos.x = std::max(-1.0f, std::min(1.0f, newPos.x));
	newPos.y = std::max(-1.0f, std::min(1.0f, newPos.y));
	return newPos;
}
