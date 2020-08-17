#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera() {

}

void Camera::move(float forward, float vertical, float sideways) {
	xPos += forward * glm::sin(rotationYaw) + sideways * glm::cos(-rotationYaw);
	zPos -= forward * glm::cos(rotationYaw) + sideways * glm::sin(-rotationYaw);
	yPos += vertical;

	if (xPos < minX) {
		xPos = minX;
	} else if (xPos > maxX) {
		xPos = maxX;
	}
	if (zPos < minZ) {
		zPos = minZ;
	} else if (zPos > maxZ) {
		zPos = maxZ;
	}
}

void Camera::rotate(float yaw, float pitch) {
	rotationYaw += yaw;
	rotationPitch += pitch;

	// this prevents the camera from rotating too far up or down
	if (rotationPitch > M_PI / 2.0f) {
		rotationPitch = M_PI / 2.0f;
	} else if (rotationPitch < -M_PI / 2.0f) {
		rotationPitch = -M_PI / 2.0f;
	}
}

void Camera::setBounds(float lx, float hx, float lz, float hz) {
	minX = lx;
	maxX = hx;
	minZ = lz;
	maxZ = hz;
}

glm::mat4 Camera::getMatrix(float windowSizeRatio) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), windowSizeRatio, 0.01f, (float)(renderDistance * 16));
	glm::mat4 view = glm::rotate(model, rotationPitch, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(model, rotationYaw, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(-xPos, -yPos, -zPos));
	return model * projection * view;
}

void Camera::initCamera(float x, float y, float z, float yaw, float pitch) {
	xPos = x;
	yPos = y;
	zPos = z;
	rotationYaw = yaw;
	rotationPitch = pitch;
	minX = minZ = 0.0f;
	maxX = maxZ = 256.0f;
	renderDistance = 16;
}
