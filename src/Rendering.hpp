#ifndef RENDERING_HPP_
#define RENDERING_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "World.hpp"
#include "Blocks.hpp"
#include "VoxelPlanet.hpp"

GLuint vertexbuffer;
GLuint colorbuffer;

unsigned int vertexIndex = 0;

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
		colors[i + 0] *= red;
		colors[i + 1] *= green;
		colors[i + 2] *= blue;
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


#endif
