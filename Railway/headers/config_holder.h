#ifndef __CONFIG_HOLDER_H
#define __CONFIG_HOLDER_H
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
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
		float CAMERA_SPEED;

		// shaders
		pair<string, string> SHADER_SKYBOX;
		pair<string, string> SHADER_LIGHT;
		pair<string, string> SHADER_EXPLOSION;
		
		// objects
		float TRAIN_SIZE;
		float TRAIN_SPEED;
		string TRAIN_MODEL_PATH;

		float FLATCAR_SIZE;
		string FLATCAR_MODEL_PATH;

		float FREIGHTCAR_SIZE;
		string FREIGHTCAR_MODEL_PATH;

		float TRACK_SIZE;
		string TRACK_MODEL_PATH;
		vector<glm::vec3> TRACK_PART_POSITIONS;

		float HELICOPTER_SIZE;
		float HELICOPTER_SPEED;
		string HELICOPTER_MODEL_PATH;
			
		float FACTORY_SIZE;
		string FACTORY_MODEL_PATH;

		float DUMPSTER_1_SIZE;
		string DUMPSTER_1_MODEL_PATH;

		float DUMPSTER_2_SIZE;
		string DUMPSTER_2_MODEL_PATH;

		float HOUSE_1_SIZE;
		string HOUSE_1_MODEL_PATH;

		float HOUSE_2_SIZE;
		string HOUSE_2_MODEL_PATH;
		
		float WINDMILL_SIZE;
		string WINDMILL_MODEL_PATH;

		float STONE_SIZE;
		string STONE_MODEL_PATH;
		string STONE_TEXTURE;
		
		vector<string> SKYBOX_CUBE_TEXTURE_FILES;

	private:
		string filePath;

		vector<string> createSkyboxTextureFilenames(string &prefix);
		pair<string, string> createShaderFilenames(string &prefix);

		static ConfigHolder *instance;

		static bool stringToBool(string &str);
		static glm::vec3 stringToVec3(string &str);
};

#endif // __CONFIG_HOLDER_H
