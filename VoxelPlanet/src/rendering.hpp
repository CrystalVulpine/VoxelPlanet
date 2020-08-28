#pragma once

#include "Camera.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <windef.h>
#endif

extern GLFWwindow* window;
extern int windowWidth;
extern int windowHeight;

extern unsigned char antialiasingLevel;
extern float worldBrightness;
extern float skyColorRed;
extern float skyColorGreen;
extern float skyColorBlue;

extern Camera camera;

extern bool worldIsDirty;

extern GLuint loadShaders(GLchar const * vertexShaderCode, GLchar const * fragmentShaderCode);
extern void takeScreenshot(char filename[], char folder[]);
extern void renderCube(int x, int y, int z, unsigned int cube);
extern void renderWorld();
extern void reRenderWorld();
extern void renderCubeSelect(double x, double y, double z);
extern int setupOpenGL();
extern void doDrawTick();
extern void cleanupOpenGL();
