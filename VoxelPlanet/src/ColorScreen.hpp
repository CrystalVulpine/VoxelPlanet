#pragma once

#include "Screen.hpp"

class ColorScreen: public Screen {
public:
	/** X coord on the color picker triangle. This is a screen coord and not between 0.0 and 1.0. */
	double colorTriangleX;

	/** Y coord on the color picker triangle. This is a screen coord and not between 0.0 and 1.0. */
	double colorTriangleY;

	/** Pos on the hue bar of the color picker screen. This is a screen coord and not between 0.0 and 1.0. */
	double colorBarPos;

	/** Pos on the alpha bar of the color picker screen. This is a screen coord and not between 0.0 and 1.0. */
	double colorAlphaPos;

	/** A flag that is set to true when the color picker is actually used. Initially set to false to prevent the cube color from changing just from opening the color picking screen. */
	bool changeCubeColor;

	bool colorBarActive = false;
	bool colorAlphaBarActive = false;
	bool colorTriangleActive = false;

	/** Performs the active screen operations. */
	void screenTick(const double mouseX, const double mouseY);

	/** Render and draw the screen. */
	void renderScreen();

	ColorScreen();
};
