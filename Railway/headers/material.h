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

	void createTexture(const char *fileName) {
	//	pgr::createTexture(fileName);
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (!pgr::loadTexImage2D(fileName, GL_TEXTURE_2D)) {
			glBindTexture(GL_TEXTURE_2D, 0);
			glDeleteTextures(1, &textureId);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);

		// unbind the texture (just in case someone will mess up with texture calls later)
		glBindTexture(GL_TEXTURE_2D, 0);
		CHECK_GL_ERROR();
		
		CHECK_GL_ERROR();
		useTexture = true;
	}
};

#endif // __MATERIAL_H