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
	unsigned char renderDistance;


	Camera();

	/** moves the camera in the specified directions **/
	void move(float forward, float vertical, float sideways);

	/** adds to the rotation of the camera **/
	void rotate(float yaw, float pitch);

	/** the camera will not move outside of this area **/
	void setBounds(float minX, float maxX, float minZ, float maxZ);

	/** calculates a matrix for the camera for OpenGL **/
	glm::mat4 getMatrix(float windowSizeRatio);

	/** Specify the position and rotation to start the camera at **/
	void initCamera(float x, float y, float z, float rotationYaw, float rotationPitch);
};
