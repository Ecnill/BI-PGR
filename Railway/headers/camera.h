#ifndef __CAMERA_H
#define __CAMERA_H

#include <algorithm>
#include <iostream>
#include "pgr.h"
#include "objects.h"
using namespace glm;

#define CAMERA_NORMAL_SPEED				0.75f 
#define CAMERA_ANGLE_X					0.5f
#define CAMERA_ANGLE_Y					0.0f
#define CAMERA_POSITION					vec3(-1.0f, 0.05f, 0.5f)

#define CAMERA_ELEVATION_MAX			45.0f

#define CAMERA_STATIC_VIEW1_POSITION	vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_STATIC_VIEW1_ANGLE_X		-90.0f
#define CAMERA_STATIC_VIEW1_ANGLE_Y		0.0f

#define CAMERA_STATIC_VIEW2_POSITION	vec3(-0.25f, -0.25f, 0.60f)
#define CAMERA_STATIC_VIEW2_ANGLE_X		135.0f
#define CAMERA_STATIC_VIEW2_ANGLE_Y		0.0f

struct Camera {

	enum CameraType { STATIC1, STATIC2, FREE };

	float		speed;
	float		startTime;
	float		currentTime;
	float		viewAngleX;
	float		viewAngleY;
	vec3		position;
	vec3		direction;
	CameraType	actualState;

	mat4 view;
	mat4 projection;

	Camera();
	void staticView1();
	void staticView2();
	void dynamicView();
	void goLeft();
	void goRight();
	void goForward(float timeDelta);
	void goBackwards(float timeDelta);
	void lookUp();
	void lookDown();

	void setTime(float startTime);
	void setViewMatrix();
	void setProjectionMatrix(int windowWidth, int windowHeight);

private:
	vec3 checkBorders(float timeDelta, float inver);

};

#endif // __CAMERA_H