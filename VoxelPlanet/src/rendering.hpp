#pragma once

#if defined(_WIN32)
#include <windef.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern GLFWwindow * __restrict window;
extern int windowWidth;
extern int windowHeight;

extern unsigned char antialiasingLevel;
extern float worldBrightness;
extern float skyColorRed;
extern float skyColorGreen;
extern float skyColorBlue;

extern bool worldIsDirty;

extern void takeScreenshot(const char * const __restrict filename, const char * const __restrict folder);
extern void renderWorld();
extern void reRenderWorld();
extern void renderCubeSelect(const double x, const double y, const double z);
extern int setupOpenGL();
extern void doDrawTick();
extern void cleanupOpenGL();
