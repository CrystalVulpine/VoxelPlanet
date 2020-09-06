#pragma once

#include <glm/glm.hpp>

class Player {
public:
	float xPos;
	float yPos;
	float zPos;

	/** Rotation yaw of the player in radians. */
	float rotationYaw;

	/** Rotation pitch of the player in radians. */
	float rotationPitch;


	/** An array of cubes in the inventory/hotbar. */
	unsigned int inventory[10];

	/** The current inventory slot. */
	unsigned int selectedSlot;


	/** moves the player in the specified directions **/
	void move(const float forward, const float vertical, const float sideways);

	/** adds to the rotation of the player **/
	void rotate(const float yaw, const float pitch);

	/** calculates a matrix for the camera for OpenGL **/
	glm::mat4 getMatrix(const float windowSizeRatio, const unsigned int renderDistance);

	/** Initializes the player. **/
	void createPlayer();
};
