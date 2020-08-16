#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <glm/gtc/matrix_transform.hpp>

#define WORLD_WIDTH 64
#define WORLD_HEIGHT 64

typedef struct RayTraceInfo {
	bool cubeFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:
	unsigned int* __restrict__ cubes; // if WORLD_WIDTH=256 and WORLD_HEIGHT=128, the index would be z << 15 | x << 7 | y. That's the same as ((z * 32768) + x * 128) + y.

	void setCube(const int x, const int y, const int z, const unsigned int cube);
	unsigned int getCube(const int x, const int y, const int z);

	/** Returns a pointer to the cube at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int* getCubePointer(const int x, const int y, const int z);

	/** saves the cube data in world/cubes.dat **/
	void saveWorld();

	/** Traces from the specified position to the first solid cube within reach distance. Returns the position of the cube and the previous position. **/
	RayTraceInfo rayTraceCubes(glm::vec3 startPos, float rotationYaw, float rotationPitch, float reach);

	/** loads the saved world or generates a new one **/
	World();
};

#endif
