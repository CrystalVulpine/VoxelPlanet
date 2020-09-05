#pragma once

#include "global.hpp"

/** Current mod API version */
#define API_VERSION "1.0.1"

/** Loads mods from the dll files and their functions. */
extern void loadMods();

/** Runs each loaded mod's `testFunc()`. */
extern void mods_testFunc();

/** Runs each loaded mod's `onRenderTick()`. */
extern void mods_onRenderTick();

/** Runs each loaded mod's `onWorldTick()`. */
extern void mods_onWorldTick();

/** Runs each loaded mod's `onGameStart()`. */
extern void mods_onGameStart();

/** Runs each loaded mod's `onGameExit()`. */
extern void mods_onGameExit();

/** Runs each loaded mod's `onWorldLoad()`. */
extern void mods_onWorldLoad();

/** Runs each loaded mod's `onWorldClose()`. */
extern void mods_onWorldClose();

/** Runs each loaded mod's `onGameLoop()`. */
extern void mods_onGameLoop(Clock loopTime, Clock lastLoopTime);

/** Runs each loaded mod's `processGameArgs()`. */
extern void mods_processGameArgs(int argc, char *argv[]);
