#include <stdio.h>
#include "global.hpp"

Clock lastReRender;
Clock now;

void changeBlockColors() {

	// replace up to 10 blocks every tick
	for (unsigned int i = 0; i < 10; ++i) {
		int x = rand() % mainWorld.worldLength;
		int y = rand() % mainWorld.worldHeight;
		int z = rand() % mainWorld.worldWidth;

		unsigned int* __restrict__ cubePointer = mainWorld.getCubePointer(x, y, z);
		if (*cubePointer > 0) {

			// randomize between any non-transparent color
			*cubePointer = (rand() & 16777215) << 8 | 0xff;
		}
	}

	now = currentTimeMs();

	// we're doing a lot of block changes, so limit re-rendering of the world to one time per second
	if (now - lastReRender >= 1000) {
		// make sure to render the world again once the block colors are changed!
		worldIsDirty = true;
		lastReRender = now;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

/** this function checks the version used for the mod against the version in the base game **/
const char* getApiVersion() {
	return "1.0.0";
}

const char* getModVersion() {
	return "1.0.0";
}

const char* getModName() {
	return "Cube Color Randomizer";
}

const char* getModDescription() {
	return "Randomly changes the color of cubes in the world. DO NOT USE ON AN IMPORTANT SAVE!";
}

void onWorldTick() {
	changeBlockColors();
}

#ifdef __cplusplus
}
#endif
