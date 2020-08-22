#pragma once

#include "global.hpp"


extern void mods_testFunc();
extern void mods_onRenderTick();
extern void mods_onWorldTick();
extern void mods_onGameStart();
extern void mods_onGameExit();
extern void mods_onWorldLoad();
extern void mods_onWorldClose();
extern void mods_onGameLoop(Clock loopTime, Clock lastLoopTime);
extern void mods_processGameArgs(int argc, char *argv[]);

#define API_VERSION "1.0.1"

#if defined(__linux__) || defined(__APPLE__)

void loadModsUnix();
#define loadMods() loadModsUnix()

#else

#define loadMods()

#endif
