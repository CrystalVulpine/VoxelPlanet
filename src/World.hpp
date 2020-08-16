#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <glm/gtc/matrix_transform.hpp>

#define WORLD_WIDTH 64
#define WORLD_HEIGHT 64

struct rayTraceInfo {
	bool blockFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:
	unsigned int* __restrict__ blocks; // if WORLD_WIDTH=256 and WORLD_HEIGHT=128, the index would be z << 15 | x << 7 | y. That's the same as ((z * 32768) + x * 128) + y.

	void setBlock(const int x, const int y, const int z, const unsigned int block);
	unsigned int getBlock(const int x, const int y, const int z);

	/** Returns a pointer to the block at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int* getBlockPointer(const int x, const int y, const int z);

	/** saves the cube data in world/blocks.dat **/
	void saveWorld();

	/** Traces from the specified position to the first solid block within reach distance. Returns the position of the block and the previous position. **/
	struct rayTraceInfo rayTraceBlocks(glm::vec3 startPos, float rotationYaw, float rotationPitch, float reach);

	/** loads the saved world or generates a new one **/
	World();
};

#endif
