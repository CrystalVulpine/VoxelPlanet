#pragma once

#include "World.hpp"
#include "Player.hpp"

#define _PI 3.14159265358979323846

extern World mainWorld;
extern Player player;

#define SCREEN_COLOR 1
extern bool debugMode;
extern bool hideGUI;
extern bool gamePaused;
extern bool fancyGraphics;
extern unsigned int renderDistance;

/** id of the screen that is currently open. 0 if none. **/
extern unsigned int openedScreen;

/** the cube the screen/crosshair is pointing at **/
extern bool isCubeSelected;

extern bool changeCubeColor;

/** marks the world dirty and in need of being rendered again. **/
extern bool worldIsDirty;

extern double colorTriangleX;
extern double colorTriangleY;
extern double colorBarPos;
extern double colorAlphaPos;

#ifdef __cplusplus
extern "C" {
#endif

typedef long Clock;

#ifdef __cplusplus
}
#endif

extern Clock currentTimeMs();
