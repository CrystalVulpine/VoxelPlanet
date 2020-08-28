#pragma once

#include <glm/glm.hpp>

struct RayTraceInfo {
	bool cubeFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:
	char* __restrict__ worldDir;
	char* __restrict__ cubesDatPath;
	char* __restrict__ levelDatPath;

	int worldLength;
	int worldWidth;
	int worldHeight;

	unsigned int* __restrict__ cubes;

	bool isNewWorld;
	bool isSaving;

	/** Starts the world, with the specified size if it is generating a new one. **/
	void startWorld(const int length, const int width, const int height);

	void closeWorld();

	void setCube(const int x, const int y, const int z, const unsigned int cube);

	unsigned int getCube(const int x, const int y, const int z);

	/** Returns a pointer to the cube at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int* getCubePointer(const int x, const int y, const int z);

	/** saves the cube data in world/cubes.dat **/
	void saveWorld();

	/** Traces from the specified position to the first solid cube within reach distance. Returns the position of the cube and the previous position. **/
	RayTraceInfo rayTraceCubes(glm::vec3 startPos, float rotationYaw, float rotationPitch, float reach);

	/** specifies the folder in which to load/save level.dat and cubes.dat **/
	void setSaveDir(const char* __restrict__ dir);

	/** fills the cubes starting at the coords with the given color **/
	void fillCubes(const unsigned int color, int x, int y, int z);

	World();
};
