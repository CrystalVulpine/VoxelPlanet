#include <stdlib.h>
#include <stdio.h>
#include "World.hpp"
#include "global.h"

World::World() {
	blocks = (unsigned int*) malloc(WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT * sizeof(unsigned int)); // yes I know, malloc in C++ is non-conventional. But it gets the job done.

	// very simple flat terrain generator for now
	for (unsigned int z = 0; z < WORLD_WIDTH; ++z) {
		const unsigned int temp = z * WORLD_WIDTH * WORLD_HEIGHT; // stores this index number so it doesn't have to be recomputed every time a block is set
		for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
			const unsigned int temp1 = temp + x * WORLD_HEIGHT;
			blocks[temp1 + 0] = blockBedrock;
			for (unsigned int y = 1; y < 10; ++y) {
				blocks[temp1 + y] = blockStone;
			}
			int higher = true; // (rand() & 1) == 0;
			blocks[temp1 + 10] = higher ? blockStone : blockDirt;
			for (unsigned int y = 11; y < 14; ++y) {
				blocks[temp1 + y] = blockDirt;
			}
			blocks[temp1 + 14] = higher ? blockDirt : blockGrass;
			blocks[temp1 + 15] = higher ? blockGrass : 0;
			for (unsigned int y = 16; y < WORLD_HEIGHT; ++y) {
				blocks[temp1 + y] = 0;
			}
		}
	}
}

void World::setBlock(const int x, const int y, const int z, const unsigned int block) {
	if (x >= WORLD_WIDTH || x < 0 || z >= WORLD_WIDTH || z < 0 || y >= WORLD_HEIGHT || y < 0) {
		return;
	}
	blocks[((z * WORLD_WIDTH * WORLD_HEIGHT) + x * WORLD_HEIGHT) + y] = block;
}

unsigned int World::getBlock(const int x, const int y, const int z) {
	if (x >= WORLD_WIDTH || x < 0 || z >= WORLD_WIDTH || z < 0 || y >= WORLD_HEIGHT || y < 0) {
		return 0;
	}
	return blocks[((z * WORLD_WIDTH * WORLD_HEIGHT) + x * WORLD_HEIGHT) + y];
}

unsigned int* World::getBlockPointer(const int x, const int y, const int z) {
	if (x >= WORLD_WIDTH || x < 0 || z >= WORLD_WIDTH || z < 0 || y >= WORLD_HEIGHT || y < 0) {
		return NULL;
	}
	return &blocks[((z * WORLD_WIDTH * WORLD_HEIGHT) + x * WORLD_HEIGHT) + y];
}
