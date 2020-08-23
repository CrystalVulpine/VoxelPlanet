# Mod Documentation

VoxelPlanet supports the ability to load mods into the game. Here you will find documentation on all the functions a mod can load into the game, and how to set one up. Currently mods are only available on Linux.

In order to make a mod, we must create a shared library that defines the functions. You may use as many or as few as you'd like. The mod must be compiled as a shared library that is a position-independent executable (`-fPIC` on gcc). All the mod functions need to be declared as `extern "C"` or they won't work. (You can call custom functions that are not declared as such however, and you can access global variables and functions from the main program in them). After compiling the mod, it should be placed in the "mods" folder which is located in VoxelPlanet's working directory. If your mod isn't working, make sure you compiled the actual game with the `-rdynamic` flag or whatever your compiler's equivalent to that is.

## Functions

Every function in the mod API is fully documented. This includes their arguments and details about when and how they execute. You will find this information listed below.

### Mod metadata

`const char* getApiVersion()` should return the API version the mod is designed for. This function is required, otherwise the mod won't work. The current version is 1.0.1.

`const char* getModVersion()` should return the version of the mod itself.

`const char* getModName()` should return the title of the mod.

`const char* getModDescription()` should return a short description or summary of the mod.

### Gameplay functions

`void onWorldTick()` runs once every time the world "ticks" (which is about 20 times per second).

`void onRenderTick()` runs at the end of the game's render loop, after the core features are rendered.

`void onGameStart()` runs when the game is started.

`void onGameExit()` runs when the game stops, ie the window is closed.

`void onWorldLoad()` runs when the world is initially loaded.

`void onWorldClose()` runs when the world exits, immediately before being saved.

`void onGameLoop(Clock loopTime, Clock lastLoopTime)` allows you to perform operations in the main game loop. `loopTime` refers to the start time of the current loop, in milliseconds. `lastLoopTime` is the start of the previous loop.

`void processGameArgs(int argc, char *argv[])` lets you act on command line arguments.

`void testFunc()` is a function that will run when VoxelPlanet starts, at the beginning of `main()`. It is meant to be used for testing the API and has no real purpose.

## Example

A simple example mod is shown below for your convenience. This mod and its project files are also available in the repository. There are also a few other example mods in addition to this one.

mod.cpp:
    
    #include <stdio.h>
    
    #ifdef __cplusplus
    extern "C" {
    #endif
    
    /** this function checks the version used for the mod against the version in the base game **/
    const char* getApiVersion() {
    	return "1.0.1";
    }
    
    const char* getModVersion() {
    	return "1.0.0";
    }
    
    const char* getModName() {
    	return "Test Mod";
    }
    
    void modTestFunc() {
    	printf("This mod works!\n");
    }
    
    #ifdef __cplusplus
    }
    #endif

This mod will print "This mod works!" when VoxelPlanet is started.
