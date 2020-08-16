#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "global.h"
#include "rendering.hpp"
#include "World.hpp"

bool gameRunning = false;
World mainWorld;

// don't let GLFW capture the pointer in case the application suspends.
bool debugMode = false;

/** the cube the screen/crosshair is pointing at **/
bool isCubeSelected = false;

/** the cube currently in use that would be placed **/
unsigned int usingCube = 0x808080ff;

bool gamePaused = false;
bool hideGUI = false;

int main(unsigned int argc, char *argv[]) {
	unsigned int worldLength = 64;
	unsigned int worldWidth = 64;
	unsigned int worldHeight = 64;
	for (unsigned int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "--debug") == 0) {
			debugMode = true;
		} else if (strcmp(argv[i], "--antialiasing") == 0) {
			if (argc > i + 1) {
				antialiasingLevel = atoi(argv[i + 1]);
			}
		} else if (strcmp(argv[i], "--brightness") == 0) {
			if (argc > i + 1) {
				worldBrightness = atof(argv[i + 1]);
			}
		} else if (strcmp(argv[i], "--skycolor") == 0) {
			if (argc > i + 3) {
				skyColorRed = (float)atoi(argv[i + 1]) / 255.0f;
				skyColorGreen = (float)atoi(argv[i + 2]) / 255.0f;
				skyColorBlue = (float)atoi(argv[i + 3]) / 255.0f;
			}
		} else if (strcmp(argv[i], "--worldsize") == 0) {
			if (argc > i + 3) {
				worldLength = atoi(argv[i + 1]);
				worldWidth = atoi(argv[i + 2]);
				worldHeight = atoi(argv[i + 3]);
			}
		}
	}
	mainWorld.startWorld(worldLength, worldWidth, worldHeight);

	int glErrorCode = setupOpenGL();
	if (glErrorCode != 0) {
		return glErrorCode;
	}

	gameRunning = true;
	camera.setBounds(0.0f, (float)worldLength, 0.0f, (float)worldWidth);

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

				// if this is not done the game will count the pause time as elapsed
				lastTick = currentTimeMs();
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
						*(((unsigned char*)&usingCube) + 3) = red;
						changeRed = false;
					} else if (changeGreen) {
						unsigned char green = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&usingCube) + 2) = green;
						changeGreen = false;
					} else if (changeBlue) {
						unsigned char blue = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&usingCube) + 1) = blue;
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
		isCubeSelected = !gamePaused;

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
			RayTraceInfo raySelection = mainWorld.rayTraceCubes(glm::vec3(camera.xPos, camera.yPos, camera.zPos), camera.rotationYaw, camera.rotationPitch, 6.0f);

			if (raySelection.cubeFound) {
				double xd = std::floor(raySelection.pos.x);
				double yd = std::floor(raySelection.pos.y);
				double zd = std::floor(raySelection.pos.z);

				renderCubeSelect(xd, yd, zd);

				int x = (int)xd;
				int y = (int)yd;
				int z = (int)zd;

				// mouse clicking functions for placing/deleting cubes
				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ cube = mainWorld.getCubePointer((int)std::floor(raySelection.lastPos.x), (int)std::floor(raySelection.lastPos.y), (int)std::floor(raySelection.lastPos.z));
					if (cube != NULL && *cube == 0) {
						*cube = usingCube;
						reRenderWorld();
					}
				}
				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ cube = mainWorld.getCubePointer(x, y, z);
					if (cube != NULL && *cube > 0) {
						*cube = 0;
						reRenderWorld();
					}
				}

				if (!mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
					mMousePress = true;
					unsigned int b = mainWorld.getCube(x, y, z);
					if (b > 0) {
						usingCube = b;
					}
				} else if (mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS) {
					mMousePress = false;
				}
				isCubeSelected = true;
			} else {
				isCubeSelected = false;
			}
		}

		doDrawTick();
	}

	cleanupOpenGL();

	mainWorld.saveWorld();

	return 0;
}
