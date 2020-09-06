/**
 * @file rendering.hpp
 *
 * This header contains most of the functions and variables for rendering and drawing the game with OpenGL.
 */

#pragma once

#if defined(_WIN32)
#include <windef.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/** The main OpenGL window of the game. */
extern GLFWwindow * __restrict window;

extern GLuint GLprogram;
extern GLuint GLmatrix;

/** Whether or not the world has been altered and needs re-rendered. This should be set to true when placing, deleting, or editing cubes. */
extern bool worldIsDirty;

/** Takes a screenshot with the specified \p filename (ex. "sh.png") in the specified \p folder (relative path). */
extern void takeScreenshot(const char * const __restrict filename, const char * const __restrict folder);

extern void renderWorld();

/** Resets rendering info and renders the world again. */
extern void reRenderWorld();

/** Renders the black lines around the pointed-at cube. */
extern void renderCubeSelect(const double x, const double y, const double z);

/** Initializes OpenGL, GLEW, and GLFW. */
extern int setupOpenGL();

/** Renders and draws a new frame. */
extern void doDrawTick();

/** Shuts down OpenGL and any graphical libraries. */
extern void cleanupOpenGL();

/** Loads the shaders from the specified (relative) file paths. */
extern GLuint loadShaders(const GLchar * const __restrict vertexShaderPath, const GLchar * const __restrict fragmentShaderPath);

/** Adds and draws a list to the gui vbo. `count` refers to the number of vertices. **/
extern void renderToGUI(unsigned int count, const double * const __restrict vertices, const float * const __restrict colors);
