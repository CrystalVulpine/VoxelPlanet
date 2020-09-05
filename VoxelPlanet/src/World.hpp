#pragma once

#include <glm/glm.hpp>

struct RayTraceInfo {
	bool cubeFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:
	char * __restrict worldDir;
	char * __restrict cubesDatPath;
	char * __restrict levelDatPath;
	bool isNewWorld;
	bool isSaving;

	unsigned short worldLength;
	unsigned short worldWidth;
	unsigned short worldHeight;

	unsigned int * __restrict cubes;


	/** Starts the world, with the specified size if it is generating a new one. **/
	void startWorld(const int length, const int width, const int height);

	/** saves the cube data in world/cubes.dat **/
	void saveWorld();

	void closeWorld();

	void setCube(const int x, const int y, const int z, const unsigned int cube);

	unsigned int getCube(const int x, const int y, const int z);

	/** Returns a pointer to the cube at the specified coords. Useful for quick operations with its memory location. **/
	unsigned int * getCubePointer(const int x, const int y, const int z);

	/** Traces from the specified position to the first solid cube within reach distance. Returns the position of the cube and the previous position. **/
	RayTraceInfo rayTraceCubes(const glm::vec3 startPos, const float rotationYaw, const float rotationPitch, const float reach);

	/** specifies the folder in which to load/save level.dat and cubes.dat **/
	void setSaveDir(const char * const __restrict dir);

	/** fills the cubes starting at the coords with the given color **/
	void fillCubes(const unsigned int color, const int x, const int y, const int z);

	World();
};
