#pragma once


extern void mods_testFunc();
extern void mods_onRenderTick();
extern void mods_onWorldTick();
extern void mods_onGameStart();
extern void mods_onGameExit();
extern void mods_onWorldLoad();
extern void mods_onWorldClose();
extern void mods_onGameLoop(Clock loopTime, Clock lastLoopTime);
extern void mods_processGameArgs(int argc, char *argv[]);

#ifdef __linux__


#define API_VERSION "1.0.1"

extern void loadModsLinux();
#define loadMods() loadModsLinux()


#else


#define loadMods()


#endif
