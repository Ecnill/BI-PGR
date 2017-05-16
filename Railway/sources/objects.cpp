#include "../headers/objects.h"

void Object::showObjectCreatedMessage() {
	std::cout << getObjectName() << " created" << std::endl;
}

void DynamicObject::setTime(float startTime) {
	startTime = startTime;
	currentTime = startTime;
}
//------------------------------------------------------------
TrainObject::TrainObject() {
	showObjectCreatedMessage();

	size = config->TRAIN_SIZE;
	direction = DEFAULT_DIRECTION;
	startPosition = position = TRAIN_POSITION;
	speed = config->TRAIN_SPEED;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, -65.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

void TrainObject::update(float elapsedTime) {
	currentTime = elapsedTime;
	
}
//------------------------------------------------------------
HelicopterObject::HelicopterObject() {
	showObjectCreatedMessage();
	initCurveData();

	size = config->HELICOPTER_SIZE;
	direction = DEFAULT_DIRECTION;
	startPosition = position = HELICOPTER_POSITION;
	speed = config->HELICOPTER_SPEED;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 1));
	modelMatrix = scale(modelMatrix, vec3(size));
}

void HelicopterObject::update(float elapsedTime) {
	currentTime = elapsedTime;
	float curveParamT = speed * (currentTime - startTime);
	position = startPosition + evaluateClosedCurve(curveData, curveSize, curveParamT);
	direction = normalize(evaluateClosedCurve_1stDerivative(curveData, curveSize, curveParamT));
	modelMatrix = alignObject(position, direction, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = scale(modelMatrix, vec3(size));
}

void HelicopterObject::initCurveData() {
	curveSize = HELICOPTER_CURVE_SIZE;
	curveData = new vec3[curveSize + 10];
	for (int i = 0; i < curveSize + 10; ++i) {
		curveData[i] = vec3(
			(float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
			(float)(2.0 * (rand() / (double)RAND_MAX) - 1.0),
			0.0f
		);
	}
}

HelicopterObject::~HelicopterObject() {
	delete curveData;
}
//------------------------------------------------------------
FlatcarObject::FlatcarObject() {
	showObjectCreatedMessage();

	size = config->FLATCAR_SIZE;
	direction = DEFAULT_DIRECTION;
	position = FLATCAR_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, -65.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
FreightcarObject::FreightcarObject(vec3 position) {
	showObjectCreatedMessage();
	size = config->FREIGHTCAR_SIZE;
	direction = DEFAULT_DIRECTION;
	this->position = position;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, 30.0f, vec3(0, 1, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
FactoryObject::FactoryObject() {
	showObjectCreatedMessage();
	size = config->FACTORY_SIZE;
	direction = DEFAULT_DIRECTION;
	position = FACTORY_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 180.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, 20.0f, vec3(0, 1, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
DumpsterType1Object::DumpsterType1Object() {
	showObjectCreatedMessage();
	size = config->DUMPSTER_1_SIZE;
	direction = DEFAULT_DIRECTION;
	position = DUMPSTER_1_POSITION;
	
	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
DumpsterType2Object::DumpsterType2Object() : isFallen(false) {
	showObjectCreatedMessage();
	size = config->DUMPSTER_2_SIZE;
	direction = DEFAULT_DIRECTION;
	position = DUMPSTER_2_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}

void DumpsterType2Object::checkFall() {
	if (isFallen) {
		modelMatrix = rotate(modelMatrix, 90.0f, vec3(0, 0, 1));
		isFallen = false;
	}
}

//------------------------------------------------------------
HouseType1Object::HouseType1Object() {
	showObjectCreatedMessage();
	size = config->HOUSE_1_SIZE;
	direction = DEFAULT_DIRECTION;
	position = HOUSE_1_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, 20.0f, vec3(0, 1, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
HouseType2Object::HouseType2Object() {
	showObjectCreatedMessage();
	size = config->HOUSE_1_SIZE;
	direction = DEFAULT_DIRECTION;
	position = HOUSE_2_POSITION;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = rotate(modelMatrix, 40.0f, vec3(0, 1, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
TrackObject::TrackObject(vec3 position) {
	showObjectCreatedMessage();
	size = config->TRACK_SIZE;
	direction = DEFAULT_DIRECTION;
	this->position = position;

	modelMatrix = translate(mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 25.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
WindmillObject::WindmillObject(vec3 position) {
	showObjectCreatedMessage();
	size = config->WINDMILL_SIZE;
	direction = DEFAULT_DIRECTION;
	this->position = position;
		
	modelMatrix = translate(glm::mat4(1.0f), position);
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(0, 0, 1));
	modelMatrix = rotate(modelMatrix, 90.0f, vec3(1, 0, 0));
	modelMatrix = scale(modelMatrix, vec3(size));
}
//------------------------------------------------------------
StoneObject::StoneObject() {
	showObjectCreatedMessage();
	size = STONE_SIZE;
	direction = DEFAULT_DIRECTION;
	position = STONE_POSITION;

	modelMatrix = translate(glm::mat4(1.0f), position);
	modelMatrix = alignObject(position, direction, vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = scale(modelMatrix, vec3(size));
}
