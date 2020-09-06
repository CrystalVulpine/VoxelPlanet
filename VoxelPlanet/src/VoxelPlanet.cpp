/*! \mainpage VoxelPlanet documentation
 *
 * \section intro_sec Introduction
 *
 * This is the main documentation page for VoxelPlanet. Here you will find the documentation for all of the global functions, variables, and definitions available in the game.
 */

#include "global.hpp"
#include "World.hpp"
#include "rendering.hpp"
#include "mods.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>

World mainWorld;
Player player;

Settings settings;

bool isCubeSelected = false;

bool gameRunning = false;
bool gamePaused = false;

Screen * openedScreen = NULL;
ColorScreen colorScreen;


void runGameLoop() {
	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool escPressed = false;
	bool f1Pressed = false;
	bool mMousePress = false;
	bool f2Pressed = false;
	bool cPressed = false;

	Clock loopTime;
	Clock lastLoopTime = currentTimeMs();

	Clock lastTickTime = currentTimeMs();
	Clock tickTime;

	Clock clickClock = currentTimeMs();

	gameRunning = true;

	while (glfwWindowShouldClose(window) == 0 && gameRunning) {
		if (!escPressed && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			if (openedScreen == NULL) {
				gamePaused = !gamePaused;
			} else {
				openedScreen = NULL;
				colorScreen.changeCubeColor = false;
			}

			if (!gamePaused) {
				glfwSetCursorPos(window, (double)settings.windowWidth / 2.0, (double)settings.windowHeight / 2.0);
				if (!settings.debugMode && openedScreen == NULL) {
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
			settings.hideGUI = !settings.hideGUI;
		} else if (f1Pressed && glfwGetKey(window, GLFW_KEY_F1) != GLFW_PRESS) {
			f1Pressed = false;
		}

		if (!f2Pressed && glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
			f2Pressed = true;

			char timeString[96] = "screenshots/";
			time_t rawtime;
			time(&rawtime);
			strftime(&timeString[12], sizeof(char[80]),"%d-%m-%Y %H-%M-%S", localtime(&rawtime));
			strcat(timeString, ".png");
			takeScreenshot(timeString, "screenshots/");
		} else if (f2Pressed && glfwGetKey(window, GLFW_KEY_F2) != GLFW_PRESS) {
			f2Pressed = false;
		}


		glfwGetWindowSize(window, &settings.windowWidth, &settings.windowHeight);

		// we want to control the speed of things like moving the player
		loopTime = currentTimeMs();
		isCubeSelected = !gamePaused && openedScreen == NULL;

		if (!gamePaused) {
			if (openedScreen == NULL) {
				if (!cPressed && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
					cPressed = true;
					openedScreen = &colorScreen;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				} else if (cPressed && glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS) {
					cPressed = false;
				}

				if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
					player.selectedSlot = 0;
				} else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
					player.selectedSlot = 1;
				} else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
					player.selectedSlot = 2;
				} else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
					player.selectedSlot = 3;
				} else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
					player.selectedSlot = 4;
				} else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
					player.selectedSlot = 5;
				} else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
					player.selectedSlot = 6;
				} else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
					player.selectedSlot = 7;
				} else if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
					player.selectedSlot = 8;
				} else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
					player.selectedSlot = 9;
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

				player.move(forward, upward, sideways);
				player.rotate(((float)mouseX - ((float)settings.windowWidth / 2.0f)) / 180.0f, ((float)mouseY - ((float)settings.windowHeight / 2.0f)) / 180.0f);

				glfwSetCursorPos(window, (double)settings.windowWidth / 2.0, (double)settings.windowHeight / 2.0);

				// here's where we trace a ray from the player to a cube in the world
				RayTraceInfo raySelection = mainWorld.rayTraceCubes(glm::vec3(player.xPos, player.yPos, player.zPos), player.rotationYaw, player.rotationPitch, 10.0f);

				if (raySelection.cubeFound) {
					float xd = floorf(raySelection.pos.x);
					float yd = floorf(raySelection.pos.y);
					float zd = floorf(raySelection.pos.z);

					renderCubeSelect(xd, yd, zd);

					int x = (int)xd;
					int y = (int)yd;
					int z = (int)zd;

					// mouse clicking functions for placing/deleting cubes
					if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
						clickClock = currentTimeMs();

						// calling glfwGetKey here is necessary since otherwise it won't queue the key state, which would cause the fill to activate after any Ctrl press, even if it were released before left clicking
						glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
						glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL);
						if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
							mainWorld.fillCubes(player.inventory[player.selectedSlot], x, y, z);
							worldIsDirty = true;
						} else {
							unsigned int * const __restrict cube = mainWorld.getCubePointer((int)std::floor(raySelection.lastPos.x), (int)std::floor(raySelection.lastPos.y), (int)std::floor(raySelection.lastPos.z));
							if (cube != NULL && *cube == 0) {
								*cube = player.inventory[player.selectedSlot];
								worldIsDirty = true;
							}
						}
					}
					if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
						clickClock = currentTimeMs();

						unsigned int * const __restrict cube = mainWorld.getCubePointer(x, y, z);
						if (cube != NULL && *cube > 0) {
							*cube = 0;
							worldIsDirty = true;
						}
					}

					if (!mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
						mMousePress = true;
						const unsigned int b = mainWorld.getCube(x, y, z);
						if (b > 0) {
							player.inventory[player.selectedSlot] = b;
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
			} else if (openedScreen != NULL) {
				openedScreen->screenTick(mouseX, mouseY);
			}
		}

		mods_onGameLoop(loopTime, lastLoopTime);

		lastLoopTime = loopTime;

		doDrawTick();
	}
}

int main(int argc, char * argv[]) {
	srand(time(NULL));

	loadMods();
	mods_testFunc();

	int worldLength = 64;
	int worldWidth = 64;
	int worldHeight = 64;

	bool customSaveDir = false;

	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "--debug") == 0) {
			settings.debugMode = true;
		} else if (strcmp(argv[i], "--antialiasing") == 0) {
			if (argc > i + 1) {
				settings.antialiasingLevel = atoi(argv[i + 1]);
			}
		} else if (strcmp(argv[i], "--brightness") == 0) {
			if (argc > i + 1) {
				settings.worldBrightness = atof(argv[i + 1]);
			}
		} else if (strcmp(argv[i], "--skycolor") == 0) {
			if (argc > i + 3) {
				settings.skyColorRed = (float)atoi(argv[i + 1]) / 255.0f;
				settings.skyColorGreen = (float)atoi(argv[i + 2]) / 255.0f;
				settings.skyColorBlue = (float)atoi(argv[i + 3]) / 255.0f;
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
		} else if (strcmp(argv[i], "--fastgraphics") == 0) {
			settings.fancyGraphics = false;
		}
	}
	mods_processGameArgs(argc, argv);

	mods_onGameStart();

	if (!customSaveDir) {
		mainWorld.setSaveDir("world");
	}

	mainWorld.startWorld(worldLength, worldWidth, worldHeight);


	int glErrorCode = setupOpenGL();
	if (glErrorCode != 0) return glErrorCode;

	runGameLoop();

	cleanupOpenGL();

	mainWorld.closeWorld();

	mods_onGameExit();

	return 0;
}
