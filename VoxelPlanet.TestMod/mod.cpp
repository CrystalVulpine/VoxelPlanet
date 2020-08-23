#include <cstdio>

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
	return "Test Mod";
}

const char* getModDescription() {
	return "Just a test mod that prints to the console when VoxelPlanet starts.";
}

void testFunc() {
	printf("This mod works!\n");
}

#ifdef __cplusplus
}
#endif
