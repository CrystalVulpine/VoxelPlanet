#include <png.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string>
#include "rendering.hpp"

#include "global.hpp"
#include "mods.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>


GLFWwindow * __restrict window;

bool worldIsDirty = false;

static GLuint vertexbuffer;
static GLuint colorbuffer;
static GLuint guiVertexBuffer;
static GLuint guiColorBuffer;

static unsigned int vertexIndex = 0;
static unsigned int guiVertexCount;

static double crosshairs[] = {
		-0.006, -0.051, 0.0,
		0.006, -0.051, 0.0,
		0.006, 0.051, 0.0,
		0.006, 0.051, 0.0,
		-0.006, 0.051, 0.0,
		-0.006, -0.051, 0.0,

		0.051, -0.006, 0.0,
		0.051, 0.006, 0.0,
		-0.051, 0.006, 0.0,
		-0.051, 0.006, 0.0,
		-0.051, -0.006, 0.0,
		0.051, -0.006, 0.0,

		-0.004, -0.049, 0.0,
		0.004, -0.049, 0.0,
		0.004, 0.049, 0.0,
		0.004, 0.049, 0.0,
		-0.004, 0.049, 0.0,
		-0.004, -0.049, 0.0,

		0.049, -0.004, 0.0,
		0.049, 0.004, 0.0,
		-0.049, 0.004, 0.0,
		-0.049, 0.004, 0.0,
		-0.049, -0.004, 0.0,
		0.049, -0.004, 0.0,
};

static float crosshairColor[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
};

static GLuint vao;
GLuint GLprogram;
GLuint GLmatrix;

GLuint loadShaders(const GLchar * const __restrict vertexShaderPath, const GLchar * const __restrict fragmentShaderPath) {
	struct stat st;
	if (stat(vertexShaderPath, &st) != 0) {
		std::cout << "Could not load shader " << vertexShaderPath << ", exiting\n";
		exit(-1);
	} else if (stat(fragmentShaderPath, &st) != 0) {
		std::cout << "Could not load shader " << fragmentShaderPath << ", exiting\n";
		exit(-1);
	}

	std::ifstream vertexStream;
	vertexStream.open(vertexShaderPath);
	vertexStream.seekg(0, std::ios::end);
	unsigned int vertexLength = vertexStream.tellg();
	std::ifstream fragmentStream;
	fragmentStream.open(fragmentShaderPath);
	fragmentStream.seekg(0, std::ios::end);
	unsigned int fragmentLength = fragmentStream.tellg();
	unsigned char * const __restrict vertexShaderCode = (unsigned char*)malloc(sizeof(char[vertexLength + 1 + fragmentLength + 1]));
	vertexStream.seekg(0, std::ios::beg);
	vertexStream.read((char*)vertexShaderCode, vertexLength);
	vertexShaderCode[vertexLength] = '\0';
	vertexStream.close();
	unsigned char * const __restrict fragmentShaderCode = &vertexShaderCode[vertexLength + 1];
	fragmentStream.seekg(0, std::ios::beg);
	fragmentStream.read((char*)fragmentShaderCode, fragmentLength);
	fragmentShaderCode[fragmentLength] = '\0';
	fragmentStream.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const GLchar * const *)&vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, (const GLchar * const *)&fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		GLint errorLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLength);
		char error[errorLength];
		glGetShaderInfoLog(vertexShader, errorLength, &errorLength, error);
		std::cout << vertexShaderPath << ": " << error;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		GLint errorLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &errorLength);
		char error[errorLength];
		glGetShaderInfoLog(fragmentShader, errorLength, &errorLength, error);
		std::cout << fragmentShaderPath << ": " << error;
	}

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

static void renderCube(int x, int y, int z, unsigned int cube) {
	double vertices[6 * 2 * 3 * 3];

	unsigned int vertexCount = 0;

	// attempts to speed through the surrounding cube check by going through memory as contiguously as possible. cubePointer is the cube's memory location.
	const unsigned int * const __restrict cubePointer = mainWorld.getCubePointer(x, y, z);
	bool renderFrontFace = z > 0 && (cubePointer[-(mainWorld.worldLength * mainWorld.worldHeight)] & 0xff) != (cube & 0xff);
	bool renderLeftFace = x > 0 && (cubePointer[-mainWorld.worldHeight] & 0xff) != (cube & 0xff);
	bool renderBottomFace = y <= 0 || (cubePointer[-1] & 0xff) != (cube & 0xff);
	bool renderTopFace = y >= mainWorld.worldHeight - 1 || (cubePointer[1] & 0xff) != (cube & 0xff);
	bool renderRightFace = x < mainWorld.worldLength - 1 && (cubePointer[mainWorld.worldHeight] & 0xff) != (cube & 0xff);
	bool renderBackFace = z < mainWorld.worldWidth - 1 && (cubePointer[mainWorld.worldLength * mainWorld.worldHeight] & 0xff) != (cube & 0xff);

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

	GLfloat colors[6 * 2 * 3 * 4];

	int vertexCount2 = 0;
	if (renderTopFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 1.0;
			colors[vertexCount2 + i + 1] = 1.0;
			colors[vertexCount2 + i + 2] = 1.0;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}
	if (renderFrontFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 0.8;
			colors[vertexCount2 + i + 1] = 0.8;
			colors[vertexCount2 + i + 2] = 0.8;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}
	if (renderLeftFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 0.6;
			colors[vertexCount2 + i + 1] = 0.6;
			colors[vertexCount2 + i + 2] = 0.6;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}
	if (renderBackFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 0.8;
			colors[vertexCount2 + i + 1] = 0.8;
			colors[vertexCount2 + i + 2] = 0.8;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}
	if (renderRightFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 0.6;
			colors[vertexCount2 + i + 1] = 0.6;
			colors[vertexCount2 + i + 2] = 0.6;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}
	if (renderBottomFace) {
		for (unsigned int i = 0; i < 24; i += 4) {
			colors[vertexCount2 + i + 0] = 0.5;
			colors[vertexCount2 + i + 1] = 0.5;
			colors[vertexCount2 + i + 2] = 0.5;
			colors[vertexCount2 + i + 3] = 1.0;
		}
		vertexCount2 += 24;
	}

	float red = (float)(cube >> 24) / 255.0f;
	float green = (float)((cube >> 16) & 255) / 255.0f;
	float blue = (float)((cube >> 8) & 255) / 255.0f;
	float alpha = (float)(cube & 255) / 255.0f;

	for (unsigned int i = 0; i < sizeof(colors) / sizeof(colors[0]); i += 4) {
		colors[i + 0] *= red * settings.worldBrightness;
		colors[i + 1] *= green * settings.worldBrightness;
		colors[i + 2] *= blue * settings.worldBrightness;
		colors[i + 3] *= alpha;
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, (vertexIndex / 3) * 4 * sizeof(colors[0]), sizeof(colors[0]) * vertexCount2, colors);

	vertexIndex += vertexCount;
}

unsigned int vertexPass1 = 0;

void renderWorld() {

	// render all solid cubes in pass 1
	for (int z = 0; z < mainWorld.worldWidth; ++z) {
		const unsigned int temp = z * mainWorld.worldLength * mainWorld.worldHeight; // stores this index number so it doesn't have to be recomputed every time a cube is set
		for (int x = 0; x < mainWorld.worldLength; ++x) {
			const unsigned int temp1 = temp + (x * mainWorld.worldHeight);
			for (int y = 0; y < mainWorld.worldHeight; ++y) {
				const unsigned int cube = mainWorld.cubes[temp1 + y];
				if (cube && (cube & 0xff) == 255) {
					renderCube(x, y, z, cube);
				}
			}
		}
	}

	// render all transparent cubes in pass 2
	vertexPass1 = vertexIndex;
	for (int z = 0; z < mainWorld.worldWidth; ++z) {
		const unsigned int temp = z * mainWorld.worldLength * mainWorld.worldHeight; // stores this index number so it doesn't have to be recomputed every time a cube is set
		for (int x = 0; x < mainWorld.worldLength; ++x) {
			const unsigned int temp1 = temp + (x * mainWorld.worldHeight);
			for (int y = 0; y < mainWorld.worldHeight; ++y) {
				const unsigned int cube = mainWorld.cubes[temp1 + y];
				if (cube && (cube & 0xff) < 255) {
					renderCube(x, y, z, cube);
				}
			}
		}
	}
}

void reRenderWorld() {
	vertexIndex = 0;
	renderWorld();
}

void renderCubeSelect(const double x, const double y, const double z) {
	double lines[] = {
			x - 0.001, y + 1.001, z - 0.001,
			x + 1.001, y + 1.001, z - 0.001,
			x + 1.001, y + 1.001, z - 0.001,
			x + 1.001, y + 1.001, z + 1.001,
			x + 1.001, y + 1.001, z + 1.001,
			x - 0.001, y + 1.001, z + 1.001,
			x - 0.001, y + 1.001, z + 1.001,
			x - 0.001, y + 1.001, z - 0.001,

			x - 0.001, y - 0.001, z - 0.001,
			x + 1.001, y - 0.001, z - 0.001,
			x + 1.001, y - 0.001, z - 0.001,
			x + 1.001, y - 0.001, z + 1.001,
			x + 1.001, y - 0.001, z + 1.001,
			x - 0.001, y - 0.001, z + 1.001,
			x - 0.001, y - 0.001, z + 1.001,
			x - 0.001, y - 0.001, z - 0.001,

			x - 0.001, y - 0.001, z - 0.001,
			x - 0.001, y + 1.001, z - 0.001,
			x + 1.001, y - 0.001, z - 0.001,
			x + 1.001, y + 1.001, z - 0.001,
			x - 0.001, y - 0.001, z + 1.001,
			x - 0.001, y + 1.001, z + 1.001,
			x + 1.001, y - 0.001, z + 1.001,
			x + 1.001, y + 1.001, z + 1.001,
	};
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lines), lines);
}

int setupOpenGL() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, settings.antialiasingLevel);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	window = glfwCreateWindow(settings.windowWidth, settings.windowHeight, "VoxelPlanet v0.2", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to open GLFW window. Your GPU or CPU may not be compatible with OpenGL 3.3.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	if (!settings.debugMode) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW\n";
		glfwTerminate();
		return -1;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLprogram = loadShaders("assets/shaders/world/vertex.glsl", "assets/shaders/world/fragment.glsl");

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &colorbuffer);
	glGenBuffers(1, &guiVertexBuffer);
	glGenBuffers(1, &guiColorBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((mainWorld.worldLength * mainWorld.worldWidth * mainWorld.worldHeight) * 6 * 2 * 3 * 3) * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((mainWorld.worldLength * mainWorld.worldWidth * mainWorld.worldHeight) * 6 * 2 * 3 * 4) * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2000 * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, guiColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2000 * sizeof(float), NULL, GL_STATIC_DRAW);

	GLmatrix = glGetUniformLocation(GLprogram, "MVP");

	glClearColor(settings.skyColorRed, settings.skyColorGreen, settings.skyColorBlue, 1.0f);

	renderWorld();

	return 0;
}

void renderToGUI(unsigned int count, const double * const __restrict vertices, const float * const __restrict colors) {
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, guiVertexCount * 3 * sizeof(double), count * 3 * sizeof(double), vertices);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, guiColorBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, guiVertexCount * 4 * sizeof(float), count * 4 * sizeof(float), colors);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, guiVertexCount, count);

	guiVertexCount += count;
}

void doDrawTick() {
	glViewport(0, 0, settings.windowWidth, settings.windowHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glDepthMask(true);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(GLprogram);

	glm::mat4 mvp = player.getMatrix((float)settings.windowWidth / (float)settings.windowHeight, 16);
	glUniformMatrix4fv(GLmatrix, 1, GL_FALSE, &mvp[0][0]);

	glDisable(GL_BLEND);

	if (worldIsDirty) {
		reRenderWorld();
		worldIsDirty = false;
	}

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertexPass1 / 3);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (settings.fancyGraphics) {
		glDisable(GL_CULL_FACE);
		glDepthMask(true);
		glColorMask(false, false, false, false);
		glDrawArrays(GL_TRIANGLES, vertexPass1 / 3, (vertexIndex - vertexPass1) / 3);
		glDepthMask(false);
		glColorMask(true, true, true, true);
		glDrawArrays(GL_TRIANGLES, vertexPass1 / 3, (vertexIndex - vertexPass1) / 3);
	} else {
		glDrawArrays(GL_TRIANGLES, vertexPass1 / 3, (vertexIndex - vertexPass1) / 3);
	}


	glEnable(GL_CULL_FACE);
	glDepthMask(false);

	if (!settings.hideGUI || openedScreen != NULL) {
		guiVertexCount = 0;

		glDisableVertexAttribArray(1);

		if (isCubeSelected) {
			glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
			glDrawArrays(GL_LINES, 0, 24);

			guiVertexCount += 24;
		}

		glEnableVertexAttribArray(1);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvpb = glm::ortho<float>(-((float)settings.windowWidth / (float)settings.windowHeight), (float)settings.windowWidth / (float)settings.windowHeight, -1.0f, 1.0f, -1.0f, 1.0f);

		for (unsigned int i = 0; i < 10; ++i) {
			mvp = mvpb * glm::translate(model, glm::vec3((float)i / 5.0f - 0.9f, -0.8f, 0.0)) * glm::rotate(model, 0.5f, glm::vec3(1.0, 0.0, 0.0)) * glm::rotate(model, 0.785398f, glm::vec3(0.0, 1.0, 0.0));
			glUniformMatrix4fv(GLmatrix, 1, GL_FALSE, &mvp[0][0]);

			const unsigned int usingCube = player.inventory[i];
			float red = (float)(usingCube >> 24) / 255.0f;
			float green = (float)((usingCube >> 16) & 255) / 255.0f;
			float blue = (float)((usingCube >> 8) & 255) / 255.0f;
			float alpha = (float)(usingCube & 255) / 255.0f;
			float usingCubeColor[] = {
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
					red * 0.8f, green * 0.8f, blue * 0.8f, alpha,

					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
					red * 0.6f, green * 0.6f, blue * 0.6f, alpha,

					red, green, blue, alpha,
					red, green, blue, alpha,
					red, green, blue, alpha,
					red, green, blue, alpha,
					red, green, blue, alpha,
					red, green, blue, alpha,
			};
			const double size = i == player.selectedSlot ? 0.06 : 0.05;
			double usingCubeVertices[] = {
					-size, size, size,
					-size, -size, size,
					size, -size, size,
					-size, size, size,
					size, -size, size,
					size, size, size,

					-size, -size, size,
					-size, size, size,
					-size, size, -size,
					-size, -size, size,
					-size, size, -size,
					-size, -size, -size,

					size, size, size,
					size, size, -size,
					-size, size, -size,
					-size, size, -size,
					-size, size, size,
					size, size, size,
			};
			renderToGUI(18, usingCubeVertices, usingCubeColor);
		}


		model = glm::mat4(1.0f);
		mvp = glm::ortho<float>(-((float)settings.windowWidth / (float)settings.windowHeight), (float)settings.windowWidth / (float)settings.windowHeight, -1.0f, 1.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(GLmatrix, 1, GL_FALSE, &mvp[0][0]);

		glDisable(GL_DEPTH_TEST);

		renderToGUI(sizeof(crosshairs) / sizeof(crosshairs[0]) / 3, crosshairs, crosshairColor);

		if (gamePaused || openedScreen != NULL) {

			// render darkened background
			double background[] = {
					-(double)settings.windowWidth / (double)settings.windowHeight, -1.0, 0.0,
					(double)settings.windowWidth / (double)settings.windowHeight, -1.0, 0.0,
					(double)settings.windowWidth / (double)settings.windowHeight, 1.0, 0.0,
					(double)settings.windowWidth / (double)settings.windowHeight, 1.0, 0.0,
					-(double)settings.windowWidth / (double)settings.windowHeight, 1.0, 0.0,
					-(double)settings.windowWidth / (double)settings.windowHeight, -1.0, 0.0
			};
			float backgroundColor[] = {
					0.5f, 0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f, 0.5f,
					0.5f, 0.5f, 0.5f, 0.5f,
			};
			renderToGUI(6, background, backgroundColor);
		}

		if (openedScreen != NULL) {
			openedScreen->renderScreen();
		}

		mods_onRenderTick();
	}

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void takeScreenshot(const char * const __restrict filename, const char * const __restrict folder) {
    unsigned char * const __restrict pixels = (unsigned char*)malloc(sizeof(char[settings.windowWidth * settings.windowHeight * 3]));
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, settings.windowWidth, settings.windowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	struct stat st;
    if (stat(folder, &st) != 0 && mkdir(folder, 0777) != 0) {
    	std::cout << "Could not create screenshot directory\n";
    	return;
    } else if (stat(filename, &st) == 0) {
    	std::cout << "Could not take screenshot, file already exists!\n";
    	return;
    }
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    FILE * const __restrict file = fopen(filename, "wb");
    png_init_io(png, file);
    png_set_IHDR(png, info, settings.windowWidth, settings.windowHeight, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_colorp palette = (png_colorp)png_malloc(png, sizeof(png_color[PNG_MAX_PALETTE_LENGTH]));
    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);
    png_bytepp rows = (png_bytepp)png_malloc(png, sizeof(png_bytep[settings.windowHeight]));
    for (int i = 0; i < settings.windowHeight; ++i) {
        rows[i] = (png_bytep)(&pixels[(settings.windowHeight - i - 1) * settings.windowWidth * 3]);
    }
    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_free(png, rows);
	free(pixels);
    png_destroy_write_struct(&png, &info);
    fclose(file);
	std::cout << "Saved screenshot '" << filename << "'\n";
}


void cleanupOpenGL() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &guiVertexBuffer);
	glDeleteBuffers(1, &guiColorBuffer);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}
