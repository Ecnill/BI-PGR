#include "../headers/objects.h"

void Object::setTime(float startTime) {
	this->startTime = startTime;
	this->currentTime = startTime;
}

void Object::update(float elapsedTime) {
	float timeDelta = elapsedTime - currentTime;
	currentTime = elapsedTime;
	position += timeDelta * speed * direction;
}

void Object::showObjectCreatedMessage() {
	std::cout << getObjectName() << " created" << std::endl;
}

TrainObject::TrainObject() : destroyed(false) {
	showObjectCreatedMessage();
	this->size = config->TRAIN_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = TRAIN_POSITION;
	this->speed = TRAIN_SPEED;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, -65.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

TrainCarObject::TrainCarObject() {
	showObjectCreatedMessage();
	this->size = config->TRAIN_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = TRAIN_CAR_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, -65.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

FactoryObject::FactoryObject() {
	showObjectCreatedMessage();
	this->size = config->FACTORY_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = FACTORY_POSITION;

	modelMatrix = translate(glm::mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 180.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, 20.0f, vec3(0, 1, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

TrackObject::TrackObject(vec3 position) {
	showObjectCreatedMessage();
	this->size = config->TRACK_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = position;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 25.0f, glm::vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

WindmillObject::WindmillObject(vec3 position) {
	showObjectCreatedMessage();
	this->size = config->WINDMILL_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = position;
		
	modelMatrix = translate(glm::mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, glm::vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, glm::vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, glm::vec3(size, size, size));
}

StoneObject::StoneObject() {
	showObjectCreatedMessage();
	this->size = STONE_SIZE;
	this->direction = DEFAULT_DIRECTION;
	this->position = STONE_POSITION;

	modelMatrix = translate(glm::mat4(1.0f), position);

	modelMatrix = alignObject(position, direction, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = scale(modelMatrix, vec3(size));
}
