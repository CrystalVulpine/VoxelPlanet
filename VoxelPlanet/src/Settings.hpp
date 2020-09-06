#pragma once

class Settings {
public:

	/** This enables debugging stuff, such as not capturing the pointer (in case the program freezes). */
	bool debugMode;

	bool hideGUI;
	bool fancyGraphics;

	/** How many 16-cube areas are visible in any given direction. */
	unsigned int renderDistance;

	/** Width of the game window in pixels. */
	int windowWidth;

	/** Height of the game window in pixels. */
	int windowHeight;

	/** The level of antialiasing/MSAA sampling. */
	unsigned char antialiasingLevel;

	/** The brightness of cubes and objects in the world. */
	float worldBrightness;

	float skyColorRed;
	float skyColorGreen;
	float skyColorBlue;

	Settings();
};
