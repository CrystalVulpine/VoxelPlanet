#pragma once

#include "World.hpp"
#include <experimental/filesystem>

extern World mainWorld;
extern bool debugMode;
extern bool hideGUI;
extern bool gamePaused;

/** the cube the screen/crosshair is pointing at **/
extern bool isCubeSelected;

/** the cube currently in use that would be placed **/
extern unsigned int usingCube;

/** marks the world dirty and in need of being rendered again. **/
extern bool worldIsDirty;

#define SCREEN_COLOR 1
extern double colorTriangleX;
extern double colorTriangleY;
extern double colorBarPos;
extern double colorAlphaPos;

/** id of the screen that is currently open. 0 if none. **/
extern unsigned int openedScreen;

namespace fs = std::experimental::filesystem;

#ifdef __cplusplus
extern "C" {
#endif

typedef long Clock;

#ifdef __cplusplus
}
#endif

extern Clock currentTimeMs();
