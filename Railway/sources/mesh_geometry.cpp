#include "../headers/mesh_geometry.h"

const unsigned FACE_VERT_COUNT = 3;  // triangles

MeshGeometry::MeshGeometry() {
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);
}

MeshGeometry::~MeshGeometry() {
	glDeleteVertexArrays(1, &(vertexArrayObject));
	glDeleteBuffers(1, &(elementBufferObject));
	glDeleteBuffers(1, &(vertexBufferObject));
	for (auto sub : subMeshVector) {
		delete sub;
	}
}
//------------------------------------------------------------
shared_ptr<ObjectMeshGeometry> ObjectMeshGeometry::loadMultiMesh(const string &fileName, LightShaderProgram &shader) {
	Assimp::Importer importer;
	shared_ptr<ObjectMeshGeometry> geometry = nullptr;
	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); // Unitize object in size (scale the model to fit into (-1..1)^3)
																// Load asset from the file - you can play with various processing steps
	const aiScene *scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // Triangulate polygons (if any).
		| aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
		| aiProcess_GenSmoothNormals        // Calculate normals per vertex.
		| aiProcess_JoinIdenticalVertices);
	// abort if the loader fails
	if (!scn) {
		cerr << "assimp error: " << importer.GetErrorString() << std::endl;
		return geometry;
	}
	if (scn->mNumMeshes < 1) {
		std::cerr << "No mesh in file " << fileName << std::endl;
		return geometry;
	}
	std::cout << "Loaded " << scn->mNumMeshes << " meshes" << std::endl;
	geometry = shared_ptr<ObjectMeshGeometry> (new ObjectMeshGeometry());
	for (size_t i = 0; i < scn->mNumMeshes; ++i) {
		// in this phase we know we have one mesh in our loaded scene, we can directly copy its data to opengl ...
		const aiMesh * mesh = scn->mMeshes[i];
		SubMeshGeometry *subMesh = new SubMeshGeometry;

		// vertex buffer object, store all vertex positions and normals
		glGenBuffers(1, &((subMesh)->vertexBufferObject));
		glBindBuffer(GL_ARRAY_BUFFER, (subMesh)->vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float)*mesh->mNumVertices, 0, GL_STATIC_DRAW); // allocate memory for vertices, normals, and texture coordinates
																								// first store all vertices
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float)*mesh->mNumVertices, mesh->mVertices);
		// then store all normals
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float)*mesh->mNumVertices, 3 * sizeof(float)*mesh->mNumVertices, mesh->mNormals);

		float *textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
		float *currentTextureCoord = textureCoords;
		// copy texture coordinates
		aiVector3D vect;
		if (mesh->HasTextureCoords(0)) {
			for (unsigned int idx = 0; idx<mesh->mNumVertices; idx++) {
				vect = (mesh->mTextureCoords[0])[idx];
				*currentTextureCoord++ = vect.x;
				*currentTextureCoord++ = vect.y;
			}
		} 
		// finally store all texture coordinates
		glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float)*mesh->mNumVertices, 2 * sizeof(float)*mesh->mNumVertices, textureCoords);

		// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
		for (size_t f = 0; f < mesh->mNumFaces; ++f) {
			indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
			indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
			indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
		}
		// copy our temporary index array to OpenGL and free the array
		glGenBuffers(1, &(subMesh->elementBufferObject));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh->elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

		delete[] indices;

		// copy the material info to MeshGeometry structure
		const aiMaterial *mat = scn->mMaterials[mesh->mMaterialIndex];
		CHECK_GL_ERROR();
		subMesh->material = getSingleMeshMaterial(mat, fileName);

		glGenVertexArrays(1, &(subMesh->vertexArrayObject));
		glBindVertexArray(subMesh->vertexArrayObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (subMesh)->elementBufferObject); // bind our element array buffer (indices) to vao
		glBindBuffer(GL_ARRAY_BUFFER, (subMesh)->vertexBufferObject);
		glEnableVertexAttribArray(shader.posLocation);
		glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shader.normalLocation);
		glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
		
		glEnableVertexAttribArray(shader.texCoordLocation);
		glVertexAttribPointer(shader.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
		CHECK_GL_ERROR();
		glBindVertexArray(0);
		subMesh->numTriangles = mesh->mNumFaces;
		geometry->subMeshVector.push_back(subMesh);
	}
	return geometry;
}

Material *ObjectMeshGeometry::getSingleMeshMaterial(const aiMaterial *mat, const std::string &fileName) {
	aiColor3D color;
	aiString name;
	Material *material = new Material;

	mat->Get(AI_MATKEY_NAME, name);
	material->textureName = name.data;

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE, color);
	material->diffuse = glm::vec3(color.r, color.g, color.b);

	// mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_AMBIENT, color);
	material->ambient = glm::vec3(color.r, color.g, color.b);

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_SPECULAR, color);
	material->specular = glm::vec3(color.r, color.g, color.b);

	float shininess;
	mat->Get<float>(AI_MATKEY_SHININESS, shininess);
	material->shininess = shininess / 4.0f;  // shininess divisor-not descibed anywhere

											 // load texture image
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		// get texture name 
		mat->Get<aiString>(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
		std::string textureName = name.data;
		size_t found = fileName.find_last_of("/\\");
		if (found != std::string::npos)
			textureName.insert(0, fileName.substr(0, found + 1));
		std::cout << "Loading texture file: " << textureName << std::endl;
		material->textureId = pgr::createTexture(textureName);
	}
	return material;
}
//------------------------------------------------------------
shared_ptr<CodeMeshGeometry> CodeMeshGeometry::loadCodeMesh(LightShaderProgram &shader, std::string &textureName, const std::vector<unsigned> &triangles, const std::vector<float> &vertices) {
	shared_ptr<CodeMeshGeometry> geometry (new CodeMeshGeometry);
	SubMeshGeometry *s = new SubMeshGeometry;
	
	glGenVertexArrays(1, &(s->vertexArrayObject));
	glBindVertexArray(s->vertexArrayObject);

	glGenBuffers(1, &(s->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, s->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(shader.posLocation);
	// vertices of triangles - start at the beginning of the array
	glVertexAttribPointer(shader.posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	glEnableVertexAttribArray(shader.normalLocation);
	// normal of vertex starts after the color (interlaced array)
	glVertexAttribPointer(shader.normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// copy our temporary index array to opengl and free the array
	glGenBuffers(1, &(s->elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s->elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	s->numTriangles = triangles.size() / 3;

	Material *m = new Material;
	m->ambient = glm::vec3(0.0f, 1.0f, 1.0f);
	m->diffuse = glm::vec3(0.0f, 1.0f, 1.0f);
	m->specular = glm::vec3(0.0f, 1.0f, 1.0f);
	m->shininess = 10.0f;
	m->textureId = pgr::createTexture(textureName);
	s->material = m;
	geometry->subMeshVector.push_back(s);

	CHECK_GL_ERROR();
	return geometry;
}
//------------------------------------------------------------
DynamicTextureGeometry *DynamicTextureGeometry::loadTextureGeometry(ShaderProgram &program, const std::string textureName, std::vector<float> explosionVertexData) {
	DynamicTextureGeometry *geometry = new DynamicTextureGeometry;
	geometry->texture = pgr::createTexture(textureName);

	glGenVertexArrays(1, &(geometry->vertexArrayObject));
	glBindVertexArray(geometry->vertexArrayObject);
	glGenBuffers(1, &(geometry->vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(explosionVertexData.data()) * explosionVertexData.size(), explosionVertexData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(program.posLocation);
	// vertices of triangles - start at the beginning of the array (interlaced array)
	glVertexAttribPointer(program.posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(program.texCoordLocation);
	// texture coordinates are placed just after the position of each vertex (interlaced array)
	glVertexAttribPointer(program.texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	geometry->numTriangles = explosionVertexData.size();
	return geometry;
}
//------------------------------------------------------------
SkyBoxGeometry::SkyBoxGeometry(SkyboxShaderProgram &shaderProgram) {
	glGenVertexArrays(1, &(vertexArrayObject));
	glBindVertexArray(vertexArrayObject);
	// buffer for far plane rendering
	glGenBuffers(1, &(vertexBufferObject)); 
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shaderProgram.screenCoordLocation);
	glVertexAttribPointer(shaderProgram.screenCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	numTriangles = 2;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &(texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	auto skyboxFiles = ConfigHolder::getInstance()->SKYBOX_CUBE_TEXTURE_FILES;
	for (size_t i = 0; i < skyboxFiles.size(); ++i) {
		if (!pgr::loadTexImage2D(skyboxFiles[i], targets[i])) {
			pgr::dieWithError("Skybox cube map loading failed!");
		}
		std::cout << "Loading cube map texture: " << skyboxFiles[i] << std::endl;
		CHECK_GL_ERROR();
	}
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	CHECK_GL_ERROR();
}
