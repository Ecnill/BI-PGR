#ifndef __CONFIG_HOLDER_H
#define __CONFIG_HOLDER_H
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "pgr.h"
using namespace std;

class ConfigHolder {
	public:
		static ConfigHolder *getInstance();
		void readConfigFile(const string &configFilePath);
		void reloadConfigFile();

		// window
		int WINDOW_WIDTH;
		int WINDOW_HEIGHT;
		string WINDOW_TITLE;

		// program variables
		bool USR_LIGHT;

		// shaders
		pair<string, string> SHADER_SKYBOX;
		pair<string, string> SHADER_LIGHT;
		
		// objects
		string	TRAIN_MODEL_PATH;
		float	TRAIN_SIZE;

		string FREIGHTN_MODEL_PATH;

		string TRACK_MODEL_PATH;
		float TRACK_SIZE;
		vector<glm::vec3> TRACK_PART_POSITIONS;

		string FACTORY_MODEL_PATH;
		float FACTORY_SIZE;

		string WINDMILL_MODEL_PATH;
		float WINDMILL_SIZE;

		string SKYBOX_CUBE_TEXTURE_FILE_PREFIX;

	private:
		static ConfigHolder *instance;
		string filePath;

		static bool toBool(string &str);
		static glm::vec3 parsePosition(string &str);
};

#endif // __CONFIG_HOLDER_H
