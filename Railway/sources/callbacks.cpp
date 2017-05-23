#include "../headers/callbacks.h"

Scene *scene = Scene::getInstance();
ConfigHolder *config = ConfigHolder::getInstance();

void displayCallback() {
	GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	glClear(mask);
	scene->show();
	glutSwapBuffers();
}

void reshapeCallback(int newWidth, int newHeight) {
	scene->setWindowSize(newWidth, newHeight);
	glViewport(0, 0, (GLsizei)newWidth, (GLsizei)newHeight);
}

void keyboardCallback(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
		case 27:
			glutLeaveMainLoop();
			break;
		case 'r': {
			scene->restart();
			glutReshapeWindow(config->WINDOW_WIDTH, config->WINDOW_HEIGHT);
			break;
		}
		case 'f': {
			scene->isFog = !scene->isFog;
			break;
		}
		case 's': {
			if (!scene->isDay) {
				scene->isSpotLight = !scene->isSpotLight;
			}
			break;
		}
		case 'd': {
			scene->isDay = !scene->isDay; 
			if (scene->isDay) {
				scene->isPointLight = false;
			}
			break;
		}
		case 'p': {
			if (!scene->isDay) {
				scene->isPointLight = !scene->isPointLight;
			}
		}
		default:
			break;
	}
}

void keyboardUpCallback(unsigned char keyPressed, int mouseX, int mouseY) {
	switch (keyPressed) {
		case ' ':
			scene->sceneState.keyMap[KEY_SPACE] = false;
			break;
		default:
			break;
		}
}

void specialKeyboardCallback(int specKeyPressed, int mouseX, int mouseY) {
	switch (specKeyPressed) {
		case GLUT_KEY_F1:
			scene->sceneState.keyMap[KEY_F1] = true;
			break;
		case GLUT_KEY_F2:
			scene->sceneState.keyMap[KEY_F2] = true;
			break;
		case GLUT_KEY_F3:
			scene->sceneState.keyMap[KEY_F3] = true;
			break;
		case GLUT_KEY_F4:
			scene->sceneState.keyMap[KEY_F4] = true;
			break;
		case GLUT_KEY_RIGHT:
			scene->sceneState.keyMap[KEY_RIGHT_ARROW] = true;
			break;
		case GLUT_KEY_LEFT:
			scene->sceneState.keyMap[KEY_LEFT_ARROW] = true;
			break;
		case GLUT_KEY_UP:
			scene->sceneState.keyMap[KEY_UP_ARROW] = true;
			break;
		case GLUT_KEY_DOWN:
			scene->sceneState.keyMap[KEY_DOWN_ARROW] = true;
			break;
		default:
		//	std::cout << "Unrecognized special key pressed." << std::endl;
			break; 
		}
}

void specialKeyboardUpCallback(int specKeyPressed, int mouseX, int mouseY) {
	scene->sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);	// update scene time
	float timeDelta = scene->sceneState.elapsedTime - scene->camera->currentTime;
	switch (specKeyPressed) {
		case GLUT_KEY_F1:
			scene->sceneState.keyMap[KEY_F1] = false;
			break;
		case GLUT_KEY_F2:
			scene->sceneState.keyMap[KEY_F2] = false;
			break;
		case GLUT_KEY_F3:
			scene->sceneState.keyMap[KEY_F3] = false;
			break;
		case GLUT_KEY_F4:
			scene->sceneState.keyMap[KEY_F4] = false; 
		case GLUT_KEY_RIGHT:
			scene->sceneState.keyMap[KEY_RIGHT_ARROW] = false;
			break;
		case GLUT_KEY_LEFT:
			scene->sceneState.keyMap[KEY_LEFT_ARROW] = false;
			break;
		case GLUT_KEY_UP:
			scene->sceneState.keyMap[KEY_UP_ARROW] = false;
			break;
		case GLUT_KEY_DOWN:
			scene->sceneState.keyMap[KEY_DOWN_ARROW] = false;
			break;
		default:
		//	std::cout << "Unrecognized special key released." << std::endl;
			break;
		}
}

void timerCallback(int) {
	scene->sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	float timeDelta = scene->sceneState.elapsedTime - scene->camera->currentTime;
	if (scene->sceneState.keyMap[KEY_RIGHT_ARROW] == true) {
		scene->camera->goRight();
	}
	if (scene->sceneState.keyMap[KEY_LEFT_ARROW] == true) {
		scene->camera->goLeft();
	}
	if (scene->sceneState.keyMap[KEY_UP_ARROW] == true) {
		scene->camera->lookUp();
	}
	if (scene->sceneState.keyMap[KEY_DOWN_ARROW] == true) {
		scene->camera->lookDown();
	}
	if (scene->sceneState.keyMap[KEY_F1] == true) {
		scene->camera->actualState = scene->camera->TOP;
		glutPassiveMotionFunc(NULL);
	}
	if (scene->sceneState.keyMap[KEY_F2] == true) {
		scene->camera->actualState = scene->camera->AT_TRAIN;
		glutPassiveMotionFunc(NULL);
	}
	if (scene->sceneState.keyMap[KEY_F3] == true) {
		scene->camera->actualState = scene->camera->FREE;
		glutPassiveMotionFunc(passiveMouseMotionCallback);
		glutWarpPointer(scene->sceneState.windowWidth / 2, scene->sceneState.windowHeight / 2);
	}
	if (scene->sceneState.keyMap[KEY_F4] == true) {
		scene->camera->actualState = scene->camera->FROM_HELICOPTER;
		glutPassiveMotionFunc(NULL);
	}
	scene->updateObjects(scene->sceneState.elapsedTime);
	glutTimerFunc(REFRESH_INTERVAL, timerCallback, 0);
	glutPostRedisplay();
}

void passiveMouseMotionCallback(int mouseX, int mouseY) {
	if (mouseY != scene->sceneState.windowHeight / 2) {
		float cameraElevationAngleDelta = 0.5f * (mouseY - scene->sceneState.windowHeight / 2);
		if (fabs(scene->camera->viewAngleY + cameraElevationAngleDelta) < CAMERA_ELEVATION_MAX) {
			scene->camera->viewAngleY += cameraElevationAngleDelta;
		}
		glutWarpPointer(scene->sceneState.windowWidth / 2, scene->sceneState.windowHeight / 2);
		glutPostRedisplay();
	}
	if (mouseX != scene->sceneState.windowWidth / 2) {
		float elevationDelta = 0.5f * (mouseX - scene->sceneState.windowWidth / 2);
		if ((mouseX - scene->sceneState.windowWidth / 2) < 0) {
			scene->camera->goLeft();
		} else {
			scene->camera->goRight();
		}
		glutWarpPointer(scene->sceneState.windowWidth / 2, scene->sceneState.windowHeight / 2);
		glutPostRedisplay();
	}
}

void mouseWheelCallback(int button, int dir, int x, int y) {
	scene->sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);	// update scene time
	float timeDelta = scene->sceneState.elapsedTime - scene->camera->currentTime;
	if (dir > 0) {
		scene->camera->goForward(timeDelta);
	} else {
		scene->camera->goBackwards(timeDelta);
	}
}

void onMouseButton(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (button == GLUT_DOWN)) {
		unsigned char id = 0;
		glReadPixels(x, config->WINDOW_HEIGHT - 1 - y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &id);
		switch (id) {
			case 1: {
				scene->startFire();
				break;
			}
			case 2: {
				scene->fallDumpster();
				break;
			}
			case 3: {
				scene->goTrain();
				break;
			}
			case 4: {
				scene->camera->lookAtObject(scene->dumpsterType1->position);
				break;
			}
			default:
				std::cout << "background" << std::endl;
				break;
			}
	}
}

int buildPopupMenu(void) {
	int menu = glutCreateMenu(selectFromMenuCallback);
	glutAddMenuEntry("Turn on/off light", MENU_LIGHTING);
	glutAddMenuEntry("Make explosion!", MENU_EXPLOSION);
	glutAddMenuEntry("Camera top view", MENU_CAMERA_TOP);
	glutAddMenuEntry("Camera at train view", MENU_CAMERA_TRAIN);
	glutAddMenuEntry("Camera from helicopter", MENU_CAMERA_HELICOPTER);
	glutAddMenuEntry("Exit", MENU_EXIT);
	return menu;
}

void selectFromMenuCallback(int command) {
	switch (command) {
		case MENU_LIGHTING: {
			scene->isDay = !scene->isDay;
			break;
		}
		case MENU_EXPLOSION: {
			scene->startFire();
			break;
		}
		case MENU_CAMERA_TOP: {
			scene->camera->actualState = scene->camera->TOP;
			break;
		}
		case MENU_CAMERA_TRAIN: {
			scene->camera->actualState = scene->camera->AT_TRAIN;
			break;
		}
		case MENU_CAMERA_FREE: {
			scene->camera->actualState = scene->camera->FROM_HELICOPTER;
			break;
		}
		case MENU_CAMERA_HELICOPTER: {
			scene->camera->actualState = scene->camera->FROM_HELICOPTER;
			break;
		}
		case MENU_EXIT: {
			glutLeaveMainLoop();
			break;
		}
	}
}

void finalizeCallback() {
	scene->~Scene();
}

void manageCallbacks() {
	glutDisplayFunc(displayCallback);
	// register callback for change of window size
	glutReshapeFunc(reshapeCallback);
	// register callbacks for keyboard
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialKeyboardCallback);    
	glutSpecialUpFunc(specialKeyboardUpCallback); 

	buildPopupMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMouseFunc(onMouseButton);
	glutMouseWheelFunc(mouseWheelCallback);		

	glutTimerFunc(REFRESH_INTERVAL, timerCallback, 0);
}