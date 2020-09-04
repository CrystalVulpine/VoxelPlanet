#include "Player.hpp"
#include "global.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Player::move(const float forward, const float vertical, const float sideways) {
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

void Player::rotate(const float yaw, const float pitch) {
	rotationYaw += yaw;
	rotationPitch += pitch;

	// this prevents the Player from rotating too far up or down
	if (rotationPitch > M_PI / 2.0f) {
		rotationPitch = M_PI / 2.0f;
	} else if (rotationPitch < -M_PI / 2.0f) {
		rotationPitch = -M_PI / 2.0f;
	}
}

void Player::setBounds(const float lx, const float hx, const float lz, const float hz) {
	minX = lx;
	maxX = hx;
	minZ = lz;
	maxZ = hz;
}

glm::mat4 Player::getMatrix(const float windowSizeRatio, const unsigned int renderDistance) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), windowSizeRatio, 0.01f, (float)(renderDistance * 16));
	glm::mat4 view = glm::rotate(model, rotationPitch, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(model, rotationYaw, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(-xPos, -yPos, -zPos));
	return model * projection * view;
}

void Player::createPlayer(const float x, const float y, const float z, const float yaw, const float pitch) {
	xPos = x;
	yPos = y;
	zPos = z;
	rotationYaw = yaw;
	rotationPitch = pitch;
	minX = minZ = 0.0f;
	maxX = maxZ = 256.0f;

	usingCube = 0x808080ff;
}
