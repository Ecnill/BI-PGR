#include "../headers/scene.h"
Scene *Scene::instance = nullptr;

Scene *Scene::getInstance() {
	return instance = (!instance) ? new Scene : instance;
}

bool Scene::init(const std::string &configFilePath) {
	config = ConfigHolder::getInstance();
	config->readConfigFile(configFilePath);
	// init and create window
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(config->WINDOW_WIDTH, config->WINDOW_HEIGHT);
	glutCreateWindow(config->WINDOW_TITLE.data());

	manageCallbacks();
	// initialize GL, devil etc.
	return pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
}

void Scene::start() {
	// initialize random seed
	srand((unsigned int)time(NULL));

	// initialize OpenGL
	glClearColor(0.1f, 0.1f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0f);

	useLight = config->USR_LIGHT;
	initShaders();
	restart();
}

void Scene::initShaders() {
	lightProgram.init(useLight, config->SHADER_LIGHT.first, config->SHADER_LIGHT.second);
	skyboxProgram.init(config->SHADER_SKYBOX.first, config->SHADER_SKYBOX.second);
}

void Scene::restart() {
	config->reloadConfigFile();
	useLight = config->USR_LIGHT;

	deleteModels();
	if (camera != nullptr) delete camera;
	sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	camera = new Camera;
	camera->setTime(sceneState.elapsedTime);
	if (sceneState.freeCameraMode == true) {
		sceneState.freeCameraMode = false;
		glutPassiveMotionFunc(NULL);
	}
	sceneState.setKeysStateUnpressed();
	initModels();
	train->setTime(sceneState.elapsedTime);
}

void Scene::show() {
	sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	if (camera->actualState == camera->FREE) {
		camera->dynamicView();
	} else if (camera->actualState == camera->STATIC1) {
		camera->staticView1();
	} else if (camera->actualState == camera->STATIC2) {
		camera->staticView2();
	}
	camera->setViewMatrix();
	camera->setProjectionMatrix(sceneState.windowWidth, sceneState.windowHeight);
	sceneState.setWindowTitleTime(config->WINDOW_TITLE, glutGet(GLUT_ELAPSED_TIME));
	
	drawSkybox();
	drawMesh(train->geometry.get(), train->modelMatrix);
	drawMesh(trainCar->geometry.get(), trainCar->modelMatrix);
	drawMesh(trainCar->geometry.get(), trainCar->modelMatrix);
	drawMesh(factory->geometry.get(), factory->modelMatrix);

	for (Object *mill : windmills) {
		drawMesh(mill->geometry.get(), mill->modelMatrix);
	}
	for (Object *trackPart : track) {
		drawMesh(trackPart->geometry.get(), trackPart->modelMatrix);
	}
//	drawMesh(stone->geometry, stone->modelMatrix);
}

void Scene::drawMesh(MeshGeometry *geometry, const glm::mat4 &modelMatrix) {
	glUseProgram(lightProgram.program);
	// setting matrices to the vertex & fragment shader
	lightProgram.setTransformUniforms(modelMatrix, camera->view, camera->projection);

	for (auto it : geometry->subMeshVector) {
		lightProgram.setMaterialUniforms(it->material);
		glBindVertexArray(it->vertexArrayObject);
		glDrawElements(GL_TRIANGLES, it->numTriangles * 3, GL_UNSIGNED_INT, 0);
	}
	resetProgram();
}

void Scene::drawSkybox() {
	glUseProgram(skyboxProgram.program);
	// compose transformations

	glm::mat4 matrix = camera->projection * camera->view;
	// crate view rotation matrix by using view matrix with cleared translation
	glm::mat4 viewRotation = camera->view;
	viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	// vertex shader will translate screen space coordinates (NDC) using inverse PV matrix
	glm::mat4 inversePVmatrix = glm::inverse(camera->projection * viewRotation);
	glUniformMatrix4fv(skyboxProgram.inversePVmatrixLocation, 1, GL_FALSE, glm::value_ptr(inversePVmatrix));

	glUniform1i(skyboxProgram.skyboxSamplerLocation, 0);
	// draw "skybox" rendering 2 triangles covering the far plane
	glBindVertexArray(skybox->geometry->vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->geometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, skybox->geometry->numTriangles + 2);
	resetProgram();
}

void Scene::updateObjects(float elapsedTime) {
	camera->currentTime = elapsedTime;
	train->update(elapsedTime);
	trainCar->update(elapsedTime);
}

void Scene::setWindowSize(const unsigned int newWidth, const unsigned int newHeight) {
	sceneState.windowHeight = newHeight;
	sceneState.windowWidth = newWidth;
}

void Scene::initModels() {
	train = new TrainObject;
	train->geometry = ObjectMeshGeometry::loadMultiMesh(config->TRAIN_MODEL_PATH, this->lightProgram);

	trainCar = new TrainCarObject;
	trainCar->geometry = ObjectMeshGeometry::loadMultiMesh(config->FREIGHTN_MODEL_PATH, this->lightProgram);

	skybox = new SkyBoxObject;
	skybox->geometry = new SkyBoxGeometry(skyboxProgram);

	factory = new FactoryObject;
	factory->geometry = ObjectMeshGeometry::loadMultiMesh(config->FACTORY_MODEL_PATH, this->lightProgram);

	std::shared_ptr<MeshGeometry> millGeometry = ObjectMeshGeometry::loadMultiMesh(config->WINDMILL_MODEL_PATH, this->lightProgram);
	
	for (int y = -10; y <= 10; y += 4) {
		Object *mill = new WindmillObject(vec3(10.0f, y, 0.0f));
		mill->geometry = millGeometry;
		windmills.push_back(mill);
		if (y == 10) {
			for (int x = 3; x <= 6; x += 3) {
				mill = new WindmillObject(vec3(x, y, 0.0f));
				mill->geometry = millGeometry;
				windmills.push_back(mill);
			}
		}
	}

	std::shared_ptr<MeshGeometry> trackGeomtery = ObjectMeshGeometry::loadMultiMesh(config->TRACK_MODEL_PATH, this->lightProgram);
	for (vec3 pos : config->TRACK_PART_POSITIONS) {
		Object *trackPart = new TrackObject(pos);
		trackPart->geometry = trackGeomtery;
		track.push_back(trackPart);
	} 

//	stone = new StoneObject();
//	stone->geometry = CodeMeshGeometry::loadCodeMesh(this->lightProgram, stoneTriangles, stoneVertices);
}

void Scene::resetProgram() {
	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();
}

Scene::~Scene() {
	deleteModels();
	delete camera;
	pgr::deleteProgramAndShaders(lightProgram.program);
	pgr::deleteProgramAndShaders(skyboxProgram.program);
}

void Scene::deleteModels() {
	if (train != nullptr) {
		delete train;
		train = nullptr;
	}
	if (trainCar != nullptr) {
		delete trainCar;
		trainCar = nullptr;
	}
	if (factory != nullptr) {
		delete factory;
		factory = nullptr;
	}
	if (skybox != nullptr) {
		delete skybox;
		skybox = nullptr;
	}
	for (Object *ob : windmills) {
		delete ob;
		ob = nullptr;
	}
	windmills.clear();
	for (Object *ob : track) {
		delete ob;
		ob = nullptr;
	}
	track.clear();
	if (stone != nullptr) {
		delete stone;
		stone = nullptr;
	}
}
