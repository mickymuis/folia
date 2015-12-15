/*
 * This file is part of Folia, an experimental mini-engine using OpenGL 3
 * Created by Micky Faas. Freely usable and modifiable for academic purposes.
 */

#define GLM_SWIZZLE
#include "camera.h"
#include "../utils/glm/glm.hpp"
#include "../utils/glm/gtc/type_ptr.hpp"
#include "../utils/glm/gtc/matrix_transform.hpp"
#include "../utils/glm/gtx/transform.hpp"
#include "../utils/glm/gtx/quaternion.hpp"

Camera::Camera( glm::vec3 position, glm::vec3 target, double fov ) {
	camera_up = glm::vec3(0, 1, 0);
	field_of_view = fov;
	camera_position_delta = glm::vec3(0, 0, 0);
	camera_position =position;
	camera_target =target;
	camera_pitch =0.0f;
	camera_heading =0.0f;
	max_pitch_rate = 5;
	max_heading_rate = 5;
	near_clip =0.1;
	far_clip =1000.0;
	orbit_yaw =0;
	orbit_pitch =0;
	update();
}

Camera::~Camera() {
}


void 
Camera::update() {
	camera_direction = glm::normalize(camera_target - camera_position);
	float distance_to_target = glm::distance( camera_target, camera_position );

	projection = glm::perspective(field_of_view, camera_aspect, near_clip, far_clip);
	//detmine axis for pitch rotation
	glm::vec3 axis = glm::cross(camera_direction, camera_up);
	//compute quaternion for pitch based on the camera pitch angle
	glm::quat pitch_quat = glm::angleAxis(camera_pitch, axis);
	//determine heading quaternion from the camera up vector and the heading angle
	glm::quat heading_quat = glm::angleAxis(camera_heading, camera_up);
	//add the two quaternions
	glm::quat temp = glm::cross(pitch_quat, heading_quat);
	temp = glm::normalize(temp);
	//update the direction from the quaternion
	camera_direction = glm::rotate(temp, camera_direction);
	//add the camera delta
	camera_position += camera_position_delta;
	//set the look at to be infront of the camera
	camera_target = camera_position + camera_direction * distance_to_target;

	//compute the view matrix
	view = glm::lookAt(camera_position, camera_target, camera_up);
	//view =glm::rotate( view, orbit_yaw, camera_up );
	
	//damping for smooth camera
	camera_heading *= .5;
	camera_pitch *= .5;
	camera_position_delta = camera_position_delta * .8f;
	orbit_yaw =0;
	orbit_pitch =0;
}

void 
Camera::setPosition(glm::vec3 pos) {
	camera_position = pos;
}

void 
Camera::setTarget(glm::vec3 pos) {
	camera_target = pos;
}
void 
Camera::setFOV(double fov) {
	field_of_view = fov;
}

void 
Camera::setClipping(double near_clip_distance, double far_clip_distance) {
	near_clip = near_clip_distance;
	far_clip = far_clip_distance;
}

void 
Camera::moveLocal(CameraDirection dir, float distance) {
	switch (dir) {
		case UP:
			camera_position_delta += camera_up * distance;
			break;
		case DOWN:
			camera_position_delta -= camera_up * distance;
			break;
		case LEFT:
			camera_position_delta -= glm::cross(camera_direction, camera_up) * distance;
			break;
		case RIGHT:
			camera_position_delta += glm::cross(camera_direction, camera_up) * distance;
			break;
		case FORWARD:
			camera_position_delta += camera_direction * distance;
			break;
		case BACK:
			camera_position_delta -= camera_direction * distance;
			break;
	}
}

void 
Camera::rotate(float heading, float pitch) {

	camera_pitch += pitch;

	//Check bounds for the camera pitch
	if (camera_pitch > 360.0f) {
		camera_pitch -= 360.0f;
	} else if (camera_pitch < -360.0f) {
		camera_pitch += 360.0f;
	}

	//This controls how the heading is changed if the camera is pointed straight up or down
	//The heading delta direction changes
	if ( (camera_pitch > 90 && camera_pitch < 270) || (camera_pitch < -90 && camera_pitch > -270)) {
		camera_heading -= heading;
	} else {
		camera_heading += heading;
	}
	//Check bounds for the camera heading
	if (camera_heading > 360.0f) {
		camera_heading -= 360.0f;
	} else if (camera_heading < -360.0f) {
		camera_heading += 360.0f;
	}
}

void 
Camera::orbit( float yaw, float pitch ) {

	orbit_yaw +=yaw;
	orbit_pitch +=pitch;
}

