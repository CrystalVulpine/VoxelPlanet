#pragma once

#include <glm/glm.hpp>

class Player {
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

	unsigned int inventory[9];
	unsigned int selectedSlot;

	/** the cube currently in use that would be placed **/
	unsigned int usingCube;


	/** moves the player in the specified directions **/
	void move(const float forward, const float vertical, const float sideways);

	/** adds to the rotation of the player **/
	void rotate(const float yaw, const float pitch);

	/** the player will not move outside of this area **/
	void setBounds(const float minX, const float maxX, const float minZ, const float maxZ);

	/** calculates a matrix for the camera for OpenGL **/
	glm::mat4 getMatrix(const float windowSizeRatio, const unsigned int renderDistance);

	void createPlayer(const float x, const float y, const float z, const float rotationYaw, const float rotationPitch);
};
