#include <stdio.h>
#include "global.hpp"


void generateCompass() {

	if (!mainWorld.isNewWorld) {
		return;
	}

	int centerX = mainWorld.worldLength / 2;
	int centerZ = mainWorld.worldWidth / 2;
	int top = 16;

	// generate the 4 quadrant colors
	for (int x = 0; x < mainWorld.worldLength; ++x) {
		for (int z = 0; z < mainWorld.worldWidth; ++z) {
			if (x < centerX) {
				if (z < centerZ) {
					mainWorld.setCube(x, top, z, 0xf5f471ff);
				} else {
					mainWorld.setCube(x, top, z, 0x00adf9ff);
				}
			} else {
				if (z < centerZ) {
					mainWorld.setCube(x, top, z, 0x89ec9eff);
				} else {
					mainWorld.setCube(x, top, z, 0xff6e77ff);
				}
			}
		}
	}

	// add the black lines in between quadrants
	for (int x = 0; x < mainWorld.worldLength; ++x) {
		mainWorld.setCube(x, top, centerZ, 0x000000ff);
	}
	for (int z = 0; z < mainWorld.worldWidth; ++z) {
		mainWorld.setCube(centerX, top, z, 0x000000ff);
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
	return "Political Compass";
}

const char* getModDescription() {
	return "Small mod that generates a political compass in new worlds.";
}

void onWorldLoad() {
	generateCompass();
}


#ifdef __cplusplus
}
#endif
