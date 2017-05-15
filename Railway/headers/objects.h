#ifndef __OBJECTS_H
#define __OBJECTS_H
#include <memory>
#include "pgr.h"
#include "material.h"
#include "mesh_geometry.h"
#include "spline.h"
using namespace glm;

#define DEFAULT_DIRECTION	vec3(0.0f, 0.0f, 0.0f)

#define FACTORY_POSITION	vec3(15.0f, 10.0f, 0.0f)
#define TRAIN_POSITION		vec3(0.7f, 0.0f, 0.1f)
#define FLATCAR_POSITION	vec3(0.4f, 0.5f, 0.1f)
#define DUMPSTER_1_POSITION	vec3(0.5f, -0.5f, 0.0f)
#define DUMPSTER_2_POSITION	vec3(0.0f, 0.5f, 0.0f)
#define HOUSE_1_POSITION	vec3(-1.0f, 0.7f, 0.0f)
#define HOUSE_2_POSITION	vec3(-1.0f, -0.3f, 0.0f)

#define STONE_SIZE			22.5f
#define STONE_POSITION		vec3(-10.0f, 0.0f, 0.0f)

class Object {
	public:
		Object() : speed(0.0f) {}
		void setTime(float startTime);
		void update(float elapsedTime);

		vec3 position;
		vec3 direction;
		mat4 modelMatrix;
		std::shared_ptr<MeshGeometry> geometry;

	protected:
		virtual std::string getObjectName() = 0;
		void showObjectCreatedMessage();

		float startTime;
		float currentTime;
		float size;
		float speed;

		ConfigHolder *config = ConfigHolder::getInstance();
};

class TrainObject : public Object {
	public:
		TrainObject();
		std::string getObjectName() { return "TrainObject"; }

		bool destroyed;
};

class FlatCarObject : public Object {
	public:
		FlatCarObject();
		std::string getObjectName() { return "FlatCarObject"; }
};

class FreightcarObject : public Object {
	public:
		FreightcarObject(vec3 position);
		std::string getObjectName() { return "FreightcarObject"; }
};

class FactoryObject : public Object {
	public:
		FactoryObject();
		std::string getObjectName() { return "FactoryObject"; }
};

class TrackObject : public Object {
	public:
		TrackObject(vec3 position);
		std::string getObjectName() { return "TrackObject"; }
};

class WindmillObject : public Object {
	public:
		WindmillObject(vec3 position);
		std::string getObjectName() { return "WindmillObject"; }
};

class DumpsterType1Object : public Object {
	public:
		DumpsterType1Object();
		std::string getObjectName() { return "Dumpster1Object"; }
};

class DumpsterType2Object : public Object {
	public:
		DumpsterType2Object();
		std::string getObjectName() { return "Dumpster2Object"; }
};

class HouseType1Object : public Object {
	public:
		HouseType1Object();
		std::string getObjectName() { return "HouseObject"; }
};

class HouseType2Object : public Object {
	public:
		HouseType2Object();
		std::string getObjectName() { return "House2Object"; }
};

class StoneObject : public Object {
	public:
		StoneObject();
		std::string getObjectName() { return "StoneObject"; }
};

class SkyBoxObject : public Object {
	public:
		std::string getObjectName() { return "SkyBoxObject"; }
		SkyBoxGeometry *geometry;
};

#endif // __OBJECTS_H