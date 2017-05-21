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
	useLight = config->USE_LIGHT;
	isFire = false;
	initShaders();
	restart();
}

void Scene::initShaders() {
	lightProgram.init(useLight, config->SHADER_LIGHT.first, config->SHADER_LIGHT.second);
	skyboxProgram.init(config->SHADER_SKYBOX.first, config->SHADER_SKYBOX.second);
	explosionProgram.init(isFire, config->SHADER_EXPLOSION.first, config->SHADER_EXPLOSION.second);
}

void Scene::restart() {
	config->reloadConfigFile();
	useLight = config->USE_LIGHT;
	deleteModels();
	if (camera != nullptr) delete camera;
	sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	camera = new Camera(config->CAMERA_SPEED);
	camera->currentTime = sceneState.elapsedTime;
	if (sceneState.freeCameraMode == true) {
		sceneState.freeCameraMode = false;
		glutPassiveMotionFunc(NULL);
	}
	sceneState.setKeysStateUnpressed();
	initModels();
	train->setTime(sceneState.elapsedTime);
	helicopter->setTime(sceneState.elapsedTime);
	explosion->setTime(sceneState.elapsedTime);
}

void Scene::updateObjects(float elapsedTime) {
	camera->currentTime = elapsedTime;
	helicopter->update(elapsedTime);
	explosion->update(elapsedTime);
	train->update(elapsedTime);
	((DumpsterType2Object*)dumpsterType2)->checkFall();
}

void Scene::show() {
	sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	if (camera->actualState == camera->FREE) {
		camera->dynamicView();
	} else if (camera->actualState == camera->TOP) {
		camera->staticFromTopView();
	} else if (camera->actualState == camera->AT_TRAIN) {
		camera->staticAtTrainView();
	} else if (camera->actualState == camera->FROM_HELICOPTER) {
		camera->helicopterView(((HelicopterObject*)helicopter), sceneState.elapsedTime);
	}
	camera->setViewMatrix();
	camera->setProjectionMatrix(sceneState.windowWidth, sceneState.windowHeight);
	sceneState.setWindowTitleTime(config->WINDOW_TITLE, glutGet(GLUT_ELAPSED_TIME));

	drawSkybox();

	glClearStencil(0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, -1);
	drawMesh(factory->geometry.get(), factory->modelMatrix);
	glStencilFunc(GL_ALWAYS, 2, -1);
	drawMesh(dumpsterType2->geometry.get(), dumpsterType2->modelMatrix);
	glStencilFunc(GL_ALWAYS, 3, -1);
	drawWithReflection(train);
	glDisable(GL_STENCIL_TEST);
	drawMesh(trainFlatcar->geometry.get(), trainFlatcar->modelMatrix);
	drawMesh(dumpsterType1->geometry.get(), dumpsterType1->modelMatrix);
	drawMesh(houseType1->geometry.get(), houseType1->modelMatrix);
	drawMesh(houseType2->geometry.get(), houseType2->modelMatrix);

	drawWithReflection(helicopter);

	for (auto car : trainFreightcars) {
		drawMesh(car->geometry.get(), car->modelMatrix);
	}

	for (auto mill : windmills) {
		drawMesh(mill->geometry.get(), mill->modelMatrix);
	}
	for (auto trackPart : track) {
		drawMesh(trackPart->geometry.get(), trackPart->modelMatrix);
	}
	drawExplosion();
	drawMesh(rock->geometry.get(), rock->modelMatrix);
}

void Scene::drawWithReflection(Object *object) {
	glUseProgram(lightProgram.program);
	glUniform1f(lightProgram.timeLocation, sceneState.elapsedTime);
	glUniform3fv(lightProgram.reflectorPositionLocation, 1, glm::value_ptr(object->position));
	glUniform3fv(lightProgram.reflectorDirectionLocation, 1, glm::value_ptr(object->direction));
	glUseProgram(0);
	drawMesh(object->geometry.get(), object->modelMatrix);
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
	glUniformMatrix4fv(skyboxProgram.inversePVmatrixLocation, 1, GL_FALSE, glm::value_ptr(skybox->getInversePVmatrix(camera->view, camera->projection)));
	glUniform1i(skyboxProgram.skyboxSamplerLocation, 0);
	// draw "skybox" rendering 2 triangles covering the far plane
	glBindVertexArray(skybox->geometry->vertexArrayObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->geometry->texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, skybox->geometry->numTriangles + 2);
	resetProgram();
}

void Scene::drawExplosion() {
	if (explosion->show) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(explosionProgram.program);

		glm::mat4 rotationMatrix = glm::mat4(camera->view[0], camera->view[1], camera->view[2], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		rotationMatrix = glm::transpose(rotationMatrix);
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), explosion->position);
		matrix = glm::scale(matrix, glm::vec3(explosion->size));
		matrix = matrix*rotationMatrix;
		glm::mat4 PVMmatrix = camera->projection * camera->view * matrix;

		glUniformMatrix4fv(explosionProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));
		glUniformMatrix4fv(explosionProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr(camera->view));
		glUniformMatrix4fv(explosionProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(matrix));

		glUniform1f(explosionProgram.timeLocation, explosion->currentTime - explosion->startTime);
		glUniform1i(explosionProgram.texSamplerLocation, 0);
		glUniform1f(explosionProgram.frameDurationLocation, explosion->frameDuration);

		auto geometry = explosion->textures[explosion->actualFrame++];
		glBindVertexArray(geometry->vertexArrayObject);
		glBindTexture(GL_TEXTURE_2D, geometry->texture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->numTriangles);
		resetProgram();
		glDisable(GL_BLEND);
	}
}

void Scene::setWindowSize(const unsigned int newWidth, const unsigned int newHeight) {
	sceneState.windowHeight = newHeight;
	sceneState.windowWidth = newWidth;
}

void Scene::fallDumpster() {
	((DumpsterType2Object*)dumpsterType2)->isFallen = true;
}

void Scene::startFire() {
	explosion->show = true;
	//isFire = true;
}

void Scene::goTrain() {
	((TrainObject*)train)->run = true;
}

void Scene::initModels() {
	/*stone = new StoneObject;
	stone->geometry = ObjectMeshGeometry::loadMultiMesh(config->STONE_MODEL_PATH, lightProgram);*/

	rock = new StoneObject;
	rock->geometry = CodeMeshGeometry::loadCodeMesh(lightProgram, config->STONE_TEXTURE, stoneTriangles, stoneVertices);

	explosion = new ExplosionObject;
	for (size_t i = 1; i < explosion->countFrames; ++i) {
		std::string textureName;
		if (i < 10) {
			textureName = "data/explosion/explosion1_000";
		} else {
			textureName = "data/explosion/explosion1_00";
		}
		textureName += std::to_string(i) + ".png";
		explosion->textures.push_back(DynamicTextureGeometry::loadTextureGeometry(explosionProgram, textureName, explosion->explosionNumQuadVertices));
	}

	train = new TrainObject;
	train->geometry = ObjectMeshGeometry::loadMultiMesh(config->TRAIN_MODEL_PATH, this->lightProgram);
	
	trainFlatcar = new FlatcarObject;
	trainFlatcar->geometry = ObjectMeshGeometry::loadMultiMesh(config->FLATCAR_MODEL_PATH, this->lightProgram);

	helicopter = new HelicopterObject;
	helicopter->geometry = ObjectMeshGeometry::loadMultiMesh(config->HELICOPTER_MODEL_PATH, this->lightProgram);
	
	skybox = new SkyBoxObject;
	skybox->geometry = new SkyBoxGeometry(skyboxProgram);

	factory = new FactoryObject;
	factory->geometry = ObjectMeshGeometry::loadMultiMesh(config->FACTORY_MODEL_PATH, this->lightProgram);

	dumpsterType1 = new DumpsterType1Object;
	dumpsterType1->geometry = ObjectMeshGeometry::loadMultiMesh(config->DUMPSTER_1_MODEL_PATH, this->lightProgram);

	dumpsterType2 = new DumpsterType2Object;
	dumpsterType2->geometry = ObjectMeshGeometry::loadMultiMesh(config->DUMPSTER_2_MODEL_PATH, this->lightProgram);

	houseType1 = new HouseType1Object;
	houseType1->geometry = ObjectMeshGeometry::loadMultiMesh(config->HOUSE_1_MODEL_PATH, this->lightProgram);

	houseType2 = new HouseType2Object;
	houseType2->geometry = ObjectMeshGeometry::loadMultiMesh(config->HOUSE_2_MODEL_PATH, this->lightProgram);

	std::shared_ptr<MeshGeometry> freightcarGeometry = ObjectMeshGeometry::loadMultiMesh(config->FREIGHTCAR_MODEL_PATH, this->lightProgram);
	for (size_t i = 0; i < 3; ++i) {
		Object *freightcar = new FreightcarObject(vec3(-0.1f, 1.5f + 0.5f * i / 2, 0.0f));
		freightcar->geometry = freightcarGeometry;
		trainFreightcars.push_back(freightcar);
	}

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
}

void Scene::resetProgram() {
	glBindVertexArray(0);
	glUseProgram(0);
	CHECK_GL_ERROR();
}

Scene::~Scene() {
	deleteModels();
	delete skybox; 
	delete camera;
	pgr::deleteProgramAndShaders(lightProgram.program);
	pgr::deleteProgramAndShaders(skyboxProgram.program);
	pgr::deleteProgramAndShaders(explosionProgram.program);
}

void Scene::deleteModels() {
	delete train; 
	delete trainFlatcar; 
	delete helicopter; 
	delete factory; 
	delete dumpsterType1;
	delete dumpsterType2; 
	delete houseType1; 
	delete houseType2; 
	delete explosion;
	delete rock; 
	for (auto ob : trainFreightcars) { delete ob; }
	trainFreightcars.clear();
	for (auto ob : windmills) { delete ob; }
	windmills.clear();
	for (auto ob : track) { delete ob; }
	track.clear();
}
