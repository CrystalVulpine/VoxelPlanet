#pragma once

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <dlfcn.h>
#include <vector>
#include <functional>
#include "global.hpp"

namespace fs = std::experimental::filesystem;

std::vector<void (*)()> mod_testFunc;
std::vector<void (*)()> mod_onRenderTick;
std::vector<void (*)()> mod_onWorldTick;
std::vector<void (*)()> mod_onGameStart;
std::vector<void (*)()> mod_onGameExit;
std::vector<void (*)(Clock, Clock)> mod_onGameLoop;
std::vector<void (*)(int, char*[])> mod_processGameArgs;

void mods_testFunc() {
	for (unsigned int i = 0; i < mod_testFunc.size(); ++i) {
		mod_testFunc.at(i)();
	}
}

void mods_onRenderTick() {
	for (unsigned int i = 0; i < mod_onRenderTick.size(); ++i) {
		mod_onRenderTick.at(i)();
	}
}

void mods_onWorldTick() {
	for (unsigned int i = 0; i < mod_onWorldTick.size(); ++i) {
		mod_onWorldTick.at(i)();
	}
}

void mods_onGameStart() {
	for (unsigned int i = 0; i < mod_onGameStart.size(); ++i) {
		mod_onGameStart.at(i)();
	}
}

void mods_onGameExit() {
	for (unsigned int i = 0; i < mod_onGameExit.size(); ++i) {
		mod_onGameExit.at(i)();
	}
}

void mods_onGameLoop(Clock loopTime, Clock lastLoopTime) {
	for (unsigned int i = 0; i < mod_onGameLoop.size(); ++i) {
		mod_onGameLoop.at(i)(loopTime, lastLoopTime);
	}
}

void mods_processGameArgs(int argc, char *argv[]) {
	for (unsigned int i = 0; i < mod_processGameArgs.size(); ++i) {
		mod_processGameArgs.at(i)(argc, argv);
	}
}

#ifdef __linux__

#define API_VERSION "1.0.0"

void loadModsLinux()
{
    std::string path("mods/");

    // there are no mods, so don't try to load any
    if (!fs::is_directory(path)) {
    	return;
    }

	mod_testFunc.reserve(5);
	mod_onRenderTick.reserve(5);
	mod_onWorldTick.reserve(5);
	mod_onGameStart.reserve(5);
	mod_onGameExit.reserve(5);
	mod_processGameArgs.reserve(5);
	mod_onGameLoop.reserve(5);

    for(auto& p: fs::recursive_directory_iterator(path))
    {
        if(strcmp(p.path().extension().c_str(), ".so") == 0) {

			void* handle;

			if ((handle = dlopen(p.path().string().c_str(), RTLD_LAZY)) != NULL) {

				const char* (*versionGetter)() = (const char* (*)())dlsym(handle, "getApiVersion");

				auto error = dlerror();
				if (error != NULL) {
					std::cout << error << '\n';
					continue;
				} else {
					const char* modApiVersion = versionGetter();
					if (strcmp(modApiVersion, API_VERSION) != 0) {
						std::cout << "Could not load mod " << p << ". Wrong mod API version: Mod uses " << modApiVersion << " but this is " << API_VERSION << ".\n";
						continue;
					}
				}

				void (*testFunc)() = (void (*)())dlsym(handle, "testFunc");

				// skip if the function isn't used by the mod
				if (dlerror() == NULL) {
					mod_testFunc.push_back(testFunc);
				}

				void (*onRenderTick)() = (void (*)())dlsym(handle, "onRenderTick");

				if (dlerror() == NULL) {
					mod_onRenderTick.push_back(onRenderTick);
				}

				void (*onWorldTick)() = (void (*)())dlsym(handle, "onWorldTick");

				if (dlerror() == NULL) {
					mod_onWorldTick.push_back(onWorldTick);
				}

				void (*onGameStart)() = (void (*)())dlsym(handle, "onGameStart");

				if (dlerror() == NULL) {
					mod_onGameStart.push_back(onGameStart);
				}

				void (*onGameExit)() = (void (*)())dlsym(handle, "onGameExit");

				if (dlerror() == NULL) {
					mod_onGameExit.push_back(onGameExit);
				}

				void (*onGameLoop)(Clock, Clock) = (void (*)(Clock, Clock))dlsym(handle, "onGameLoop");

				if (dlerror() == NULL) {
					mod_onGameLoop.push_back(onGameLoop);
				}

				void (*processGameArgs)(int, char*[]) = (void (*)(int, char*[]))dlsym(handle, "processGameArgs");

				if (dlerror() == NULL) {
					mod_processGameArgs.push_back(processGameArgs);
				}

			} else {

				std::cout << "Unable to load mod \"" << p.path().string() << "\"\n";
			}
        }
    }
}

#define loadMods() loadModsLinux()

#else

#define loadMods()

#endif
