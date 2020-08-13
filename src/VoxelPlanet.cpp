#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include "World.hpp"
#include "rendering.h"
#include "global.h"

GLFWwindow* window;
int windowWidth = 1024;
int windowHeight = 768;

unsigned char antialiasing_level = 4;
bool gameRunning = false;
unsigned char renderDistance = 16;
World mainWorld;

float xPos = (float)WORLD_WIDTH / 2.0F;
float yPos = 17.62;
float zPos = (float)WORLD_WIDTH / 2.0F;
float rotationYaw = 0.0F;
float rotationPitch = 0.0F;

bool debugMode = true; // don't let GLFW capture the pointer in case the application suspends.

int main(void) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, antialiasing_level);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	window = glfwCreateWindow(windowWidth, windowHeight, "VoxelPlanet Alpha 0.0", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. Your GPU or CPU may not be compatible with OpenGL 3.3.");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	if (!debugMode) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW");
		getchar();
		glfwTerminate();
		return -1;
	}

	gameRunning = true;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLchar *vertexShader = R"(
	#version 330 core
	layout(location = 0) in vec3 vertexPosition_modelspace;
    layout(location = 1) in vec3 vertexColor;
    out vec3 fragmentColor;
  
    uniform mat4 MVP;
  
    void main() {
        gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
        fragmentColor = vertexColor;
    })";

	const GLchar *fragmentShader = R"(
	#version 330 core
    out vec3 color;
    in vec3 fragmentColor;
    void main() {
        color = fragmentColor;
    })";

	GLuint program = loadShaders(vertexShader, fragmentShader);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);
	glGenBuffers(1, &linebuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(double), NULL, GL_STATIC_DRAW);

	GLuint matrix = glGetUniformLocation(program, "MVP");

	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);

	renderWorld();

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool esc_pressed = false;
	bool gamePaused = false;

	bool mMousePress = false;

	unsigned int selectedBlock = blockStone;

	Clock now;
	Clock lastTick = currentTimeMs();

	Clock clickClock = currentTimeMs();

	double square[] = {
			-0.2, -0.2, 0.0,
			-0.2, 0.2, 0.0,
			0.2, 0.2, 0.0,
			-0.2, -0.2, 0.0,
			0.2, 0.2, 0.0,
			0.2, -0.2, 0.0
	};

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
						*(((unsigned char*)&selectedBlock) + 3) = red;
						changeRed = false;
					} else if (changeGreen) {
						unsigned char red = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&selectedBlock) + 2) = red;
						changeGreen = false;
					} else if (changeBlue) {
						unsigned char red = colorInput[0] * 100 + colorInput[1] * 10 + colorInput[2];
						*(((unsigned char*)&selectedBlock) + 1) = red;
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
		bool noBlockSelected = false;

		if (!gamePaused) {
			float forward = 0.0F;
			float sideways = 0.0F;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				forward = 1.0F;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				forward = -1.0F;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				sideways = -1.0F;
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				sideways = 1.0F;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				yPos += (float)(now - lastTick) / 100.0;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				yPos -= (float)(now - lastTick) / 100.0;
			}
			forward *= (float)(now - lastTick) / 100.0;
			sideways *= (float)(now - lastTick) / 100.0;
			xPos += forward * std::sin(rotationYaw) + sideways * std::cos(-rotationYaw);
			zPos -= forward * std::cos(rotationYaw) + sideways * std::sin(-rotationYaw);

			if (xPos < 0.0) {
				xPos = 0.0;
			} else if (xPos > WORLD_WIDTH) {
				xPos = WORLD_WIDTH;
			}
			if (zPos < 0.0) {
				zPos = 0.0;
			} else if (zPos > WORLD_WIDTH) {
				zPos = WORLD_WIDTH;
			}

			glfwGetCursorPos(window, &mouseX, &mouseY);

			rotationYaw += ((float)mouseX - ((float)windowWidth / 2.0)) / 180.0F;
			rotationPitch += ((float)mouseY - ((float)windowHeight / 2.0)) / 180.0F;

			// this prevents the camera from rotating too far upwards or downwards
			if (rotationPitch > 3.14159265359F / 2.0F) {
				rotationPitch = 3.14159265359F / 2.0F;
			} else if (rotationPitch < -3.14159265359F / 2.0F) {
				rotationPitch = -3.14159265359F / 2.0F;
			}

			glfwSetCursorPos(window, (double)windowWidth / 2.0, (double)windowHeight / 2.0);

			lastTick = currentTimeMs();

			// here's where we trace a ray from the camera to a cube in the world
			glm::vec3 start(xPos, yPos, zPos);
			glm::vec3 pos = start;
			glm::vec3 lastPos = start;
			int bl = 0;
			while (bl == 0) {
				lastPos = pos;
			    pos.x -= glm::cos(rotationYaw + 1.5708) * 0.05;
			    pos.z -= glm::sin(rotationYaw + 1.5708) * 0.05;
			    pos.y -= glm::tan(rotationPitch) * 0.05;
			    bl = mainWorld.getBlock(pos.x, pos.y, pos.z);
			    if (glm::distance(start, pos) > 6.0) {
			    	noBlockSelected = true;
			    	break;
			    }
			}

			// this prevents the edge case of placing a cube on a corner
		    int posDiff = 0;
		    if (std::floor(pos.x) != std::floor(lastPos.x)) {
		    	posDiff += 1;
		    }
		    if (std::floor(pos.y) != std::floor(lastPos.y)) {
		    	posDiff += 1;
		    }
		    if (std::floor(pos.z) != std::floor(lastPos.z)) {
		    	posDiff += 1;
		    }
		    if (posDiff > 1) {
		    	noBlockSelected = true;
		    }

			if (!noBlockSelected) {
				double sx = std::floor(pos.x);
				double sy = std::floor(pos.y);
				double sz = std::floor(pos.z);

				renderSelectBlock(sx, sy, sz);

				int x = (int)sx;
				int y = (int)sy;
				int z = (int)sz;

				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ bp = mainWorld.getBlockPointer((int)std::floor(lastPos.x), (int)std::floor(lastPos.y), (int)std::floor(lastPos.z));
					if (bp != NULL && *bp == 0) {
						*bp = selectedBlock;
						reRenderWorld();
					}
				}
				if (currentTimeMs() - clickClock > 200 && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
					clickClock = currentTimeMs();

					unsigned int * __restrict__ blockPointer = mainWorld.getBlockPointer(x, y, z);
					if (blockPointer != NULL && *blockPointer > 0) {
						*blockPointer = 0;
						reRenderWorld();
					}
				}

				if (!mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
					mMousePress = true;
					unsigned int b = mainWorld.getBlock(x, y, z);
					if (b > 0) {
						selectedBlock = b;
					}
				} else if (mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS) {
					mMousePress = false;
				}
			}
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float) windowWidth / (float)windowHeight, 0.01f, (float)(renderDistance * 16));
		glm::mat4 view = glm::rotate(model, rotationPitch, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(model, rotationYaw, glm::vec3(0.0, 1.0, 0.0)) * glm::translate(model, glm::vec3(-xPos, -yPos, -zPos));

		glViewport(0, 0, windowWidth, windowHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);

		glUseProgram(program);

		glm::mat4 mvp = projection * view;
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, vertexIndex / 3);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glDisableVertexAttribArray(1);

		if (!noBlockSelected) {
			glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
			glDrawArrays(GL_LINES, 0, 24);
		}

		glEnableVertexAttribArray(1);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		mvp = glm::ortho<float>(((float)windowWidth / (float)windowHeight) * 0.95f, ((float)windowWidth / (float)windowHeight) * 0.0f, 1.0f, 0.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(double), sizeof(square), square);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, vertexIndex / 3, 6);

		float red = (float)(selectedBlock >> 24) / 255.0F;
		float green = (float)((selectedBlock >> 16) & 255) / 255.0F;
		float blue = (float)((selectedBlock >> 8) & 255) / 255.0F;
		float squareColor[] = {
				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
		};
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(float), sizeof(squareColor), squareColor);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &linebuffer);

	glfwTerminate();

	return 0;
}
