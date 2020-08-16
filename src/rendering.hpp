#ifndef RENDERING_H_
#define RENDERING_H_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include "World.hpp"
#include "global.h"

GLuint vertexbuffer;
GLuint colorbuffer;
GLuint linebuffer;

unsigned int vertexIndex = 0;

GLFWwindow* window;
int windowWidth = 1024;
int windowHeight = 768;
unsigned char antialiasing_level = 4;
float brightness = 1.0f;
float skyColorRed = 0.5f;
float skyColorGreen = 0.5f;
float skyColorBlue = 1.0f;

Camera camera((float)WORLD_WIDTH / 2.0f, 17.6f, (float)WORLD_WIDTH / 2.0f, 0.0f, 0.0f);

double blockGUI[] = {
		-0.1, -0.1, 0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		-0.1, -0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, -0.1, 0.1,

		-0.1, -0.1, 0.1,
		-0.1, 0.1, 0.1,
		-0.1, 0.1, -0.1,
		-0.1, -0.1, 0.1,
		-0.1, 0.1, -0.1,
		-0.1, -0.1, -0.1,

		0.1, 0.1, 0.1,
		0.1, 0.1, -0.1,
		-0.1, 0.1, -0.1,
		-0.1, 0.1, -0.1,
		-0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
};

double crosshairs[] = {
		-0.005, 0.05, 0.0,
		0.005, 0.05, 0.0,
		0.005, -0.05, 0.0,
		0.005, -0.05, 0.0,
		-0.005, -0.05, 0.0,
		-0.005, 0.05, 0.0,

		0.05, -0.005, 0.0,
		0.05, 0.005, 0.0,
		-0.05, 0.005, 0.0,
		-0.05, 0.005, 0.0,
		-0.05, -0.005, 0.0,
		0.05, -0.005, 0.0,
};

double crosshairColor[] = {
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
};

GLuint program;
GLuint matrix;
GLuint vao;

GLuint loadShaders(GLchar const * vertexShaderCode, GLchar const * fragmentShaderCode) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void renderBlock(int x, int y, int z, unsigned int block) {
	double vertices[6 * 2 * 3 * 3];

	unsigned int vertexCount = 0;

	// attempts to speed through the surrounding block check by going through memory as contiguously as possible. blockPointer is the block's memory location.
	unsigned int* __restrict__ blockPointer = mainWorld.getBlockPointer(x, y, z);
	bool renderFrontFace = z <= 0 || blockPointer[-(WORLD_WIDTH * WORLD_HEIGHT)] == 0;
	bool renderLeftFace = x <= 0 || blockPointer[-WORLD_HEIGHT] == 0;
	bool renderBottomFace = y <= 0 || blockPointer[-1] == 0;
	bool renderTopFace = y >= WORLD_HEIGHT - 1 || blockPointer[1] == 0;
	bool renderRightFace = x >= WORLD_WIDTH - 1 || blockPointer[WORLD_HEIGHT] == 0;
	bool renderBackFace = z >= WORLD_WIDTH - 1 || blockPointer[WORLD_WIDTH * WORLD_HEIGHT] == 0;

	if (renderTopFace) {
		vertices[vertexCount + 0] = (double)x + 1.0; vertices[vertexCount + 1] = (double)y + 1.0; vertices[vertexCount + 2] = (double)z + 1.0;
		vertices[vertexCount + 3] = (double)x + 1.0; vertices[vertexCount + 4] = (double)y + 1.0; vertices[vertexCount + 5] = (double)z;
		vertices[vertexCount + 6] = (double)x; vertices[vertexCount + 7] = (double)y + 1.0; vertices[vertexCount + 8] = (double)z;
		vertices[vertexCount + 9] = (double)x; vertices[vertexCount + 10] = (double)y + 1.0; vertices[vertexCount + 11] = (double)z + 1.0;
		vertices[vertexCount + 12] = (double)x + 1.0; vertices[vertexCount + 13] = (double)y + 1.0; vertices[vertexCount + 14] = (double)z + 1.0;
		vertices[vertexCount + 15] = (double)x; vertices[vertexCount + 16] = (double)y + 1.0; vertices[vertexCount + 17] = (double)z;
		vertexCount += 18;
	}
	if (renderFrontFace) {
		vertices[vertexCount + 0] = (double)x; vertices[vertexCount + 1] = (double)y + 1.0; vertices[vertexCount + 2] = (double)z;
		vertices[vertexCount + 3] = (double)x + 1.0; vertices[vertexCount + 4] = (double)y + 1.0; vertices[vertexCount + 5] = (double)z;
		vertices[vertexCount + 6] = (double)x; vertices[vertexCount + 7] = (double)y; vertices[vertexCount + 8] = (double)z;
		vertices[vertexCount + 9] = (double)x + 1.0; vertices[vertexCount + 10] = (double)y + 1.0; vertices[vertexCount + 11] = (double)z;
		vertices[vertexCount + 12] = (double)x + 1.0; vertices[vertexCount + 13] = (double)y; vertices[vertexCount + 14] = (double)z;
		vertices[vertexCount + 15] = (double)x; vertices[vertexCount + 16] = (double)y; vertices[vertexCount + 17] = (double)z;
		vertexCount += 18;
	}
	if (renderLeftFace) {
		vertices[vertexCount + 0] = (double)x; vertices[vertexCount + 1] = (double)y + 1.0; vertices[vertexCount + 2] = (double)z;
		vertices[vertexCount + 3] = (double)x; vertices[vertexCount + 4] = (double)y; vertices[vertexCount + 5] = (double)z;
		vertices[vertexCount + 6] = (double)x; vertices[vertexCount + 7] = (double)y; vertices[vertexCount + 8] = (double)z + 1.0;
		vertices[vertexCount + 9] = (double)x; vertices[vertexCount + 10] = (double)y + 1.0; vertices[vertexCount + 11] = (double)z + 1.0;
		vertices[vertexCount + 12] = (double)x; vertices[vertexCount + 13] = (double)y + 1.0; vertices[vertexCount + 14] = (double)z;
		vertices[vertexCount + 15] = (double)x; vertices[vertexCount + 16] = (double)y; vertices[vertexCount + 17] = (double)z + 1.0;
		vertexCount += 18;
	}
	if (renderBackFace) {
		vertices[vertexCount + 0] = (double)x; vertices[vertexCount + 1] = (double)y; vertices[vertexCount + 2] = (double)z + 1.0;
		vertices[vertexCount + 3] = (double)x + 1.0; vertices[vertexCount + 4] = (double)y + 1.0; vertices[vertexCount + 5] = (double)z + 1.0;
		vertices[vertexCount + 6] = (double)x; vertices[vertexCount + 7] = (double)y + 1.0; vertices[vertexCount + 8] = (double)z + 1.0;
		vertices[vertexCount + 9] = (double)x; vertices[vertexCount + 10] = (double)y; vertices[vertexCount + 11] = (double)z + 1.0;
		vertices[vertexCount + 12] = (double)x + 1.0; vertices[vertexCount + 13] = (double)y; vertices[vertexCount + 14] = (double)z + 1.0;
		vertices[vertexCount + 15] = (double)x + 1.0; vertices[vertexCount + 16] = (double)y + 1.0; vertices[vertexCount + 17] = (double)z + 1.0;
		vertexCount += 18;
	}
	if (renderRightFace) {
		vertices[vertexCount + 0] = (double)x + 1.0; vertices[vertexCount + 1] = (double)y; vertices[vertexCount + 2] = (double)z + 1.0;
		vertices[vertexCount + 3] = (double)x + 1.0; vertices[vertexCount + 4] = (double)y; vertices[vertexCount + 5] = (double)z;
		vertices[vertexCount + 6] = (double)x + 1.0; vertices[vertexCount + 7] = (double)y + 1.0; vertices[vertexCount + 8] = (double)z;
		vertices[vertexCount + 9] = (double)x + 1.0; vertices[vertexCount + 10] = (double)y; vertices[vertexCount + 11] = (double)z + 1.0;
		vertices[vertexCount + 12] = (double)x + 1.0; vertices[vertexCount + 13] = (double)y + 1.0; vertices[vertexCount + 14] = (double)z;
		vertices[vertexCount + 15] = (double)x + 1.0; vertices[vertexCount + 16] = (double)y + 1.0; vertices[vertexCount + 17] = (double)z + 1.0;
		vertexCount += 18;
	}
	if (renderBottomFace) {
		vertices[vertexCount + 0] = (double)x; vertices[vertexCount + 1] = (double)y; vertices[vertexCount + 2] = (double)z;
		vertices[vertexCount + 3] = (double)x + 1.0; vertices[vertexCount + 4] = (double)y; vertices[vertexCount + 5] = (double)z;
		vertices[vertexCount + 6] = (double)x + 1.0; vertices[vertexCount + 7] = (double)y; vertices[vertexCount + 8] = (double)z + 1.0;
		vertices[vertexCount + 9] = (double)x; vertices[vertexCount + 10] = (double)y; vertices[vertexCount + 11] = (double)z;
		vertices[vertexCount + 12] = (double)x + 1.0; vertices[vertexCount + 13] = (double)y; vertices[vertexCount + 14] = (double)z + 1.0;
		vertices[vertexCount + 15] = (double)x; vertices[vertexCount + 16] = (double)y; vertices[vertexCount + 17] = (double)z + 1.0;
		vertexCount += 18;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(vertices[0]), sizeof(vertices[0]) * vertexCount, vertices);

	GLfloat colors[6 * 2 * 3 * 3];

	int vertexCount2 = 0;
	if (renderTopFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 1.0;
		}
		vertexCount2 += 18;
	}
	if (renderFrontFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 0.8;
		}
		vertexCount2 += 18;
	}
	if (renderLeftFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 0.6;
		}
		vertexCount2 += 18;
	}
	if (renderBackFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 0.8;
		}
		vertexCount2 += 18;
	}
	if (renderRightFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 0.6;
		}
		vertexCount2 += 18;
	}
	if (renderBottomFace) {
		for (unsigned int i = 0; i < 18; ++i) {
			colors[vertexCount2 + i] = 0.5;
		}
		vertexCount2 += 18;
	}

	float red = (float)(block >> 24) / 255.0F;
	float green = (float)((block >> 16) & 255) / 255.0F;
	float blue = (float)((block >> 8) & 255) / 255.0F;

	for (unsigned int i = 0; i < sizeof(colors) / sizeof(colors[0]); i += 3) {
		colors[i + 0] *= red * brightness;
		colors[i + 1] *= green * brightness;
		colors[i + 2] *= blue * brightness;
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(colors[0]), sizeof(colors[0]) * vertexCount2, colors);

	vertexIndex += vertexCount2;
}

void renderWorld() {
	for (unsigned int z = 0; z < WORLD_WIDTH; ++z) {
		const unsigned int temp = z * WORLD_WIDTH * WORLD_HEIGHT; // stores this index number so it doesn't have to be recomputed every time a block is set
		for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
			const unsigned int temp1 = temp + (x * WORLD_HEIGHT);
			for (unsigned int y = 0; y < WORLD_HEIGHT; ++y) {
				const unsigned int block = mainWorld.blocks[temp1 + y];
				if (block > 0) {
					renderBlock(x, y, z, block);
				}
			}
		}
	}
}

void reRenderWorld() {
	vertexIndex = 0;
	renderWorld();
}

void renderSelectBlock(double sx, double sy, double sz) {
	double lines[] = {
			sx - 0.001, sy + 1.001, sz - 0.001,
			sx + 1.001, sy + 1.001, sz - 0.001,
			sx + 1.001, sy + 1.001, sz - 0.001,
			sx + 1.001, sy + 1.001, sz + 1.001,
			sx + 1.001, sy + 1.001, sz + 1.001,
			sx - 0.001, sy + 1.001, sz + 1.001,
			sx - 0.001, sy + 1.001, sz + 1.001,
			sx - 0.001, sy + 1.001, sz - 0.001,

			sx - 0.001, sy - 0.001, sz - 0.001,
			sx + 1.001, sy - 0.001, sz - 0.001,
			sx + 1.001, sy - 0.001, sz - 0.001,
			sx + 1.001, sy - 0.001, sz + 1.001,
			sx + 1.001, sy - 0.001, sz + 1.001,
			sx - 0.001, sy - 0.001, sz + 1.001,
			sx - 0.001, sy - 0.001, sz + 1.001,
			sx - 0.001, sy - 0.001, sz - 0.001,

			sx - 0.001, sy - 0.001, sz - 0.001,
			sx - 0.001, sy + 1.001, sz - 0.001,
			sx + 1.001, sy - 0.001, sz - 0.001,
			sx + 1.001, sy + 1.001, sz - 0.001,
			sx - 0.001, sy - 0.001, sz + 1.001,
			sx - 0.001, sy + 1.001, sz + 1.001,
			sx + 1.001, sy - 0.001, sz + 1.001,
			sx + 1.001, sy + 1.001, sz + 1.001,
	};
	glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lines), lines);
}

int setupOpenGL() {
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
	window = glfwCreateWindow(windowWidth, windowHeight, "VoxelPlanet Alpha v0.1", NULL, NULL);
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

	program = loadShaders(vertexShader, fragmentShader);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);
	glGenBuffers(1, &linebuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) * 6 * 2 * 3 * 3) * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(double), NULL, GL_STATIC_DRAW);

	matrix = glGetUniformLocation(program, "MVP");

	glClearColor(skyColorRed, skyColorGreen, skyColorBlue, 0.0f);

	renderWorld();

	return 0;
}

void doDrawTick() {
	glViewport(0, 0, windowWidth, windowHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glUseProgram(program);

	glm::mat4 mvp = camera.getMatrix((float)windowWidth / (float)windowHeight);
	glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertexIndex / 3);

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	if (!hideGUI) {
		glDisableVertexAttribArray(1);

		if (isBlockSelected) {
			glLineWidth(1.0f);
			glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
			glDrawArrays(GL_LINES, 0, 24);
		}

		glEnableVertexAttribArray(1);

		glDisable(GL_CULL_FACE);

		glm::mat4 model = glm::mat4(1.0f);
		mvp = glm::ortho<float>(-((float)windowWidth / (float)windowHeight), (float)windowWidth / (float)windowHeight, -1.0f, 1.0f, -1.0f, 1.0f) * glm::rotate(model, 0.5f, glm::vec3(1.0, 0.0, 0.0)) * glm::translate(model, glm::vec3(((double)windowWidth / (double)windowHeight) - 0.2, 0.9, 0.0)) * glm::rotate(model, 0.785398f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		const int blockGUIVertexCount = 18;

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(double), sizeof(blockGUI), blockGUI);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, vertexIndex / 3, blockGUIVertexCount);

		float red = (float)(heldBlock >> 24) / 255.0F;
		float green = (float)((heldBlock >> 16) & 255) / 255.0F;
		float blue = (float)((heldBlock >> 8) & 255) / 255.0F;
		float blockGUIColor[] = {
				red * 0.8f, green * 0.8f, blue * 0.8f,
				red * 0.8f, green * 0.8f, blue * 0.8f,
				red * 0.8f, green * 0.8f, blue * 0.8f,
				red * 0.8f, green * 0.8f, blue * 0.8f,
				red * 0.8f, green * 0.8f, blue * 0.8f,
				red * 0.8f, green * 0.8f, blue * 0.8f,

				red * 0.6f, green * 0.6f, blue * 0.6f,
				red * 0.6f, green * 0.6f, blue * 0.6f,
				red * 0.6f, green * 0.6f, blue * 0.6f,
				red * 0.6f, green * 0.6f, blue * 0.6f,
				red * 0.6f, green * 0.6f, blue * 0.6f,
				red * 0.6f, green * 0.6f, blue * 0.6f,

				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
				red, green, blue,
		};
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(float), sizeof(blockGUIColor), blockGUIColor);

		model = glm::mat4(1.0f);
		mvp = glm::ortho<float>(-((float)windowWidth / (float)windowHeight), (float)windowWidth / (float)windowHeight, -1.0f, 1.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(double) + sizeof(blockGUI), sizeof(crosshairs), crosshairs);
		glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
		glDrawArrays(GL_TRIANGLES, vertexIndex / 3 + blockGUIVertexCount, 12);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(float) + sizeof(blockGUIColor), sizeof(crosshairColor), crosshairColor);
		glDisable(GL_BLEND);
	}

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void cleanupOpenGL() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &linebuffer);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}


#endif
