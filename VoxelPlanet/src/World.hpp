#pragma once

#include <glm/glm.hpp>

/**
 * Information about a ray trace.
 *
 * @param cubeFound is whether the trace collided with a cube or not.
 *
 * @param pos is the ending coordinates of the ray trace.
 *
 * @param lastPos is the coordinates of the ray trace immediately before the last ones (\p pos). Useful for using the position outside of a collided cube.
 */
struct RayTraceInfo {
	bool cubeFound;
	glm::vec3 pos;
	glm::vec3 lastPos;
};

class World {
public:

	/** Folder that the world is stored in. */
	char * __restrict worldDir;

	/** Relative path to cubes.dat. */
	char * __restrict cubesDatPath;

	/** Relative path to level.dat. */
	char * __restrict levelDatPath;

	/** Was the world newly created or loaded from a file? */
	bool isNewWorld;

	/** Length (x) of the world in cubes. */
	unsigned short worldLength;

	/** Width (z) of the world in cubes. */
	unsigned short worldWidth;

	/** Height (y) of the world in cubes. */
	unsigned short worldHeight;

	/** An array of all the cubes in the world, in the order of x, z, y. */
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

	/** Specifies the folder in which to load/save level.dat and cubes.dat **/
	void setSaveDir(const char * const __restrict dir);

	/** Fills the cubes starting at the coords with the given color **/
	void fillCubes(const unsigned int color, const int x, const int y, const int z);

	/** Initializes critical variables so they don't mess things up. **/
	World();
};
