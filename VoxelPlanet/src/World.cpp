#include "World.hpp"

#include "global.hpp"
#include "mods.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

World::World() __restrict {
	worldDir = NULL;
	cubesDatPath = NULL;
	levelDatPath = NULL;
}

void World::startWorld(const int length, const int width, const int height) __restrict {

	struct stat st;

	// generate a new world if one doesn't exist, otherwise load the save file
	if (stat(cubesDatPath, &st) != 0) {

		isNewWorld = true;

		worldLength = length;
		worldWidth = width;
		worldHeight = height;

		player.inventory[0] = 0xff0000ff;
		player.inventory[1] = 0xff8000ff;
		player.inventory[2] = 0xffff00ff;
		player.inventory[3] = 0x80ff00ff;
		player.inventory[4] = 0x00ff00ff;
		player.inventory[5] = 0x00ff80ff;
		player.inventory[6] = 0x00ffffff;
		player.inventory[7] = 0x0000ffff;
		player.inventory[8] = 0x8000ffff;
		player.inventory[9] = 0xff00ffff;

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
			std::cout << "Could not load level information. You must move the current world so that a new one can be created.\n";
			exit(1);
		}

		std::ifstream level(levelDatPath, std::ios::binary | std::ios::ate);
		std::streamsize size = level.tellg();

		if (size < 6) {
			std::cout << "Could not load world size. You must move the current world so that a new one can be created.\n";
			exit(1);
		}

		isNewWorld = false;

		level.seekg(0, std::ios::beg);
		unsigned char info[size];
		level.read((char*)info, size);
		level.close();
		worldLength = (unsigned short)info[0] << 8 | (unsigned short)info[1];
		worldWidth = (unsigned short)info[2] << 8 | (unsigned short)info[3];
		worldHeight = (unsigned short)info[4] << 8 | (unsigned short)info[5];

		if (size >= 46) {
			for (unsigned int i = 0; i < 10; ++i) {
				player.inventory[i] = (unsigned int)info[6 + (i * 4) + 0] << 24 | (unsigned int)info[6 + (i * 4) + 1] << 16 | (unsigned int)info[6 + (i * 4) + 2] << 8 | (unsigned int)info[6 + (i * 4) + 3];
			}
		} else {
			player.inventory[0] = 0xff0000ff;
			player.inventory[1] = 0xff8000ff;
			player.inventory[2] = 0xffff00ff;
			player.inventory[3] = 0x80ff00ff;
			player.inventory[4] = 0x00ff00ff;
			player.inventory[5] = 0x00ff80ff;
			player.inventory[6] = 0x00ffffff;
			player.inventory[7] = 0x0000ffff;
			player.inventory[8] = 0x8000ffff;
			player.inventory[9] = 0xff00ffff;
		}

		cubes = (unsigned int*)malloc(worldLength * worldWidth * worldHeight * sizeof(unsigned int));

		std::ifstream save(cubesDatPath, std::ios::binary | std::ios::ate);
		size = save.tellg();
		save.seekg(0, std::ios::beg);
		unsigned char * const __restrict data = (unsigned char*)malloc(size);
		save.read((char*)data, size);
		save.close();

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

	player.createPlayer();

	mods_onWorldLoad();
}

void World::closeWorld() __restrict {

	mods_onWorldClose();

	saveWorld();

	free(cubes);
	free(worldDir);
}


void World::setCube(const int x, const int y, const int z, const unsigned int cube) __restrict {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return;

	cubes[((z * worldLength * worldHeight) + x * worldHeight) + y] = cube;
}


unsigned int World::getCube(const int x, const int y, const int z) __restrict {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return 0;

	return cubes[((z * worldLength * worldHeight) + x * worldHeight) + y];
}


unsigned int* World::getCubePointer(const int x, const int y, const int z) __restrict {

	if (x >= worldLength || x < 0 || z >= worldWidth || z < 0 || y >= worldHeight || y < 0) return NULL;

	return &cubes[((z * worldLength * worldHeight) + x * worldHeight) + y];
}


void World::saveWorld() __restrict {

	struct stat st;

	if (stat(worldDir, &st) != 0 && mkdir(worldDir, 0777) != 0) {

		std::cout << "Could not save world: directory 'world' cannot be created.\n";
		return;
	}

	std::ofstream level(levelDatPath);

	unsigned char levelInfo[46];
	levelInfo[0] = (unsigned char)((worldLength >> 8) & 0xff);
	levelInfo[1] = (unsigned char)(worldLength & 0xff);
	levelInfo[2] = (unsigned char)((worldWidth >> 8) & 0xff);
	levelInfo[3] = (unsigned char)(worldWidth & 0xff);
	levelInfo[4] = (unsigned char)((worldHeight >> 8) & 0xff);
	levelInfo[5] = (unsigned char)(worldHeight & 0xff);

	for (unsigned int i = 0; i < 10; ++i) {
		levelInfo[6 + (i * 4) + 0] = (unsigned char)((player.inventory[i] >> 24) & 0xff);
		levelInfo[6 + (i * 4) + 1] = (unsigned char)((player.inventory[i] >> 16) & 0xff);
		levelInfo[6 + (i * 4) + 2] = (unsigned char)((player.inventory[i] >> 8) & 0xff);
		levelInfo[6 + (i * 4) + 3] = (unsigned char)(player.inventory[i] & 0xff);
	}

	level.write((char*)levelInfo, 46);
	level.close();

	std::ofstream save(cubesDatPath);

	// one byte for cube color count, 4 for block/cube id
	unsigned char * const __restrict data = (unsigned char*)malloc(worldLength * worldWidth * worldHeight * 5);
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
	save.close();

	free(data);
}


RayTraceInfo World::rayTraceCubes(const glm::vec3 start, const float rotationYaw, const float rotationPitch, const float reach) __restrict {
	glm::vec3 pos = start;
	unsigned int cube = 0;

	// This implementation is based on https://github.com/swr06/Minecraft/blob/6dfd1b5e07639225b1d4af9caf72ba25946bea03/Source/Core/World/World.cpp

	glm::vec3 direction;
	direction.x = -cosf(rotationPitch) * cosf(rotationYaw + glm::radians(90.0f));
	direction.y = sinf(-rotationPitch);
	direction.z = -cosf(rotationPitch) * sinf(rotationYaw + glm::radians(90.0f));
	glm::vec3 sign(direction[0] > 0, direction[1] > 0, direction[2] > 0);

	while (true) {
		const glm::vec3 tvec = (glm::floor(pos + sign) - pos) / direction;
		const float t = std::min(tvec.x, std::min(tvec.y, tvec.z));
		pos += direction * (t + 0.001f);

		glm::vec3 normal;
		for (unsigned int i = 0; i < 3; ++i) {
			normal[i] = (t == tvec[i]);

			if (sign[i]) {
				normal[i] = -normal[i];
			}
		}

	    cube = getCube((int)floorf(pos.x), (int)floorf(pos.y), (int)floorf(pos.z));

	    if (cube) {
	    	const RayTraceInfo info = {true, pos, pos + normal};
	    	return info;
	    } else if (glm::distance(start, pos) > reach) {
	    	const RayTraceInfo info = {false, pos, pos + normal};
	    	return info;
	    }
	}
}


void World::setSaveDir(const char * const __restrict dir) __restrict {

	if (worldDir != NULL) {
		free(worldDir);
	}

	const unsigned int dirLength = strlen(dir);
	worldDir = (char*)malloc(dirLength + 1 + dirLength + sizeof("/cubes.dat") + 1 + dirLength + sizeof("/level.dat") + 1);
	strcpy(worldDir, dir);

	cubesDatPath = &worldDir[dirLength + 1];
	strcpy(cubesDatPath, dir);
	strcat(cubesDatPath, "/cubes.dat");

	levelDatPath = &worldDir[dirLength + 1 + dirLength + sizeof("/cubes.dat") + 1];
	strcpy(levelDatPath, dir);
	strcat(levelDatPath, "/level.dat");
}

void World::fillCubes(const unsigned int color, const int x, const int y, const int z) __restrict {
	unsigned int* __restrict cubePointer = getCubePointer(x, y, z);
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
