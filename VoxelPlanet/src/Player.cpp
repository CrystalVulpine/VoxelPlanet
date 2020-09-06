#include "Player.hpp"
#include "global.hpp"
#include "World.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Player::move(const float forward, const float vertical, const float sideways) __restrict {
	xPos += forward * glm::sin(rotationYaw) + sideways * glm::cos(-rotationYaw);
	zPos -= forward * glm::cos(rotationYaw) + sideways * glm::sin(-rotationYaw);
	yPos += vertical;

	if (xPos < 0.0f) {
		xPos = 0.0f;
	} else if (xPos > mainWorld.worldLength) {
		xPos = mainWorld.worldLength;
	}
	if (zPos < 0.0f) {
		zPos = 0.0f;
	} else if (zPos > mainWorld.worldWidth) {
		zPos = mainWorld.worldWidth;
	}
}

void Player::rotate(const float yaw, const float pitch) __restrict {
	rotationYaw += yaw;
	rotationPitch += pitch;

	// this prevents the Player from rotating too far up or down
	if (rotationPitch > glm::radians(89.0f)) {
		rotationPitch = glm::radians(89.0f);
	} else if (rotationPitch < -glm::radians(89.0f)) {
		rotationPitch = -glm::radians(89.0f);
	}
}

glm::mat4 Player::getMatrix(const float windowSizeRatio, const unsigned int renderDistance) __restrict {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), windowSizeRatio, 0.01f, (float)(renderDistance * 16));
	glm::mat4 view = glm::rotate(model, rotationPitch, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(model, rotationYaw, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(-xPos, -yPos, -zPos));
	return model * projection * view;
}

void Player::createPlayer() __restrict {
	xPos = (float)mainWorld.worldLength / 2.0f;
	yPos = 17.6f;
	zPos = (float)mainWorld.worldWidth / 2.0f;
	rotationYaw = 0.0f;
	rotationPitch = 0.0f;

	selectedSlot = 0;
}
