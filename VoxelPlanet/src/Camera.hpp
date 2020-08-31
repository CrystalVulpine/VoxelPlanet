#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	float xPos;
	float yPos;
	float zPos;
	float rotationYaw;
	float rotationPitch;
	float minX;
	float maxX;
	float minZ;
	float maxZ;


	Camera();

	/** moves the camera in the specified directions **/
	void move(const float forward, const float vertical, const float sideways);

	/** adds to the rotation of the camera **/
	void rotate(const float yaw, const float pitch);

	/** the camera will not move outside of this area **/
	void setBounds(const float minX, const float maxX, const float minZ, const float maxZ);

	/** calculates a matrix for the camera for OpenGL **/
	glm::mat4 getMatrix(const float windowSizeRatio);

	/** Specify the position and rotation to start the camera at **/
	void initCamera(const float x, const float y, const float z, const float rotationYaw, const float rotationPitch);
};
