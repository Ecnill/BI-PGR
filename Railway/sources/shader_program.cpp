#include "../headers/shader_program.h"

GLuint ShaderProgram::createShaderProgramFromFile(const std::string &fileVertexShader, const std::string &fileFragmentShader) {
	std::vector<GLuint> shaders;
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, fileVertexShader));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fileFragmentShader));
	GLuint program = pgr::createProgram(shaders);
	CHECK_GL_ERROR();
	return program;
}

void ShaderProgram::createShaderProgramFromString() {
	createShaderProgramFromString(colorVertexShaderSrc, colorFragmentShaderSrc);
}

void ShaderProgram::createShaderProgramFromString(const std::string &vertexShaderString, const std::string &fragmentShaderString) {
	std::vector<GLuint> shaderList;
	shaderList.push_back(pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexShaderString));
	shaderList.push_back(pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentShaderString));
	program = pgr::createProgram(shaderList);
	CHECK_GL_ERROR();
}

void LightShaderProgram::setTransformUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
	glm::mat4 PVM = projection * view * model;
	glUniformMatrix4fv(PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
	glUniformMatrix4fv(VmatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(MmatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	CHECK_GL_ERROR();
}

void LightShaderProgram::init(bool useLight, const std::string & fileVertexShader, const std::string & fileFragmentShader) {
	if (useLight) {
		program = createShaderProgramFromFile(fileVertexShader, fileFragmentShader);
		// get vertex attributes locations, if the shader does not have this uniform -> return -1
		posLocation = glGetAttribLocation(program, "position");
		normalLocation = glGetAttribLocation(program, "normal");
		texCoordLocation = glGetAttribLocation(program, "texCoord");
		// get uniforms locations
		PVMmatrixLocation = glGetUniformLocation(program, "PVMmatrix");
		VmatrixLocation = glGetUniformLocation(program, "Vmatrix");
		MmatrixLocation = glGetUniformLocation(program, "Mmatrix");
		normalMatrixLocation = glGetUniformLocation(program, "normalMatrix");
		timeLocation = glGetUniformLocation(program, "time");
		// material
		ambientLocation = glGetUniformLocation(program, "material.ambient");
		diffuseLocation = glGetUniformLocation(program, "material.diffuse");
		specularLocation = glGetUniformLocation(program, "material.specular");
		shininessLocation = glGetUniformLocation(program, "material.shininess");
		// texture
		texSamplerLocation = glGetUniformLocation(program, "texSampler");
		useTextureLocation = glGetUniformLocation(program, "material.useTexture");
		// reflector
		reflectorPositionLocation = glGetUniformLocation(program, "reflectorPosition");
		reflectorDirectionLocation = glGetUniformLocation(program, "reflectorDirection");
	} else {
		createShaderProgramFromString();
		// get position and color attributes locations
		posLocation = glGetAttribLocation(program, "position");
		colorLocation = glGetAttribLocation(program, "color");
		// get uniforms locations
		PVMmatrixLocation = glGetUniformLocation(program, "PVMmatrix");
	}
}

void LightShaderProgram::setMaterialUniforms(Material *material) {
	if (material != nullptr) {
		glUniform3fv(diffuseLocation, 1, glm::value_ptr(material->diffuse));  // 2nd parameter must be 1 - it declares number of vectors in the vector array
		CHECK_GL_ERROR();
		glUniform3fv(ambientLocation, 1, glm::value_ptr(material->ambient));
		CHECK_GL_ERROR();
		glUniform3fv(specularLocation, 1, glm::value_ptr(material->specular));
		CHECK_GL_ERROR();
		glUniform1f(shininessLocation, material->shininess);
		CHECK_GL_ERROR();
		if (material->textureId != 0) {
			glUniform1i(useTextureLocation, 1);  // do texture sampling
			glUniform1i(texSamplerLocation, 0);  // texturing unit 0 -> samplerID   [for the GPU linker]
			glActiveTexture(GL_TEXTURE0 + 0);                  // texturing unit 0 -> to be bound [for OpenGL BindTexture]
			glBindTexture(GL_TEXTURE_2D, material->textureId);
			CHECK_GL_ERROR();
		} else {
			glUniform1i(useTextureLocation, 0);  // do not sample the texture
		}
	}
	CHECK_GL_ERROR();
}

void SkyboxShaderProgram::init(const std::string &fileVertexShader, const std::string &fileFragmentShader) {
	program = createShaderProgramFromFile(fileVertexShader, fileFragmentShader);
	// handles to vertex attributes locations
	screenCoordLocation = glGetAttribLocation(program, "screenCoord");
	// get uniforms locations
	skyboxSamplerLocation = glGetUniformLocation(program, "skyboxSampler");
	inversePVmatrixLocation = glGetUniformLocation(program, "inversePVmatrix");
}
