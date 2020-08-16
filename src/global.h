#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "World.hpp"

extern World mainWorld;
extern bool debugMode;
extern bool hideGUI;
extern bool gamePaused;

/** the cube the screen/crosshair is pointing at **/
extern bool isCubeSelected;

/** the cube currently in use that would be placed **/
extern unsigned int usingCube;

typedef long Clock;
Clock currentTimeMs();


#endif
