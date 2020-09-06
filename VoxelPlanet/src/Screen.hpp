#pragma once

class Screen {
public:

	/** Performs the active screen operations. */
	virtual void screenTick(const double mouseX, const double mouseY) = 0;

	/** Render and draw the screen. */
	virtual void renderScreen() = 0;
};
