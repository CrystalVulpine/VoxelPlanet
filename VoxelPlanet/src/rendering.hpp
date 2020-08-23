#pragma once

#ifdef _WIN32
#include <windef.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include "World.hpp"
#include "global.hpp"
#include "mods.hpp"


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
extern void renderCube(int x, int y, int z, unsigned int cube);
extern void renderWorld();
extern void reRenderWorld();
extern void renderCubeSelect(double x, double y, double z);
extern int setupOpenGL();
extern void doDrawTick();
extern void cleanupOpenGL();
