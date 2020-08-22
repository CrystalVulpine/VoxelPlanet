#include "rendering.hpp"


GLFWwindow* window;
int windowWidth = 1024;
int windowHeight = 768;

unsigned char antialiasingLevel = 4;
float worldBrightness = 1.0f;
float skyColorRed = 0.5f;
float skyColorGreen = 0.5f;
float skyColorBlue = 1.0f;

Camera camera;

bool worldIsDirty = false;

GLuint vertexbuffer;
GLuint colorbuffer;
GLuint guiVertexBuffer;
GLuint guiColorBuffer;

unsigned int vertexIndex = 0;


double colorTriangleX = 0.0;
double colorTriangleY = 0.8;
double colorBarPos = 0.0;

unsigned int guiVertexCount;


double usingCubeVertices[] = {
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
		-0.006, 0.051, 0.0,
		0.006, 0.051, 0.0,
		0.006, -0.051, 0.0,
		0.006, -0.051, 0.0,
		-0.006, -0.051, 0.0,
		-0.006, 0.051, 0.0,

		0.051, -0.006, 0.0,
		0.051, 0.006, 0.0,
		-0.051, 0.006, 0.0,
		-0.051, 0.006, 0.0,
		-0.051, -0.006, 0.0,
		0.051, -0.006, 0.0,

		-0.004, 0.049, 0.0,
		0.004, 0.049, 0.0,
		0.004, -0.049, 0.0,
		0.004, -0.049, 0.0,
		-0.004, -0.049, 0.0,
		-0.004, 0.049, 0.0,

		0.049, -0.004, 0.0,
		0.049, 0.004, 0.0,
		-0.049, 0.004, 0.0,
		-0.049, 0.004, 0.0,
		-0.049, -0.004, 0.0,
		0.049, -0.004, 0.0,
};

float crosshairColor[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
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

void renderCube(int x, int y, int z, unsigned int cube) {
	double vertices[6 * 2 * 3 * 3];

	unsigned int vertexCount = 0;

	// attempts to speed through the surrounding cube check by going through memory as contiguously as possible. cubePointer is the cube's memory location.
	unsigned int* __restrict__ cubePointer = mainWorld.getCubePointer(x, y, z);
	bool renderFrontFace = z <= 0 || cubePointer[-(mainWorld.worldLength * mainWorld.worldHeight)] == 0;
	bool renderLeftFace = x <= 0 || cubePointer[-mainWorld.worldHeight] == 0;
	bool renderBottomFace = y <= 0 || cubePointer[-1] == 0;
	bool renderTopFace = y >= mainWorld.worldHeight - 1 || cubePointer[1] == 0;
	bool renderRightFace = x >= mainWorld.worldLength - 1 || cubePointer[mainWorld.worldHeight] == 0;
	bool renderBackFace = z >= mainWorld.worldWidth - 1 || cubePointer[mainWorld.worldLength * mainWorld.worldHeight] == 0;

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

	float red = (float)(cube >> 24) / 255.0F;
	float green = (float)((cube >> 16) & 255) / 255.0F;
	float blue = (float)((cube >> 8) & 255) / 255.0F;

	for (unsigned int i = 0; i < sizeof(colors) / sizeof(colors[0]); i += 3) {
		colors[i + 0] *= red * worldBrightness;
		colors[i + 1] *= green * worldBrightness;
		colors[i + 2] *= blue * worldBrightness;
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferSubData(GL_ARRAY_BUFFER, vertexIndex * sizeof(colors[0]), sizeof(colors[0]) * vertexCount2, colors);

	vertexIndex += vertexCount2;
}

void renderWorld() {
	for (int z = 0; z < mainWorld.worldWidth; ++z) {
		const unsigned int temp = z * mainWorld.worldLength * mainWorld.worldHeight; // stores this index number so it doesn't have to be recomputed every time a cube is set
		for (int x = 0; x < mainWorld.worldLength; ++x) {
			const unsigned int temp1 = temp + (x * mainWorld.worldHeight);
			for (int y = 0; y < mainWorld.worldHeight; ++y) {
				const unsigned int cube = mainWorld.cubes[temp1 + y];
				if (cube) {
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

void renderCubeSelect(double x, double y, double z) {
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
		fprintf(stderr, "Failed to initialize GLFW");
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, antialiasingLevel);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
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
	glGenBuffers(1, &guiVertexBuffer);
	glGenBuffers(1, &guiColorBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((mainWorld.worldLength * mainWorld.worldWidth * mainWorld.worldHeight) * 6 * 2 * 3 * 3) * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, ((mainWorld.worldLength * mainWorld.worldWidth * mainWorld.worldHeight) * 6 * 2 * 3 * 3) * sizeof(float), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(double), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, guiColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(float), NULL, GL_STATIC_DRAW);

	matrix = glGetUniformLocation(program, "MVP");

	camera.initCamera((float)mainWorld.worldLength / 2.0f, 17.6f, (float)mainWorld.worldWidth / 2.0f, 0.0f, 0.0f);

	glClearColor(skyColorRed, skyColorGreen, skyColorBlue, 0.0f);

	renderWorld();

	return 0;
}

/** adds and draws a list to the gui vbo. `count` refers to the number of vertices. **/
void renderToGUI(unsigned int count, double vertices[], float colors[]) {
	glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, guiVertexCount * 3 * sizeof(double), count * 3 * sizeof(double), vertices);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, guiColorBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, guiVertexCount * 3 * sizeof(float), count * 3 * sizeof(float), colors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, guiVertexCount, count);

	guiVertexCount += count;
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
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertexIndex / 3);


	if (worldIsDirty) {
		reRenderWorld();
		worldIsDirty = false;
	}


	if (!hideGUI) {

		guiVertexCount = 0;

		glDisableVertexAttribArray(1);

		if (isCubeSelected) {
			glBindBuffer(GL_ARRAY_BUFFER, guiVertexBuffer);
			glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
			glDrawArrays(GL_LINES, 0, 24);

			guiVertexCount += 24;
		}

		glEnableVertexAttribArray(1);

		glDisable(GL_CULL_FACE);

		glm::mat4 model = glm::mat4(1.0f);
		mvp = glm::ortho<float>(-((float)windowWidth / (float)windowHeight), (float)windowWidth / (float)windowHeight, -1.0f, 1.0f, -1.0f, 1.0f) * glm::rotate(model, 0.5f, glm::vec3(1.0, 0.0, 0.0)) * glm::translate(model, glm::vec3(((double)windowWidth / (double)windowHeight) - 0.2, 0.9, 0.0)) * glm::rotate(model, 0.785398f, glm::vec3(0.0, 1.0, 0.0));
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		float red = (float)(usingCube >> 24) / 255.0F;
		float green = (float)((usingCube >> 16) & 255) / 255.0F;
		float blue = (float)((usingCube >> 8) & 255) / 255.0F;
		float usingCubeColor[] = {
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

		renderToGUI(18, usingCubeVertices, usingCubeColor);


		model = glm::mat4(1.0f);
		mvp = glm::ortho<float>(-((float)windowWidth / (float)windowHeight), (float)windowWidth / (float)windowHeight, -1.0f, 1.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

		glDisable(GL_DEPTH_TEST);

		renderToGUI(sizeof(crosshairs) / sizeof(crosshairs[0]) / 3, crosshairs, crosshairColor);

		if (openedScreen == SCREEN_COLOR) {

			double triangle[] = {
					-0.6, -0.1, 0.0,
					0.0, 0.8, 0.0,
					0.6, -0.1, 0.0,

					-0.6, -0.3, 0.0,
					-0.6, -0.2, 0.0,
					-0.4, -0.2, 0.0,
					-0.4, -0.2, 0.0,
					-0.4, -0.3, 0.0,
					-0.6, -0.3, 0.0,

					-0.4, -0.3, 0.0,
					-0.4, -0.2, 0.0,
					-0.2, -0.2, 0.0,
					-0.2, -0.2, 0.0,
					-0.2, -0.3, 0.0,
					-0.4, -0.3, 0.0,

					-0.2, -0.3, 0.0,
					-0.2, -0.2, 0.0,
					0.0, -0.2, 0.0,
					0.0, -0.2, 0.0,
					0.0, -0.3, 0.0,
					-0.2, -0.3, 0.0,

					0.0, -0.3, 0.0,
					0.0, -0.2, 0.0,
					0.2, -0.2, 0.0,
					0.2, -0.2, 0.0,
					0.2, -0.3, 0.0,
					0.0, -0.3, 0.0,

					0.2, -0.3, 0.0,
					0.2, -0.2, 0.0,
					0.4, -0.2, 0.0,
					0.4, -0.2, 0.0,
					0.4, -0.3, 0.0,
					0.2, -0.3, 0.0,

					0.4, -0.3, 0.0,
					0.4, -0.2, 0.0,
					0.6, -0.2, 0.0,
					0.6, -0.2, 0.0,
					0.6, -0.3, 0.0,
					0.4, -0.3, 0.0,

					colorTriangleX - 0.01, colorTriangleY - 0.01, 0.0,
					colorTriangleX - 0.01, colorTriangleY + 0.01, 0.0,
					colorTriangleX + 0.01, colorTriangleY + 0.01, 0.0,
					colorTriangleX + 0.01, colorTriangleY + 0.01, 0.0,
					colorTriangleX + 0.01, colorTriangleY - 0.01, 0.0,
					colorTriangleX - 0.01, colorTriangleY - 0.01, 0.0,

					colorBarPos - 0.01, -0.35, 0.0,
					colorBarPos - 0.01, -0.15, 0.0,
					colorBarPos + 0.01, -0.15, 0.0,
					colorBarPos + 0.01, -0.15, 0.0,
					colorBarPos + 0.01, -0.35, 0.0,
					colorBarPos - 0.01, -0.35, 0.0,
			};

			float colorBarRed;
			float colorBarGreen;
			float colorBarBlue;

			if (colorBarPos > 0.0) {
				colorBarRed = (colorBarPos - 0.2f) / 0.2f;
			} else {
				colorBarRed = (-colorBarPos - 0.2f) / 0.2f;
			}
			if (colorBarPos > -0.2) {
				colorBarGreen = -(colorBarPos - 0.2f) / 0.2f;
			} else {
				colorBarGreen = 1.0f + (colorBarPos + 0.4f) / 0.2f;
			}
			if (colorBarPos > 0.2) {
				colorBarBlue = -(colorBarPos - 0.6f) / 0.2f;
			} else {
				colorBarBlue = 1.0f + (colorBarPos + 0.0) / 0.2f;
			}

			if (colorBarRed < 0.0f) {
				colorBarRed = 0.0f;
			} else if (colorBarRed > 1.0f) {
				colorBarRed = 1.0f;
			}
			if (colorBarGreen < 0.0f) {
				colorBarGreen = 0.0f;
			} else if (colorBarGreen > 1.0f) {
				colorBarGreen = 1.0f;
			}
			if (colorBarBlue < 0.0f) {
				colorBarBlue = 0.0f;
			} else if (colorBarBlue > 1.0f) {
				colorBarBlue = 1.0f;
			}

			float triangleColor[] = {
					0.0f, 0.0f, 0.0f,
					colorBarRed, colorBarGreen, colorBarBlue,
					1.0f, 1.0f, 1.0f,

					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					1.0f, 0.0f, 0.0f,

					1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f,

					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 0.0f,

					0.0f, 1.0f, 1.0f,
					0.0f, 1.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 1.0f, 1.0f,

					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,

					1.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 1.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					1.0f, 0.0f, 1.0f,

					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,

					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
			};
			renderToGUI(sizeof(triangle) / sizeof(triangle[0]) / 3, triangle, triangleColor);

			float multiplierRed = ((colorTriangleY + 0.1) / 0.9) * colorBarRed;
			multiplierRed += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
			multiplierRed += (colorTriangleX / 1.2);
			if (multiplierRed > 1.0f) {
				multiplierRed = 1.0f;
			} else if (multiplierRed < 0.0f) {
				multiplierRed = 0.0f;
			}

			float multiplierGreen = ((colorTriangleY + 0.1) / 0.9) * colorBarGreen;
			multiplierGreen += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
			multiplierGreen += (colorTriangleX / 1.2);
			if (multiplierGreen > 1.0f) {
				multiplierGreen = 1.0f;
			} else if (multiplierGreen < 0.0f) {
				multiplierGreen = 0.0f;
			}

			float multiplierBlue = ((colorTriangleY + 0.1) / 0.9) * colorBarBlue;
			multiplierBlue += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
			multiplierBlue += (colorTriangleX / 1.2);
			if (multiplierBlue > 1.0f) {
				multiplierBlue = 1.0f;
			} else if (multiplierBlue < 0.0f) {
				multiplierBlue = 0.0f;
			}

			usingCube = (unsigned char)(multiplierRed * 255.0f) << 24 | (unsigned char)(multiplierGreen * 255.0f) << 16 | (unsigned char)(multiplierBlue * 255.0f) << 8 | 0xff;
		}

		mods_onRenderTick();
	}

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}


void cleanupOpenGL() {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &guiVertexBuffer);
	glDeleteBuffers(1, &guiColorBuffer);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}
