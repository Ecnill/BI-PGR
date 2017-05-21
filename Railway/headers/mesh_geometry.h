#ifndef __MESH_GEOMETRY_H
#define __MESH_GEOMETRY_H
#include <memory>
#include <iostream>
#include <vector>
#include <utility>  
#include "config_holder.h"
#include "pgr.h"
#include "material.h"
#include "shader_program.h"
using namespace std;
//------------------------------------------------------------
struct SubMeshGeometry {
		GLuint			vertexBufferObject;   
		GLuint			elementBufferObject;
		Material		*material;
		GLuint			vertexArrayObject;
		unsigned int	numTriangles;

		~SubMeshGeometry() {
			delete material; 
		}
};
//------------------------------------------------------------
struct MeshGeometry {
		MeshGeometry();
		virtual	~MeshGeometry();

		GLuint	vertexBufferObject;   
		GLuint	elementBufferObject;  
		GLuint	vertexArrayObject;    
		unsigned int numTriangles;        
		vector<SubMeshGeometry*> subMeshVector;
};
//------------------------------------------------------------
class ObjectMeshGeometry : public MeshGeometry {
	public:
		static shared_ptr<ObjectMeshGeometry>loadMultiMesh(const string &fileName, LightShaderProgram &shader);

	private:
		static Material *getSingleMeshMaterial(const aiMaterial *mat, const string &fileName);
};
//------------------------------------------------------------
class CodeMeshGeometry : public MeshGeometry {
	public:
		static shared_ptr<CodeMeshGeometry> loadCodeMesh(LightShaderProgram &shader, std::string &textureName, const std::vector<unsigned> &triangles, const std::vector<float> &vertices);
};
//------------------------------------------------------------
class DynamicTextureGeometry : public MeshGeometry {
	public:
		static DynamicTextureGeometry *loadTextureGeometry(ExplosionShaderProgram program, const std::string textureName, std::vector<float> explosionVertexData);

		GLuint texture;
};
//------------------------------------------------------------
class SkyBoxGeometry : public MeshGeometry {
	public:
		SkyBoxGeometry(SkyboxShaderProgram &program);
		GLuint texture;
	private:
		static constexpr float screenCoords[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f };
};
//------------------------------------------------------------
#endif // __MESH_GEOMETRY_H