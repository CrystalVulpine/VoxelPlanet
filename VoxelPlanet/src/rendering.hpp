#pragma once

#if defined(_WIN32)
#include <windef.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern GLFWwindow * __restrict window;

extern GLuint GLprogram;
extern GLuint GLmatrix;

extern bool worldIsDirty;

extern void takeScreenshot(const char * const __restrict filename, const char * const __restrict folder);
extern void renderWorld();
extern void reRenderWorld();
extern void renderCubeSelect(const double x, const double y, const double z);
extern int setupOpenGL();
extern void doDrawTick();
extern void cleanupOpenGL();

/** Loads the shaders from the specified (relative) file paths. */
extern GLuint loadShaders(const GLchar * const __restrict vertexShaderPath, const GLchar * const __restrict fragmentShaderPath);

/** adds and draws a list to the gui vbo. `count` refers to the number of vertices. **/
extern void renderToGUI(unsigned int count, const double * const __restrict vertices, const float * const __restrict colors);
