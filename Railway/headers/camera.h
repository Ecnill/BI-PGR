#ifndef __CAMERA_H
#define __CAMERA_H
#include <algorithm>
#include <iostream>
#include "spline.h"
#include "objects.h"
using namespace glm;

#define CAMERA_ANGLE_X					0.5f
#define CAMERA_ANGLE_Y					0.0f
#define CAMERA_POSITION					vec3(-1.0f, 0.05f, 0.5f)
#define CAMERA_ELEVATION_MAX			45.0f

#define STATIC_FROM_TOP_VIEW_POSITION	vec3(-3.0f, 3.0f, 7.0f)
#define STATIC_FROM_TOP_VIEW_ANGLE_X	0.0f
#define STATIC_FROM_TOP_VIEW_ANGLE_Y	-45.0f

#define STATIC_TO_TRAIN_VIEW_POSITION	vec3(1.7f, -0.9f, 0.40f)
#define STATIC_TO_TRAIN_VIEW_ANGLE_X	135.0f
#define STATIC_TO_TRAIN_VIEW_ANGLE_Y	0.0f

class Camera {
	public:
		Camera(float speed);

		enum CameraType { TOP, AT_TRAIN, FREE, FREE_MOUSE, FROM_HELICOPTER };

		void staticFromTopView();
		void staticAtTrainView();
		void dynamicView();
		void helicopterView(HelicopterObject *helicopter, float elapsedTime);
		void goLeft();
		void goRight();
		void goForward(float timeDelta);
		void goBackwards(float timeDelta);
		void lookUp();
		void lookDown();

		void setViewMatrix();
		void setProjectionMatrix(int windowWidth, int windowHeight);

		float viewAngleY;
		float currentTime;
		mat4 view;
		mat4 projection;
		CameraType actualState;
		
	private:
		vec3 checkBorders(float timeDelta, float inver);

		float speed;
		float viewAngleX;
		vec3 position;
		vec3 direction;
};

#endif // __CAMERA_H