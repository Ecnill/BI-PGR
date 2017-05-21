#ifndef __SCENE_H
#define __SCENE_H
#include <memory>
#include <time.h>
#include <iostream>
#include <vector>
#include "callbacks.h"
#include "shader_program.h"
#include "objects.h"
#include "camera.h"
#include "scene_state.h"
#include "mesh_geometry.h"
#include "../data/models/rock/rock.h"
using namespace glm;

class Scene {

	public:
		static Scene *getInstance();
		~Scene();
		bool init(const std::string &configFilePath);
		void start();
		void restart();
		void show();
		void updateObjects(float elapsedTime);
		void setWindowSize(const unsigned int newWidth, const unsigned int newHeight);

		// interactivity 
		void fallDumpster();
		void startFire();
		void goTrain();

		SceneState sceneState;
		Camera *camera;

		bool isFog;

	private:
		static Scene *instance;
		
		// shaders 
		void initShaders();
		LightShaderProgram lightProgram;
		SkyboxShaderProgram skyboxProgram;
		ExplosionShaderProgram explosionProgram;
		
		// models
		void initModels();
		
		DynamicObject *helicopter;
		DynamicObject *train;

		Object *rock;

		Object *stone;

		Object *trainFlatcar;
		Object *factory;
		Object *dumpsterType1;
		Object *dumpsterType2;
		Object *houseType1;
		Object *houseType2;

		ExplosionObject *explosion;
		SkyBoxObject *skybox;

		std::vector<Object*> trainFreightcars;
		std::vector<Object*> windmills;
		std::vector<Object*> track;

		// flags
		bool useLight;
		bool isFire;

		// draw methods
		void drawWithReflection(Object *object);
		void drawMesh(MeshGeometry *geometry, const mat4 &modelMatrix);
		void drawSkybox();
		void drawExplosion();

		// utils, config
		void resetProgram();
		void deleteModels();
		ConfigHolder *config;

};

#endif // __SCENE_H