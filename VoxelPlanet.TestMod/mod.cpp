#include <stdio.h>

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

void modTestFunc() {
	printf("This mod works!\n");
}

#ifdef __cplusplus
}
#endif
