#pragma once

#include "World.hpp"
#include "Player.hpp"

#define _PI 3.14159265358979323846

extern World mainWorld;
extern Player player;

/** Id of the color picker screen. */
#define SCREEN_COLOR 1

/** This enables debugging stuff, such as not capturing the pointer (in case the program freezes). */
extern bool debugMode;

extern bool hideGUI;
extern bool gamePaused;
extern bool fancyGraphics;

/** How many 16-cube areas are visible in any given direction. */
extern unsigned int renderDistance;

/** id of the screen that is currently open. 0 if none. **/
extern unsigned int openedScreen;

/** the cube the screen/crosshair is pointing at **/
extern bool isCubeSelected;

/** marks the world dirty and in need of being rendered again. **/
extern bool worldIsDirty;

/** X coord on the color picker triangle. This is a screen coord and not between 0.0 and 1.0. */
extern double colorTriangleX;

/** Y coord on the color picker triangle. This is a screen coord and not between 0.0 and 1.0. */
extern double colorTriangleY;

/** Pos on the hue bar of the color picker screen. This is a screen coord and not between 0.0 and 1.0. */
extern double colorBarPos;

/** Pos on the alpha bar of the color picker screen. This is a screen coord and not between 0.0 and 1.0. */
extern double colorAlphaPos;

/** A flag that is set to true when the color picker is actually used. Initially set to false to prevent the cube color from changing just from opening the color picking screen. */
extern bool changeCubeColor;

typedef long Clock;

/** Returns the current system time in milliseconds. Useful for timing operations. */
extern Clock currentTimeMs();
