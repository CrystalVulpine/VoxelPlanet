#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "World.hpp"

extern World mainWorld;
extern bool debugMode;
extern bool hideGUI;
extern bool gamePaused;
extern bool isBlockSelected;
extern unsigned int heldBlock;

typedef long Clock;
Clock currentTimeMs();


#endif
