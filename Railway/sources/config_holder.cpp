#include "../headers/config_holder.h"

ConfigHolder *ConfigHolder::instance = nullptr;

ConfigHolder *ConfigHolder::getInstance() {
	return instance = (!instance) ? new ConfigHolder() : instance;
}

void ConfigHolder::readConfigFile(const string &configFilePath) {
	this->filePath = configFilePath;
	map<string, string> configValues;
	string line;
	fstream fs;
	fs.open(configFilePath, std::fstream::in | std::fstream::out | std::fstream::app);

	if (!fs.good()) {
		cout << "!" << endl;
	}

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
				glm::vec3 pos = parsePosition(line);
				TRACK_PART_POSITIONS.push_back(pos);
			}
			if (getline(lineStream, value)) {
				configValues[key] = value;
			}
		}
	}
	WINDOW_WIDTH = atoi(configValues["WINDOW_WIDTH"].c_str());
	WINDOW_HEIGHT = atoi(configValues["WINDOW_HEIGHT"].c_str());
	WINDOW_TITLE = configValues["WINDOW_TITLE"];

	SHADER_SKYBOX = std::make_pair(configValues["SHADER_SKYBOX_PREFIX"] + ".vert", configValues["SHADER_SKYBOX_PREFIX"] + ".frag");
	SHADER_LIGHT = std::make_pair(configValues["SHADER_LIGHT_PREFIX"] + ".vert", configValues["SHADER_LIGHT_PREFIX"] + ".frag");

	SKYBOX_CUBE_TEXTURE_FILE_PREFIX = configValues["SKYBOX_CUBE_TEXTURE_FILE_PREFIX"];

	TRACK_MODEL_PATH = configValues["TRACK_MODEL_PATH"];
	TRACK_SIZE = stof(configValues["TRACK_SIZE"]);

	TRAIN_MODEL_PATH = configValues["TRAIN_MODEL_PATH"];
	TRAIN_SIZE = stof(configValues["TRAIN_SIZE"]);

	FREIGHTN_MODEL_PATH = configValues["FREIGHTN_MODEL_PATH"];

	FACTORY_MODEL_PATH = configValues["FACTORY_MODEL_PATH"];
	FACTORY_SIZE = stof(configValues["FACTORY_SIZE"]);

	WINDMILL_MODEL_PATH = configValues["WINDMILL_MODEL_PATH"];
	WINDMILL_SIZE = stof(configValues["WINDMILL_SIZE"]);

	USR_LIGHT = toBool(configValues["USR_LIGHT"]);

	fs.close();
}

void ConfigHolder::reloadConfigFile() {
	TRACK_PART_POSITIONS.clear();
	readConfigFile(filePath);
}

bool ConfigHolder::toBool(string &str) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	istringstream is(str);
	bool b;
	is >> boolalpha >> b;
	return b;
}

glm::vec3 ConfigHolder::parsePosition(string &str) {
	istringstream is(str);
	char ch;
	float x, y, z;
	is >> ch >> x >> ch >> y >> ch >> z >> ch; 
	return glm::vec3(x, y, z);
}
