#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "rendering.hpp"
#include "World.hpp"

bool gameRunning = false;
World mainWorld;

// don't let GLFW capture the pointer in case the application suspends.
bool debugMode = true;

unsigned int heldBlock = 0x7f7f7fff;
bool isBlockSelected = false;
bool gamePaused = false;
bool hideGUI = false;

int main(void) {
	int glErrorCode = setupOpenGL();
	if (glErrorCode != 0) {
		return glErrorCode;
	}

	gameRunning = true;
	camera.setBounds(0.0f, (float)WORLD_WIDTH / 2.0f, 0.0f, (float)WORLD_WIDTH / 2.0f);

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool esc_pressed = false;
	bool f1_pressed = false;
	bool mMousePress = false;

	Clock now;
	Clock lastTick = currentTimeMs();

	Clock clickClock = currentTimeMs();

	bool changeRed = false;
	bool changeGreen = false;
	bool changeBlue = false;
	bool numKeysPressed[10] = { false };
	unsigned char colorInput[3] = { 0 };
	unsigned int colorInputIndex = 0;

	while (glfwWindowShouldClose(window) == 0 && gameRunning) {
		if (!esc_pressed && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			gamePaused = !gamePaused;
			if (!gamePaused) {
				glfwSetCursorPos(window, (double)windowWidth / 2.0, (double)windowHeight / 2.0);
				if (!debugMode) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

				// if this is not done the game will count the pause time as elapsed
				lastTick = currentTimeMs();
			}
			esc_pressed = true;
		} else if (esc_pressed && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
			esc_pressed = false;
		}

		if (!f1_pressed && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
			f1_pressed = true;
			hideGUI = !hideGUI;
		} else if (f1_pressed && glfwGetKey(window, GLFW_KEY_F1) != GLFW_PRESS) {
			f1_pressed = false;
		}

		// this lets you type in new cube colors
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			colorInputIndex = 0;
			changeRed = true;
			changeGreen = false;
			changeBlue = false;
		} else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
			colorInputIndex = 0;
			changeRed = false;
			changeGreen = true;
			changeBlue = false;
		} else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			colorInputIndex = 0;
			changeRed = false;
			changeGreen = false;
			changeBlue = true;
		}
		for (unsigned int i = 0; i < 10; ++i) {
			if (!numKeysPressed[i] && glfwGetKey(window, i + GLFW_KEY_0) == GLFW_PRESS) {
				numKeysPressed[i] = true;
				colorInput[colorInputIndex] = i;
				++colorInputIndex;
				if (colorInputIndex >= 3) {
					if (changeRed) {
						unsigned char red = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&heldBlock) + 3) = red;
						changeRed = false;
					} else if (changeGreen) {
						unsigned char green = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&heldBlock) + 2) = green;
						changeGreen = false;
					} else if (changeBlue) {
						unsigned char blue = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&heldBlock) + 1) = blue;
						changeBlue = false;
					}
					colorInputIndex = 0;
				}
			} else if (numKeysPressed[i] && glfwGetKey(window, i + GLFW_KEY_0) != GLFW_PRESS) {
				numKeysPressed[i] = false;
			}
		}

		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		// we want to control the speed of things like moving the camera
		now = currentTimeMs();
		isBlockSelected = gamePaused;

		if (!gamePaused) {
			float forward = 0.0f;
			float sideways = 0.0f;
			float upward = 0.0f;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				forward = 1.0f;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				forward = -1.0f;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				sideways = -1.0f;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				sideways = 1.0f;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				upward = (float)(now - lastTick) / 100.0f;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				upward = (float)-(now - lastTick) / 100.0f;
			}
			forward *= (float)(now - lastTick) / 100.0f;
			sideways *= (float)(now - lastTick) / 100.0f;

			glfwGetCursorPos(window, &mouseX, &mouseY);

			camera.move(forward, upward, sideways);
			camera.rotate(((float)mouseX - ((float)windowWidth / 2.0f)) / 180.0f, ((float)mouseY - ((float)windowHeight / 2.0f)) / 180.0f);

			glfwSetCursorPos(window, (double)windowWidth / 2.0, (double)windowHeight / 2.0);

			lastTick = currentTimeMs();

			// here's where we trace a ray from the camera to a cube in the world
			struct rayTraceInfo raySelection = mainWorld.rayTraceBlocks(glm::vec3(camera.xPos, camera.yPos, camera.zPos), camera.rotationYaw, camera.rotationPitch, 6.0f);

			if (raySelection.blockFound) {
				double sx = std::floor(raySelection.pos.x);
				double sy = std::floor(raySelection.pos.y);
				double sz = std::floor(raySelection.pos.z);

				renderSelectBlock(sx, sy, sz);

				int x = (int)sx;
				int y = (int)sy;
				int z = (int)sz;

				// mouse clicking functions for placing/deleting cubes
				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ block = mainWorld.getBlockPointer((int)std::floor(raySelection.lastPos.x), (int)std::floor(raySelection.lastPos.y), (int)std::floor(raySelection.lastPos.z));
					if (block != NULL && *block == 0) {
						*block = heldBlock;
						reRenderWorld();
					}
				}
				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ block = mainWorld.getBlockPointer(x, y, z);
					if (block != NULL && *block > 0) {
						*block = 0;
						reRenderWorld();
					}
				}

				if (!mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
					mMousePress = true;
					unsigned int b = mainWorld.getBlock(x, y, z);
					if (b > 0) {
						heldBlock = b;
					}
				} else if (mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS) {
					mMousePress = false;
				}
				isBlockSelected = true;
			} else {
				isBlockSelected = false;
			}
		}

		doDrawTick();
	}

	cleanupOpenGL();

	mainWorld.saveWorld();

	return 0;
}
