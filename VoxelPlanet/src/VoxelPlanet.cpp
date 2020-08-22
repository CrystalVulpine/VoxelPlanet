#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "global.hpp"
#include "rendering.hpp"
#include "World.hpp"
#include "mods.hpp"

bool gameRunning = false;
World mainWorld;

// don't let GLFW capture the pointer in case the application suspends.
bool debugMode = false;

/** the cube the screen/crosshair is pointing at **/
bool isCubeSelected = false;

/** the cube currently in use that would be placed **/
unsigned int usingCube = 0x808080ff;

bool gamePaused = false;
unsigned int openedScreen = 0;
bool hideGUI = false;


int main(int argc, char *argv[]) {

	srand(time(NULL));

	loadMods();
	mods_testFunc();

	int worldLength = 64;
	int worldWidth = 64;
	int worldHeight = 64;

	bool customSaveDir = false;

	for (int i = 0; i < argc; ++i) {

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

		} else if (strcmp(argv[i], "--worlddir") == 0) {

			if (argc > i + 1) {
				mainWorld.setSaveDir(argv[i + 1]);
				customSaveDir = true;
			}
		}
	}
	mods_processGameArgs(argc, argv);

	mods_onGameStart();

	if (!customSaveDir) {
		mainWorld.setSaveDir("world");
	}

	mainWorld.startWorld(worldLength, worldWidth, worldHeight);


	int glErrorCode = setupOpenGL();

	if (glErrorCode != 0) {
		return glErrorCode;
	}

	gameRunning = true;
	camera.setBounds(0.0f, (float)mainWorld.worldLength, 0.0f, (float)mainWorld.worldWidth);

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool escPressed = false;
	bool f1Pressed = false;
	bool mMousePress = false;

	Clock loopTime;
	Clock lastLoopTime = currentTimeMs();

	Clock clickClock = currentTimeMs();

	Clock lastTickTime = currentTimeMs();
	Clock tickTime;


	bool cPressed = false;

	bool colorBarActive = false;
	bool colorAlphaBarActive = false;
	bool colorTriangleActive = false;

	while (glfwWindowShouldClose(window) == 0 && gameRunning) {

		if (!escPressed && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

			if (!openedScreen) {
				gamePaused = !gamePaused;
			} else {
				openedScreen = 0;
			}

			if (!gamePaused) {
				glfwSetCursorPos(window, (double)windowWidth / 2.0, (double)windowHeight / 2.0);

				if (!debugMode && !openedScreen) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}

			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mainWorld.saveWorld();
			}

			escPressed = true;
		} else if (escPressed && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
			escPressed = false;
		}

		if (!f1Pressed && glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
			f1Pressed = true;
			hideGUI = !hideGUI;
		} else if (f1Pressed && glfwGetKey(window, GLFW_KEY_F1) != GLFW_PRESS) {
			f1Pressed = false;
		}


		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		// we want to control the speed of things like moving the camera
		loopTime = currentTimeMs();
		isCubeSelected = !gamePaused && !openedScreen;

		if (!gamePaused) {

			if (!openedScreen) {

				if (!cPressed && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
					cPressed = true;
					openedScreen = SCREEN_COLOR;

				} else if (cPressed && glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS) {
					cPressed = false;
				}


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
					upward = (float)(loopTime - lastLoopTime) / 100.0f;
				}
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
					upward = (float)-(loopTime - lastLoopTime) / 100.0f;
				}
				forward *= (float)(loopTime - lastLoopTime) / 100.0f;
				sideways *= (float)(loopTime - lastLoopTime) / 100.0f;

				glfwGetCursorPos(window, &mouseX, &mouseY);

				camera.move(forward, upward, sideways);
				camera.rotate(((float)mouseX - ((float)windowWidth / 2.0f)) / 180.0f, ((float)mouseY - ((float)windowHeight / 2.0f)) / 180.0f);

				glfwSetCursorPos(window, (double)windowWidth / 2.0, (double)windowHeight / 2.0);

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
							worldIsDirty = true;
						}
					}
					if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
						clickClock = currentTimeMs();

						unsigned int * __restrict__ cube = mainWorld.getCubePointer(x, y, z);
						if (cube != NULL && *cube > 0) {
							*cube = 0;
							worldIsDirty = true;
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

				tickTime = currentTimeMs();

				// run a tick 20 times per second
				unsigned int ticksToRun = (tickTime - lastTickTime) / 50;
				for (unsigned int i = 0; i <= ticksToRun; ++i) {
					tickTime = currentTimeMs();
					mods_onWorldTick();
					lastTickTime = tickTime;
				}

			} else if (openedScreen == SCREEN_COLOR) {

				if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

					glfwGetCursorPos(window, &mouseX, &mouseY);

					double mouseX2 = (mouseX / (double)windowHeight) * 2.0 - ((double)windowWidth / (double)windowHeight);
					double mouseY2 = -(mouseY / (double)windowHeight) * 2.0 + 1.0;

					double maxX = (1.0 - ((mouseY2 + 0.1) / 0.9)) * 0.6;
					if (colorTriangleActive || ((!colorBarActive && !colorAlphaBarActive) && mouseY2 <= 0.8 && mouseY2 >= -0.1 && std::abs(mouseX2) < maxX)) {

						colorTriangleActive = true;

						if (mouseY2 > 0.8) {
							mouseY2 = 0.8;
						} else if (mouseY2 < -0.1) {
							mouseY2 = -0.1;
						}

						// we have to do this again since the y pos could be changed
						maxX = (1.0 - ((mouseY2 + 0.1) / 0.9)) * 0.6;

						if (mouseX2 < -maxX) {
							mouseX2 = -maxX;
						} else if (mouseX2 > maxX) {
							mouseX2 = maxX;
						}

						colorTriangleX = mouseX2;
						colorTriangleY = mouseY2;

					} else if (colorBarActive || ((!colorTriangleActive && !colorAlphaBarActive) && mouseY2 >= -0.35 && mouseY2 <= -0.15 && std::abs(mouseX2) < 0.6)) {

						colorBarActive = true;

						if (mouseX2 < -0.6) {
							mouseX2 = -0.6;
						} else if (mouseX2 > 0.6) {
							mouseX2 = 0.6;
						}

						colorBarPos = mouseX2;

					} else if (colorAlphaBarActive || ((!colorTriangleActive && !colorBarActive) && mouseY2 <= 0.8 && mouseY2 >= -0.1 && mouseX2 >= -0.8 && mouseX2 <= -0.7)) {

						colorAlphaBarActive = true;

						if (mouseY2 > 0.8) {
							mouseY2 = 0.8;
						} else if (mouseY2 < -0.1) {
							mouseY2 = -0.1;
						}

						colorAlphaPos = mouseY2;

					}
				} else {
					colorTriangleActive = false;
					colorBarActive = false;
					colorAlphaBarActive = false;
				}
			}
		}

		mods_onGameLoop(loopTime, lastLoopTime);

		lastLoopTime = loopTime;

		doDrawTick();
	}

	cleanupOpenGL();

	mainWorld.closeWorld();

	mods_onGameExit();

	return 0;
}
