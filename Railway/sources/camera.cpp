#include "../headers/camera.h"

Camera::Camera() {
	speed = CAMERA_NORMAL_SPEED;
	viewAngleX = CAMERA_ANGLE_X;
	viewAngleY = CAMERA_ANGLE_Y;
	position = CAMERA_POSITION;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = FREE;

	std::cout << "position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	std::cout << "direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")\n";
	std::cout << std::endl;
}

void Camera::staticView1() {
	position = CAMERA_STATIC_VIEW1_POSITION;
	viewAngleX = CAMERA_STATIC_VIEW1_ANGLE_X;
	viewAngleY = CAMERA_STATIC_VIEW1_ANGLE_Y;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = STATIC1;
}

void Camera::staticView2() {
	position = CAMERA_STATIC_VIEW2_POSITION;
	viewAngleX = CAMERA_STATIC_VIEW2_ANGLE_X;
	viewAngleY = CAMERA_STATIC_VIEW2_ANGLE_Y;
	direction = vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f);
	actualState = STATIC2;
}

void Camera::dynamicView() {
	actualState = FREE;
}

void Camera::goLeft() {
	viewAngleX += CAMERA_NORMAL_SPEED;
	if (viewAngleX > 360.0f) {
		viewAngleX -= 360.0f;
	}
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::goRight() {
	viewAngleX -= CAMERA_NORMAL_SPEED;
	if (viewAngleX < 360.0f) {
		viewAngleX += 360.0f;
	}
	direction = normalize(vec3(cos(radians(viewAngleX)), sin(radians(viewAngleX)), 0.0f));
}

void Camera::lookUp() {
	viewAngleY += CAMERA_NORMAL_SPEED;
	if (viewAngleY > 360.0f) {
		viewAngleY -= 360.0f;
	}
	direction = normalize(vec3(cos(radians(viewAngleX)) * sin(radians(viewAngleY)),
		sin(radians(viewAngleX)),
		cos(radians(viewAngleX)) * cos(radians(viewAngleY))));
}

void Camera::lookDown() {
	viewAngleY -= CAMERA_NORMAL_SPEED;
	if (viewAngleY < 360.0f) {
		viewAngleY += 360.0f;
	}
	//std::cout << "position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	std::cout << "direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")\n";

	direction = normalize(vec3(
		cos(radians(viewAngleX)) * sin (radians(viewAngleY)),
		cos(radians(viewAngleY)),
		sin(radians(viewAngleY))
	));
}


void Camera::goForward(float timeDelta) {
	this->position = checkBorders(timeDelta, 1.0);
	this->direction = normalize(vec3(cos(radians(this->viewAngleX)), sin(radians(this->viewAngleX)), 0.0f));
}

void Camera::goBackwards(float timeDelta) {
	this->position = checkBorders(timeDelta, -1.0);
	this->direction = normalize(vec3(cos(radians(this->viewAngleX)), sin(radians(this->viewAngleX)), 0.0f));
}

void Camera::setProjectionMatrix(int windowWidth, int windowHeight) {
	projection = perspective(60.0f, windowWidth / (float)windowHeight, 0.01f, 100.0f);
}

vec3 Camera::checkBorders(float timeDelta, float inver) {
	vec3 newPos = this->position + inver * (this->speed * timeDelta * this->direction);
	if (newPos.x > 0.33 && newPos.x < 0.66 && newPos.y < 0.43 && newPos.y > 0.11) {
		newPos = this->position;
	}
	newPos.x = std::max(-1.0f, std::min(1.0f, newPos.x));
	newPos.y = std::max(-1.0f, std::min(1.0f, newPos.y));
	return newPos;
}

void Camera::setViewMatrix() {
	vec3 cameraUpVector = vec3(0.0f, 0.0f, 1.0f);
	vec3 cameraViewDirection = direction;
	vec3 rotationAxisY = cross(direction, vec3(0.0f, 0.0f, 1.0f));
	mat4 cameraTransform = rotate(glm::mat4(1.0f), viewAngleY, rotationAxisY);
	cameraUpVector = vec3(cameraTransform * vec4(cameraUpVector, 0.0f));
	cameraViewDirection = vec3(cameraTransform * vec4(cameraViewDirection, 0.0f));
	vec3  cameraCenter = position + cameraViewDirection;
	view = lookAt(position, cameraCenter, cameraUpVector);
}

void Camera::setTime(float startTime) {
	startTime = startTime;
	currentTime = startTime;
}
