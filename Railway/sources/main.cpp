#include <time.h>
#include "pgr.h"
#include "../headers/scene.h"

const std::string CONFIG_FILE_PATH = "./data/config.txt";

int main(int argc, char** argv) {

	// initialize windowing system
	glutInit(&argc, argv);
	// init OpenGL versions
	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	if (!Scene::getInstance()->init(CONFIG_FILE_PATH)) {
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");
		return EXIT_FAILURE;
	}

	Scene::getInstance()->start();

	glutCloseFunc(finalizeCallback);
	glutMainLoop();

	return EXIT_SUCCESS;
}
