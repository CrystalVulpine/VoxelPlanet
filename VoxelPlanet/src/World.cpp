#include "World.hpp"

#include "global.hpp"
#include "mods.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

World::World() {

	worldDir = NULL;
	cubesDatPath = NULL;
	levelDatPath = NULL;
	isSaving = false;
}

void World::startWorld(const int length, const int width, const int height) {

	struct stat st;

	// generate a new world if one doesn't exist, otherwise load the save file
	if (stat(cubesDatPath, &st) != 0) {

		isNewWorld = true;

		worldLength = length;
		worldWidth = width;
		worldHeight = height;

		// yes I know, malloc in C++ is non-conventional. But it gets the job done.
		cubes = (unsigned int*)malloc(worldLength * worldWidth * worldHeight * sizeof(unsigned int));

		// very simple flat terrain generator for now
		for (int z = 0; z < worldWidth; ++z) {

			 // store this index number in a temp variable so it doesn't have to be recomputed
			const int temp = z * worldLength * worldHeight;

			for (int x = 0; x < worldLength; ++x) {

				const int temp1 = temp + x * worldHeight;

				cubes[temp1 + 0] = 0x333333ff;

				for (unsigned int y = 1; y < 11; ++y) {
					cubes[temp1 + y] = 0x808080ff;
				}

				for (unsigned int y = 11; y < 15; ++y) {
					cubes[temp1 + y] = 0x662000ff;
				}

				cubes[temp1 + 15] = 0x00bf00ff;

				for (int y = 16; y < worldHeight; ++y) {
					cubes[temp1 + y] = 0;
				}
			}
		}

	} else {

		if (stat(levelDatPath, &st) != 0) {

			printf("Could not load level information. You must move the current world so that a new one can be created.\n");
			exit(1);
		}

		std::ifstream level(levelDatPath, std::ios::binary | std::ios::ate);
		std::streamsize size = level.tellg();

		if (size < 6) {

			printf("Could not load world size. You must move the current world so that a new one can be created.\n");
			exit(1);
		}

		isNewWorld = false;

		level.seekg(0, std::ios::beg);
		unsigned char info[6];
		level.read((char*)info, size);
		worldLength = (unsigned short)info[0] << 8 | (unsigned short)info[1];
		worldWidth = (unsigned short)info[2] << 8 | (unsigned short)info[3];
		worldHeight = (unsigned short)info[4] << 8 | (unsigned short)info[5];
		cubes = (unsigned int*)malloc(worldLength * worldWidth * worldHeight * sizeof(unsigned int));

		std::ifstream save(cubesDatPath, std::ios::binary | std::ios::ate);
		size = save.tellg();
		save.seekg(0, std::ios::beg);
		unsigned char* __restrict__ data = (unsigned char*)malloc(size);
		save.read((char*)data, size);

		unsigned int worldIndex = 0;

		for (unsigned int index = 0; index < size; index += 5) {

			unsigned char count = data[index];
			unsigned int cube = (unsigned int)data[index + 1] << 24 | (unsigned int)data[index + 2] << 16 | (unsigned int)data[index + 3] << 8 | (unsigned int)data[index + 4];

			for (unsigned int i = 0; i < count; ++i) {
				cubes[worldIndex + i] = cube;
			}

			worldIndex += count;
		}

		free(data);
	}

	mods_onWorldLoad();
}

void World::closeWorld() {

	mods_onWorldClose();

	saveWorld();

	free(cubes);
	free(worldDir);
	free(cubesDatPath);
	free(levelDatPath);
}


void World::setCube(const int x, const int y, const int z, const unsigned int cube) {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return;

	cubes[((z * worldLength * worldHeight) + x * worldHeight) + y] = cube;
}


unsigned int World::getCube(const int x, const int y, const int z) {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return 0;

	return cubes[((z * worldLength * worldHeight) + x * worldHeight) + y];
}


unsigned int* World::getCubePointer(const int x, const int y, const int z) {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return NULL;

	return &cubes[((z * worldLength * worldHeight) + x * worldHeight) + y];
}


void World::saveWorld() {

	if (!isSaving) {

		isSaving = true;

		struct stat st;

		if (stat(worldDir, &st) != 0 && mkdir(worldDir, 0777) != 0) {

			printf("Could not save world: directory 'world' cannot be created.\n");
			return;
		}

		std::ofstream level(levelDatPath);

		unsigned char worldSizeInfo[6];
		worldSizeInfo[0] = (worldLength >> 8) & 0xff;
		worldSizeInfo[1] = worldLength & 0xff;
		worldSizeInfo[2] = (worldWidth >> 8) & 0xff;
		worldSizeInfo[3] = worldWidth & 0xff;
		worldSizeInfo[4] = (worldHeight >> 8) & 0xff;
		worldSizeInfo[5] = worldHeight & 0xff;

		level.write((char*)worldSizeInfo, 6);


		std::ofstream save(cubesDatPath);

		// one byte for cube color count, 4 for block/cube id
		unsigned char * __restrict__ data = (unsigned char*)malloc(worldLength * worldWidth * worldHeight * 5);
		unsigned int index = 0;

		for (int i = 0; i < worldLength * worldWidth * worldHeight;) {

			// this does some (very basic) compression; not particularly effective
			unsigned int prevCube = cubes[i];
			unsigned int cube = cubes[i];
			unsigned char count = 0;

			while (prevCube == cube) {

				++count;
				if (count >= 255 || i + count >= worldLength * worldWidth * worldHeight) break;

				cube = cubes[i + count];
			}

			data[index] = count;

			// ensure big endian so save files don't get corrupted between platforms
			data[index + 1] = (unsigned char)((prevCube >> 24) & 0xff);
			data[index + 2] = (unsigned char)((prevCube >> 16) & 0xff);
			data[index + 3] = (unsigned char)((prevCube >> 8) & 0xff);
			data[index + 4] = (unsigned char)(prevCube & 0xff);

			i += count;
			index += 5;
		}

		save.write((char*)data, index);

		free(data);

		isSaving = false;
	}
}


RayTraceInfo World::rayTraceCubes(glm::vec3 start, float rotationYaw, float rotationPitch, float reach) {

	bool couldFindCube = false;

	glm::vec3 pos = start;
	glm::vec3 lastPos = start;
	unsigned int cube = 0;

	while (!cube) {

		lastPos = pos;
	    pos.x -= glm::cos(rotationYaw + 1.5708f) * 0.05f;
	    pos.z -= glm::sin(rotationYaw + 1.5708f) * 0.05f;

	    // tan can be undefined, so still move y when that happens
	    if (rotationPitch <= -M_PI / 2.0f) {
	    	pos.y += 1.0f;
	    } else if (rotationPitch >= M_PI / 2.0f) {
	    	pos.y -= 1.0f;
	    } else {
	    	pos.y -= glm::tan(rotationPitch) * 0.05f;
	    }

	    cube = getCube(pos.x, pos.y, pos.z);

	    if (glm::distance(start, pos) > reach) {
	    	RayTraceInfo info = {couldFindCube, pos, lastPos};
	    	return info;
	    }
	}

	// this prevents the edge case of placing a cube on a corner
    int posDiff = 0;

    if (std::floor(pos.x) != std::floor(lastPos.x)) {
    	posDiff += 1;
    }
    if (std::floor(pos.y) != std::floor(lastPos.y)) {
    	posDiff += 1;
    }
    if (std::floor(pos.z) != std::floor(lastPos.z)) {
    	posDiff += 1;
    }
    if (posDiff == 1) {
    	couldFindCube = true;
    }

	RayTraceInfo info = {couldFindCube, pos, lastPos};
	return info;
}


void World::setSaveDir(const char* __restrict__ dir) {

	if (worldDir != NULL) {
		free(worldDir);
	}

	worldDir = (char*)malloc(strlen(dir) + 1);
	strcpy(worldDir, dir);

	if (cubesDatPath != NULL) {
		free(cubesDatPath);
	}

	cubesDatPath = (char*)malloc(strlen(dir) + sizeof("/cubes.dat") + 1);
	strcpy(cubesDatPath, dir);
	strcat(cubesDatPath, "/cubes.dat");

	if (levelDatPath != NULL) {
		free(levelDatPath);
	}

	levelDatPath = (char*)malloc(strlen(dir) + sizeof("/level.dat") + 1);
	strcpy(levelDatPath, dir);
	strcat(levelDatPath, "/level.dat");
}

void World::fillCubes(const unsigned int color, int x, int y, int z) {
	unsigned int* __restrict__ cubePointer = getCubePointer(x, y, z);
	if (cubePointer == NULL) return;

	const unsigned int originalColor = *cubePointer;
	if (originalColor == color) return;
	*cubePointer = color;

	if (getCube(x, y, z + 1) == originalColor) {
		fillCubes(color, x, y, z + 1);
	}
	if (getCube(x + 1, y, z) == originalColor) {
		fillCubes(color, x + 1, y, z);
	}
	if (getCube(x, y + 1, z) == originalColor) {
		fillCubes(color, x, y + 1, z);
	}
	if (getCube(x, y - 1, z) == originalColor) {
		fillCubes(color, x, y - 1, z);
	}
	if (getCube(x - 1, y, z) == originalColor) {
		fillCubes(color, x - 1, y, z);
	}
	if (getCube(x, y, z - 1) == originalColor) {
		fillCubes(color, x, y, z - 1);
	}
}
