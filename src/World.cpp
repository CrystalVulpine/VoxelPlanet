#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "World.hpp"
#include "global.h"

World::World() {
	blocks = (unsigned int*) malloc(WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT * sizeof(unsigned int)); // yes I know, malloc in C++ is non-conventional. But it gets the job done.

	// generate a new world if one doesn't exist, otherwise load the save file
	struct stat st;
	if (stat("world/blocks.dat", &st) != 0) {
		// very simple flat terrain generator for now
		for (unsigned int z = 0; z < WORLD_WIDTH; ++z) {
			const unsigned int temp = z * WORLD_WIDTH * WORLD_HEIGHT; // stores this index number so it doesn't have to be recomputed every time a block is set
			for (unsigned int x = 0; x < WORLD_WIDTH; ++x) {
				const unsigned int temp1 = temp + x * WORLD_HEIGHT;
				blocks[temp1 + 0] = blockBedrock;
				for (unsigned int y = 1; y < 11; ++y) {
					blocks[temp1 + y] = blockStone;
				}
				for (unsigned int y = 11; y < 15; ++y) {
					blocks[temp1 + y] = blockDirt;
				}
				blocks[temp1 + 15] = blockGrass;
				for (unsigned int y = 16; y < WORLD_HEIGHT; ++y) {
					blocks[temp1 + y] = 0;
				}
			}
		}
	} else {
		std::ifstream save("world/blocks.dat", std::ios::binary | std::ios::ate);
		std::streamsize size = save.tellg();
		save.seekg(0, std::ios::beg);
		unsigned char* __restrict__ data = (unsigned char*)malloc(size);
		save.read((char*)data, size);

		unsigned int worldIndex = 0;
		for (unsigned int index = 0; index < size; index += 5) {
			unsigned char count = data[index];
			unsigned int blockID = (unsigned int)data[index + 1] << 24 | (unsigned int)data[index + 2] << 16 | (unsigned int)data[index + 3] << 8 | (unsigned int)data[index + 4];
			for (unsigned int i = 0; i < count; ++i) {
				blocks[worldIndex + i] = blockID;
			}
			worldIndex += count;
		}

		free(data);
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

void World::saveWorld() {
	struct stat st;
	if (stat("world", &st) != 0) {
		if (mkdir("world", 0777) != 0) {
			printf("Could not save world: directory 'world' cannot be created.\n");
			return;
		}
	}
	std::ofstream save("world/blocks.dat");

	// one byte for cube color count, 4 for color/blockID
	unsigned char * __restrict__ data = (unsigned char*)malloc(WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT * 5);
	unsigned int index = 0;
	for (unsigned int i = 0; i < WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT;) {
		// this does some (very basic) compression; not particularly effective
		unsigned int prevBlockID = blocks[i];
		unsigned int blockID = blocks[i];
		unsigned char count = 0;
		while (prevBlockID == blockID) {
			++count;
			if (count >= 255 || i + count >= WORLD_WIDTH * WORLD_WIDTH * WORLD_HEIGHT) {
				break;
			}
			blockID = blocks[i + count];
		}
		data[index] = count;
		// ensure big endian so save files don't get corrupted between platforms
		data[index + 1] = (unsigned char)((prevBlockID >> 24) & 0xff);
		data[index + 2] = (unsigned char)((prevBlockID >> 16) & 0xff);
		data[index + 3] = (unsigned char)((prevBlockID >> 8) & 0xff);
		data[index + 4] = (unsigned char)(prevBlockID & 0xff);

		i += count;
		index += 5;
	}
	save.write((char*)data, index);
	free(data);
}
