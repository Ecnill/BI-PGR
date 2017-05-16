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
		break; // printf("Unrecognized special key pressed\n");
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
		case GLUT_KEY_RIGHT:
			scene->sceneState.keyMap[KEY_RIGHT_ARROW] = false;
			break;
		case GLUT_KEY_LEFT:
			scene->sceneState.keyMap[KEY_LEFT_ARROW] = false;
			break;
		case GLUT_KEY_UP:
			scene->camera->goForward(timeDelta);
			break;
		case GLUT_KEY_DOWN:
			scene->sceneState.keyMap[KEY_DOWN_ARROW] = false;
			break;
		default:
			break; // printf("Unrecognized special key released\n");
		}
}

void timerCallback(int) {
	scene->sceneState.elapsedTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);	// update scene time
	//float timeDelta = scene->sceneState.elapsedTime - scene->camera->currentTime;

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
		scene->camera->actualState = scene->camera->STATIC1;
		glutPassiveMotionFunc(NULL);
	}
	if (scene->sceneState.keyMap[KEY_F2] == true) {
		scene->camera->actualState = scene->camera->STATIC2;
		glutPassiveMotionFunc(NULL);
	}
	if (scene->sceneState.keyMap[KEY_F3] == true) {
		scene->camera->actualState = scene->camera->FREE;
		glutPassiveMotionFunc(passiveMouseMotionCallback);
		glutWarpPointer(scene->sceneState.windowWidth / 2, scene->sceneState.windowHeight / 2);
	}
	scene->updateObjects(scene->sceneState.elapsedTime);
	glutTimerFunc(TICK, timerCallback, 0);
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
			case 0:
				
				break;
			case 1:
				scene->goTrain();
				break;
			case 2:
			
				break;
			case 3:
				scene->fallDumpster();
				break;
			}
	}
}

int buildPopupMenu(void) {
	int menu = glutCreateMenu(selectFromMenuCallback);
	glutAddMenuEntry("Turn on/off light", MENU_LIGHTING);
	glutAddMenuEntry("Exit", MENU_EXIT);
	return menu;
}

void selectFromMenuCallback(int command) {
	switch (command) {
		case MENU_LIGHTING: {
			config->USE_LIGHT = !config->USE_LIGHT;
			scene->restart();
			break;
		}
		case MENU_EXIT:
			glutLeaveMainLoop();
			break;
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
	glutSpecialFunc(specialKeyboardCallback);     // key pressed
	glutSpecialUpFunc(specialKeyboardUpCallback); // key released

	buildPopupMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMouseFunc(onMouseButton);
	glutMouseWheelFunc(mouseWheelCallback);		// mouse scroll wheel

	glutTimerFunc(TICK, timerCallback, 0);
}