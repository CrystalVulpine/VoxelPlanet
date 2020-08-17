# Mod Documentation

VoxelPlanet supports the ability to load mods into the game. Here you will find documentation on all the functions a mod can load into the game, and how to set one up. Currently there is only one function, and it is only available on Linux.

In order to make a mod, we must create a shared library that defines the functions. You may use as many or as few as you'd like. The mod must be compiled as a shared library that is a position-independent executable (`-fPIC` on gcc). All the mod functions need to be declared as `extern "C"` or they won't work. After compiling the mod, it should be placed in the mods folder which is located in VoxelPlanet's working directory.

## Functions

`const char* getApiVersion()` should return the API version the mod is designed for. This function is required, otherwise the mod won't work. The current version is "1.0.0".

`const char* getModVersion()` should return the version of the mod itself.

`const char* getModName()` should return the name of the mod.

`void modTestFunc()` is a function that will run when VoxelPlanet starts, at the beginning of `main()`. It is meant to be used for testing the API and has no real purpose.

## Example

A simple example mod is shown below for your convenience. This mod is also available in the repository.

    mod.cpp:
    
    #include <stdio.h>
    
    #ifdef __cplusplus
    extern "C" {
    #endif
    
    /** this function checks the version used for the mod against the version in the base game **/
    const char* getApiVersion() {
    	return "1.0.0";
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
