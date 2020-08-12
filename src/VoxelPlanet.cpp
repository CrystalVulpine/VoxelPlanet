#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include "World.hpp"
#include "Blocks.hpp"
#include "Rendering.hpp"
#include "VoxelPlanet.hpp"

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

bool debugMode = false; // don't let GLFW capture the pointer in case the application suspends.

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

	mainWorld = World();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLchar *vertexShader = R"(
	#version 330 core
	layout(location = 0) in vec3 vertexPosition_modelspace;
    layout(location = 1) in vec3 vertexColor;
    out vec3 fragmentColor;
  
    // Values that stay constant for the whole mesh.
    uniform mat4 MVP;
  
    void main(){
        // Output position of the vertex, in clip space : MVP * position
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

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(float), NULL, GL_STATIC_DRAW);

	GLuint matrix = glGetUniformLocation(program, "MVP");

	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);

	renderWorld();

	double mouseX;
	double mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	bool esc_pressed = false;
	bool gamePaused = false;

	bool lMousePress = false;
	bool rMousePress = false;
	bool mMousePress = false;

	unsigned int selectedBlock = blockStone;

	long now;
	long lastTick = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
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
				lastTick = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
			}
			esc_pressed = true;
		} else if (esc_pressed && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
			esc_pressed = false;
		}

		glfwGetWindowSize(window, &windowWidth, &windowHeight);

		// we want to control the speed of things like moving the camera
		now = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
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
			if (!rMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
				// the mouse press checks are commented out for now to allow mass placing of cubes; they'll be useful for when ray selection is implemented.
				//rMousePress = true;

				unsigned int * __restrict__ bp = mainWorld.getBlockPointer((int)std::floor(xPos), (int)std::floor(yPos), (int)std::floor(zPos));
				if (bp != NULL && *bp == 0) {
					*bp = selectedBlock;
					reRenderWorld();
				}
			} else if (rMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
				rMousePress = false;
			}
			if (!lMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
				//lMousePress = true;
				unsigned int * __restrict__ bp = mainWorld.getBlockPointer((int)std::floor(xPos), (int)std::floor(yPos), (int)std::floor(zPos));
				if (bp != NULL && *bp > 0) {
					*bp = 0;
					reRenderWorld();
				}
			} else if (lMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
				//lMousePress = false;
			}
			if (!mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
				mMousePress = true;
				unsigned int b = mainWorld.getBlock((int)std::floor(xPos), (int)std::floor(yPos), (int)std::floor(zPos));
				if (b > 0) {
					selectedBlock = b;
				}
			} else if (mMousePress && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) != GLFW_PRESS) {
				mMousePress = false;
			}
			forward *= (float)(now - lastTick) / 100.0;
			sideways *= (float)(now - lastTick) / 100.0;
			xPos += forward * std::sin(rotationYaw) + sideways * std::cos(-rotationYaw);
			zPos -= forward * std::cos(rotationYaw) + sideways * std::sin(-rotationYaw);

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

			lastTick = std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
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

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, vertexIndex / 3);
		glDisableVertexAttribArray(0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(program);

	glfwTerminate();

	return 0;
}
