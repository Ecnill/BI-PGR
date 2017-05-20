#include "../headers/config_holder.h"

ConfigHolder *ConfigHolder::instance = nullptr;

ConfigHolder *ConfigHolder::getInstance() {
	return instance = (!instance) ? new ConfigHolder() : instance;
}

void ConfigHolder::reloadConfigFile() {
	TRACK_PART_POSITIONS.clear();
	readConfigFile(filePath);
}

void ConfigHolder::readConfigFile(const string &configFilePath) {
	this->filePath = configFilePath;
	map<string, string> configValues;
	string line;
	fstream fs;
	fs.open(configFilePath, std::fstream::in | std::fstream::out | std::fstream::app);

	while (getline(fs, line)) {
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		istringstream lineStream(line);
		string key;
		if (getline(lineStream, key, '=')) {
			string value;
			if (key[0] == '#') {
				continue;
			}
			if (line.find_first_of("(") != string::npos) {
				TRACK_PART_POSITIONS.push_back(stringToVec3(line));
			}
			if (getline(lineStream, value)) {
				configValues[key] = value;
			}
		}
	}

	WINDOW_WIDTH = atoi(configValues["WINDOW_WIDTH"].c_str());
	WINDOW_HEIGHT = atoi(configValues["WINDOW_HEIGHT"].c_str());
	WINDOW_TITLE = configValues["WINDOW_TITLE"];

	SHADER_SKYBOX = createShaderFilenames(configValues["SHADER_SKYBOX_PREFIX"]);  
	SHADER_LIGHT = createShaderFilenames(configValues["SHADER_LIGHT_PREFIX"]);

	SKYBOX_CUBE_TEXTURE_FILES = createSkyboxTextureFilenames(configValues["SKYBOX_CUBE_TEXTURE_FILE_PREFIX"]);

	HELICOPTER_MODEL_PATH = configValues["HELICOPTER_MODEL_PATH"];
	HELICOPTER_SIZE = stof(configValues["HELICOPTER_SIZE"]);
	HELICOPTER_SPEED = stof(configValues["HELICOPTER_SPEED"]);

	TRAIN_MODEL_PATH = configValues["TRAIN_MODEL_PATH"];
	TRAIN_SIZE = stof(configValues["TRAIN_SIZE"]);
	TRAIN_SPEED = stof(configValues["TRAIN_SPEED"]);

	FREIGHTCAR_MODEL_PATH = configValues["FREIGHTCAR_MODEL_PATH"];
	FREIGHTCAR_SIZE = stof(configValues["FREIGHTCAR_SIZE"]);

	FLATCAR_MODEL_PATH = configValues["FLATCAR_MODEL_PATH"];
	FLATCAR_SIZE = stof(configValues["FLATCAR_SIZE"]);

	TRACK_MODEL_PATH = configValues["TRACK_MODEL_PATH"];
	TRACK_SIZE = stof(configValues["TRACK_SIZE"]);

	FACTORY_MODEL_PATH = configValues["FACTORY_MODEL_PATH"];
	FACTORY_SIZE = stof(configValues["FACTORY_SIZE"]);

	DUMPSTER_1_MODEL_PATH = configValues["DUMPSTER_MODE1_1_PATH"];
	DUMPSTER_1_SIZE = stof(configValues["DUMPSTER_1_SIZE"]);

	DUMPSTER_2_MODEL_PATH = configValues["DUMPSTER_2_MODEL_PATH"];
	DUMPSTER_2_SIZE = stof(configValues["DUMPSTER_2_SIZE"]);

	HOUSE_1_MODEL_PATH = configValues["HOUSE_1_MODEL_PATH"];
	HOUSE_1_SIZE = stof(configValues["HOUSE_1_SIZE"]);

	HOUSE_2_MODEL_PATH = configValues["HOUSE_2_MODEL_PATH"];
	HOUSE_2_SIZE = stof(configValues["HOUSE_2_SIZE"]);

	WINDMILL_MODEL_PATH = configValues["WINDMILL_MODEL_PATH"];
	WINDMILL_SIZE = stof(configValues["WINDMILL_SIZE"]);

	USE_LIGHT = stringToBool(configValues["USE_LIGHT"]);
	CAMERA_SPEED = stof(configValues["CAMERA_SPEED"]);

	fs.close();
}

vector<string> ConfigHolder::createSkyboxTextureFilenames(string &prefix) {
	const char *suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	vector<string> skyboxTexturesFiles;
	for (int i = 0; i < 6; ++i) {
		skyboxTexturesFiles.push_back(prefix + "_" + suffixes[i] + ".jpg");
	}
	return skyboxTexturesFiles;
}

pair<string, string> ConfigHolder::createShaderFilenames(string &prefix) {
	return pair<string, string>(prefix + ".vert", prefix + ".frag");
}

bool ConfigHolder::stringToBool(string &str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	istringstream is(str);
	bool b;
	is >> boolalpha >> b;
	return b;
}

glm::vec3 ConfigHolder::stringToVec3(string &str) {
	istringstream is(str);
	char ch;
	float x, y, z;
	is >> ch >> x >> ch >> y >> ch >> z >> ch; 
	return glm::vec3(x, y, z);
}
