#include <stdio.h>
#include "global.hpp"

Clock lastTime;
Clock now;

int disintegrationX;
int disintegrationY;
int disintegrationZ;
unsigned int radius = 1;


void disintegrateBlocks() {

	now = currentTimeMs();

	// we're doing a lot of cube changes, so limit them to once time per second so we're not eating up all the resources
	if (now - lastTime >= 1000) {

		for (unsigned int i = 0; i < radius * radius; ++i) {
			mainWorld.setCube(disintegrationX + (rand() % radius) - (rand() % radius), disintegrationY + (rand() % radius) - (rand() % radius), disintegrationZ +  + (rand() % radius) - (rand() % radius), 0);
		}

		++radius;

		// make sure to render the world again once the cubes are gone!
		worldIsDirty = true;

		lastTime = now;
	}
}

void findDisintegrationPos() {
	int x = rand() % mainWorld.worldLength;
	int z = rand() % mainWorld.worldWidth;

	for (int y = mainWorld.worldHeight - 1; y >= 0; --y) {

		if (mainWorld.getCube(x, y, z)) {
			disintegrationX = x;
			disintegrationY = y;
			disintegrationZ = z;
			return;
		}
	}
}

#ifdef __cplusplus
extern "C" {
#endif

/** this function checks the version used for the mod against the version in the base game **/
const char* getApiVersion() {
	return "1.0.1";
}

const char* getModVersion() {
	return "1.0.0";
}

const char* getModName() {
	return "Disintegration";
}

const char* getModDescription() {
	return "Simple mod that disintegrates cubes in the world. DO NOT USE ON AN IMPORTANT SAVE!";
}

void onWorldLoad() {
	findDisintegrationPos();
}

void onWorldTick() {
	disintegrateBlocks();
}

#ifdef __cplusplus
}
#endif
