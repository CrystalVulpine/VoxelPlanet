#pragma once

#include "World.hpp"
#include "Player.hpp"
#include "Settings.hpp"
#include "Screen.hpp"
#include "ColorScreen.hpp"

#define _PI 3.14159265358979323846

extern World mainWorld;
extern Player player;
extern Settings settings;

/** The screen that is currently open. 0 if none. **/
extern Screen * openedScreen;

/** Color picker GUI screen. */
extern ColorScreen colorScreen;

extern bool gamePaused;

/** the cube the screen/crosshair is pointing at **/
extern bool isCubeSelected;

typedef long Clock;

extern Clock currentTimeMs();
