#include "Settings.hpp"

Settings::Settings() __restrict {
	debugMode = false;
	hideGUI = false;
	fancyGraphics = true;
	renderDistance = 16;

	windowWidth = 1024;
	windowHeight = 768;
	antialiasingLevel = 4;

	worldBrightness = 1.0f;
	skyColorRed = 0.5f;
	skyColorGreen = 0.5f;
	skyColorBlue = 1.0f;
}
