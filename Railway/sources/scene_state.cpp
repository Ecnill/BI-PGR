#include "../headers/scene_state.h"

SceneState::SceneState() : freeCameraMode(false), frames(0), timeBase(0) {
	setKeysStateUnpressed();
}

void SceneState::setKeysStateUnpressed() {
	for (int i = 0; i < KEYS_COUNT; ++i) {
		keyMap[i] = false;
	}
}

void SceneState::setWindowTitleTime(std::string mainTitle, int time) {
	frames++;
	currentTime = time;
	if (currentTime - timeBase > 1000) {
		std::ostringstream stringStream;
		stringStream << "[ FPS: " << std::setprecision(4) << frames * 1000.0 / (currentTime - timeBase) << " ] - " << time / 1000 << 's';
		std::string title = mainTitle + " " + stringStream.str();
		glutSetWindowTitle(title.data());
		timeBase = currentTime;
		frames = 0;
	}
}
