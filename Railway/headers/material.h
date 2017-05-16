//---------------------------------------------------------------------------------------
/**
* \file       material.h
* \author     Julia Ostrokomorets
* \date       2017/05/01
* \brief      Header file and implementation of Material class.
*
*/
//---------------------------------------------------------------------------------------
#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "pgr.h"
using namespace glm;

struct Material {
	vec3		color;
	vec3		ambient;
	vec3		diffuse;
	vec3		specular;
	float		shininess;

	std::string textureName;
	GLuint		textureId;
	bool		useTexture;

	GLuint nIndices;
	GLuint startIndex;
	GLuint baseVertex;

	Material() : textureId(0), useTexture(false) {}

	~Material() {
		if (textureId != 0) { glDeleteTextures(1, &(textureId)); }
	}

	void createTexture(const char *fileName) {
		textureId = pgr::createTexture(fileName);
		useTexture = true;
	}
};

#endif // __MATERIAL_H