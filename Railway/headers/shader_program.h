#ifndef __SHADER_PROGRAM_H
#define __SHADER_PROGRAM_H
#include "pgr.h"
#include "material.h"
//------------------------------------------------------------
const std::string colorVertexShaderSrc(
	"#version 140\n"
	"uniform mat4 PVMmatrix;\n"
	"in vec3 position;\n"
	"in vec3 color;\n"
	"smooth out vec4 theColor;\n"
	"void main() {\n"
	"  gl_Position = PVMmatrix * vec4(position, 1.0);\n"
	"  theColor = vec4(color, 1.0);\n"
	"}\n");

const std::string colorFragmentShaderSrc(
	"#version 140\n"
	"smooth in vec4 theColor;\n"
	"out vec4 outputColor;\n"
	"void main() {\n"
	"  outputColor = theColor;\n"
	"}\n");
//------------------------------------------------------------
class ShaderProgram {
	public:
			GLuint program;
			GLint posLocation;
			GLint texCoordLocation;
			GLint PVMmatrixLocation;
			GLint VmatrixLocation;
			GLint MmatrixLocation;
			GLint timeLocation;
			GLint texSamplerLocation;

	protected:
			GLuint createShaderProgramFromFile(const std::string &fileVertexShader, const std::string &fileFragmentShader);
			void createShaderProgramFromString();
			void createShaderProgramFromString(const std::string &vertexShaderString, const std::string &fragmentShaderString);
};
//------------------------------------------------------------
class LightShaderProgram: public ShaderProgram{			
	public:
		void init(bool useLight, const std::string &fileVertexShader, const std::string &fileFragmentShader);
		void setMaterialUniforms(Material *material);
		void setTransformUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

		GLint normalLocation;
		GLint colorLocation;
		GLint reflectorPositionLocation;  
		GLint reflectorDirectionLocation; 
		GLint fogActiveLocation;

	private:
		GLint normalMatrixLocation; 				
		GLint diffuseLocation;    
		GLint ambientLocation;    
		GLint specularLocation;   
		GLint shininessLocation; 			  
		GLint useTextureLocation; 			  
		
};
//------------------------------------------------------------
class SkyboxShaderProgram : public ShaderProgram {
	public:
		void init(const std::string &fileVertexShader, const std::string &fileFragmentShader);

		GLint inversePVmatrixLocation;
		GLint skyboxSamplerLocation;
		GLint screenCoordLocation;
		GLint fogActiveLocation;
};
//------------------------------------------------------------
class ExplosionShaderProgram : public ShaderProgram {
	public:
		void init(bool inFire, const std::string &fileVertexShader, const std::string &fileFragmentShader);

		GLint frameDurationLocation;

	private:
		GLint inFireLocation;
		GLint fogLocation;
		GLint fogColorLocation;
		GLint fogStartLocation;
		GLint fogEndLocation;
};
 
#endif // __SHADER_PROGRAM_H