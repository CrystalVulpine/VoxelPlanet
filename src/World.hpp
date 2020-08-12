#ifndef WORLD_HPP_
#define WORLD_HPP_

#define WORLD_WIDTH 64
#define WORLD_HEIGHT 64

class World {
public:
	unsigned int* __restrict__ blocks; // if WORLD_WIDTH=256 and WORLD_HEIGHT=128, the index would be z << 15 | x << 7 | y. That's the same as ((z * 32768) + x * 128) + y.
	void setBlock(const int x, const int y, const int z, const unsigned int block);
	unsigned int getBlock(const int x, const int y, const int z);

	/** Returns a pointer to the block at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int* getBlockPointer(const int x, const int y, const int z);
	World();
};

#endif
