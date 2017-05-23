#ifndef __SHADER_PROGRAM_H
#define __SHADER_PROGRAM_H
#include "pgr.h"
#include "material.h"
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
};
//------------------------------------------------------------
class LightShaderProgram: public ShaderProgram{			
	public:
		void init(const std::string &fileVertexShader, const std::string &fileFragmentShader);
		void setMaterialUniforms(Material *material);
		void setTransformUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

		GLint normalLocation;
		GLint colorLocation;
		GLint reflectorPositionLocation;  
		GLint reflectorDirectionLocation; 
		GLboolean fogActive;
		GLboolean dayActive;
		GLboolean spotLightActive;
		GLboolean pointLightActive;
		GLint pointLightPosition;
		GLint pointLightDirection;

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