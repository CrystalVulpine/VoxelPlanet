#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct RayTraceInfo {
	bool cubeFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:
	int worldLength;
	int worldWidth;
	int worldHeight;

	unsigned int* __restrict__ cubes; // if WORLD_WIDTH=256 and WORLD_HEIGHT=128, the index would be z << 15 | x << 7 | y. That's the same as ((z * 32768) + x * 128) + y.

	/** Starts the world, with the specified size if it is generating a new one. **/
	void startWorld(const int length, const int width, const int height);

	void setCube(const int x, const int y, const int z, const unsigned int cube);
	unsigned int getCube(const int x, const int y, const int z);

	/** Returns a pointer to the cube at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int* getCubePointer(const int x, const int y, const int z);

	/** saves the cube data in world/cubes.dat **/
	void saveWorld();

	/** Traces from the specified position to the first solid cube within reach distance. Returns the position of the cube and the previous position. **/
	RayTraceInfo rayTraceCubes(glm::vec3 startPos, float rotationYaw, float rotationPitch, float reach);

	World();
};
