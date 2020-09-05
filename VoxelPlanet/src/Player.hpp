#pragma once

#include <glm/glm.hpp>

class Player {
public:
	float xPos;
	float yPos;
	float zPos;
	float rotationYaw;
	float rotationPitch;

	unsigned int inventory[10];
	unsigned int selectedSlot;


	/** moves the player in the specified directions **/
	void move(const float forward, const float vertical, const float sideways);

	/** adds to the rotation of the player **/
	void rotate(const float yaw, const float pitch);

	/** calculates a matrix for the camera for OpenGL **/
	glm::mat4 getMatrix(const float windowSizeRatio, const unsigned int renderDistance);

	void createPlayer();
};
