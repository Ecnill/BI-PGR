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

struct SubMeshGeometry {
		GLuint			vertexBufferObject;   
		GLuint			elementBufferObject;
		Material		*material;
		GLuint			vertexArrayObject;
		unsigned int	numTriangles;

		~SubMeshGeometry() {
			if (material != nullptr) { delete material; }
		}
};

struct MeshGeometry {
						MeshGeometry();
		virtual			~MeshGeometry();
		GLuint			vertexBufferObject;   // identifier for the vertex buffer object
		GLuint			elementBufferObject;  // identifier for the element buffer object
		GLuint			vertexArrayObject;    // identifier for the vertex array object
		unsigned int	numTriangles;         // number of triangles in the mesh

		vector<SubMeshGeometry*> subMeshVector;
};

class ObjectMeshGeometry : public MeshGeometry {
	public:
		static shared_ptr<ObjectMeshGeometry>loadMultiMesh(const string &fileName, LightShaderProgram &shader);
	//	static ObjectMeshGeometry *loadSingleMesh(const std::string &fileName, ShaderProgram & shader);

	private:
		static Material *getSingleMeshMaterial(const aiMaterial *mat, const string &fileName);
};

class CodeMeshGeometry : public MeshGeometry {
	public:
		static CodeMeshGeometry *loadCodeMesh(LightShaderProgram &shader, const std::vector<unsigned> &triangles, const std::vector<float> &vertices);
};

class SkyBoxGeometry : public MeshGeometry {
	public:
		SkyBoxGeometry(SkyboxShaderProgram &program);
		GLuint texture;
	private:
		static constexpr float screenCoords[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f };
};

#endif // __MESH_GEOMETRY_H