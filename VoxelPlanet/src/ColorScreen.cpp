#include <glm/gtc/matrix_transform.hpp>
#include "global.hpp"
#include "rendering.hpp"
#include "ColorScreen.hpp"

ColorScreen::ColorScreen() __restrict {
	colorTriangleX = 0.0;
	colorTriangleY = 0.8;
	colorBarPos = 0.0;
	colorAlphaPos = 0.8;
}

void ColorScreen::screenTick(const double mouseX, const double mouseY) __restrict {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		changeCubeColor = true;

		glfwGetCursorPos(window, (double*)&mouseX, (double*)&mouseY);

		double mouseX2 = (mouseX / (double)settings.windowHeight) * 2.0 - ((double)settings.windowWidth / (double)settings.windowHeight);
		double mouseY2 = -(mouseY / (double)settings.windowHeight) * 2.0 + 1.0;

		double maxX = (1.0 - ((mouseY2 + 0.1) / 0.9)) * 0.6;
		if (colorTriangleActive || ((!colorBarActive && !colorAlphaBarActive) && mouseY2 <= 0.8 && mouseY2 >= -0.1 && std::abs(mouseX2) < maxX)) {
			colorTriangleActive = true;

			if (mouseY2 > 0.8) {
				mouseY2 = 0.8;
			} else if (mouseY2 < -0.1) {
				mouseY2 = -0.1;
			}

			// we have to do this again since the y pos could be changed
			maxX = (1.0 - ((mouseY2 + 0.1) / 0.9)) * 0.6;

			if (mouseX2 < -maxX) {
				mouseX2 = -maxX;
			} else if (mouseX2 > maxX) {
				mouseX2 = maxX;
			}

			colorTriangleX = mouseX2;
			colorTriangleY = mouseY2;
		} else if (colorBarActive || ((!colorTriangleActive && !colorAlphaBarActive) && mouseY2 >= -0.35 && mouseY2 <= -0.15 && std::abs(mouseX2) < 0.6)) {
			changeCubeColor = true;
			colorBarActive = true;

			if (mouseX2 < -0.6) {
				mouseX2 = -0.6;
			} else if (mouseX2 > 0.6) {
				mouseX2 = 0.6;
			}

			colorBarPos = mouseX2;
		} else if (colorAlphaBarActive || ((!colorTriangleActive && !colorBarActive) && mouseY2 <= 0.8 && mouseY2 >= -0.1 && mouseX2 >= -0.8 && mouseX2 <= -0.7)) {
			changeCubeColor = true;
			colorAlphaBarActive = true;

			if (mouseY2 > 0.8) {
				mouseY2 = 0.8;
			} else if (mouseY2 < -0.1) {
				mouseY2 = -0.1;
			}

			colorAlphaPos = mouseY2;
		}
	} else {
		colorTriangleActive = false;
		colorBarActive = false;
		colorAlphaBarActive = false;
	}
}

void ColorScreen::renderScreen() __restrict {
	double vertices[] = {
			0.6, -0.1, 0.0,
			0.0, 0.8, 0.0,
			-0.6, -0.1, 0.0,

			-0.4, -0.2, 0.0,
			-0.6, -0.2, 0.0,
			-0.6, -0.3, 0.0,

			-0.6, -0.3, 0.0,
			-0.4, -0.3, 0.0,
			-0.4, -0.2, 0.0,

			-0.2, -0.2, 0.0,
			-0.4, -0.2, 0.0,
			-0.4, -0.3, 0.0,
			-0.4, -0.3, 0.0,
			-0.2, -0.3, 0.0,
			-0.2, -0.2, 0.0,

			0.0, -0.2, 0.0,
			-0.2, -0.2, 0.0,
			-0.2, -0.3, 0.0,
			-0.2, -0.3, 0.0,
			0.0, -0.3, 0.0,
			0.0, -0.2, 0.0,

			0.2, -0.2, 0.0,
			0.0, -0.2, 0.0,
			0.0, -0.3, 0.0,
			0.0, -0.3, 0.0,
			0.2, -0.3, 0.0,
			0.2, -0.2, 0.0,

			0.4, -0.2, 0.0,
			0.2, -0.2, 0.0,
			0.2, -0.3, 0.0,
			0.2, -0.3, 0.0,
			0.4, -0.3, 0.0,
			0.4, -0.2, 0.0,

			0.6, -0.2, 0.0,
			0.4, -0.2, 0.0,
			0.4, -0.3, 0.0,
			0.4, -0.3, 0.0,
			0.6, -0.3, 0.0,
			0.6, -0.2, 0.0,

			colorTriangleX - 0.015, colorTriangleY + 0.015, 0.0,
			colorTriangleX - 0.015, colorTriangleY - 0.015, 0.0,
			colorTriangleX + 0.015, colorTriangleY - 0.015, 0.0,
			colorTriangleX + 0.015, colorTriangleY - 0.015, 0.0,
			colorTriangleX + 0.015, colorTriangleY + 0.015, 0.0,
			colorTriangleX - 0.015, colorTriangleY + 0.015, 0.0,

			colorTriangleX - 0.01, colorTriangleY + 0.01, 0.0,
			colorTriangleX - 0.01, colorTriangleY - 0.01, 0.0,
			colorTriangleX + 0.01, colorTriangleY - 0.01, 0.0,
			colorTriangleX + 0.01, colorTriangleY - 0.01, 0.0,
			colorTriangleX + 0.01, colorTriangleY + 0.01, 0.0,
			colorTriangleX - 0.01, colorTriangleY + 0.01, 0.0,

			colorBarPos - 0.01, -0.15, 0.0,
			colorBarPos - 0.01, -0.35, 0.0,
			colorBarPos + 0.01, -0.35, 0.0,
			colorBarPos + 0.01, -0.35, 0.0,
			colorBarPos + 0.01, -0.15, 0.0,
			colorBarPos - 0.01, -0.15, 0.0,

			-0.7, 0.8, 0.0,
			-0.8, 0.8, 0.0,
			-0.8, -0.1, 0.0,
			-0.8, -0.1, 0.0,
			-0.7, -0.1, 0.0,
			-0.7, 0.8, 0.0,

			-0.85, colorAlphaPos + 0.01, 0.0,
			-0.85, colorAlphaPos - 0.01, 0.0,
			-0.65, colorAlphaPos - 0.01, 0.0,
			-0.65, colorAlphaPos - 0.01, 0.0,
			-0.65, colorAlphaPos + 0.01, 0.0,
			-0.85, colorAlphaPos + 0.01, 0.0,
	};

	float colorBarRed;
	float colorBarGreen;
	float colorBarBlue;

	if (colorBarPos > 0.0) {
		colorBarRed = (colorBarPos - 0.2f) / 0.2f;
	} else {
		colorBarRed = (-colorBarPos - 0.2f) / 0.2f;
	}
	if (colorBarPos > -0.2) {
		colorBarGreen = -(colorBarPos - 0.2f) / 0.2f;
	} else {
		colorBarGreen = 1.0f + (colorBarPos + 0.4f) / 0.2f;
	}
	if (colorBarPos > 0.2) {
		colorBarBlue = -(colorBarPos - 0.6f) / 0.2f;
	} else {
		colorBarBlue = 1.0f + (colorBarPos + 0.0) / 0.2f;
	}

	if (colorBarRed < 0.0f) {
		colorBarRed = 0.0f;
	} else if (colorBarRed > 1.0f) {
		colorBarRed = 1.0f;
	}
	if (colorBarGreen < 0.0f) {
		colorBarGreen = 0.0f;
	} else if (colorBarGreen > 1.0f) {
		colorBarGreen = 1.0f;
	}
	if (colorBarBlue < 0.0f) {
		colorBarBlue = 0.0f;
	} else if (colorBarBlue > 1.0f) {
		colorBarBlue = 1.0f;
	}

	float multiplierRed = ((colorTriangleY + 0.1) / 0.9) * colorBarRed;
	multiplierRed += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
	multiplierRed += (colorTriangleX / 1.2);
	if (multiplierRed > 1.0f) {
		multiplierRed = 1.0f;
	} else if (multiplierRed < 0.0f) {
		multiplierRed = 0.0f;
	}

	float multiplierGreen = ((colorTriangleY + 0.1) / 0.9) * colorBarGreen;
	multiplierGreen += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
	multiplierGreen += (colorTriangleX / 1.2);
	if (multiplierGreen > 1.0f) {
		multiplierGreen = 1.0f;
	} else if (multiplierGreen < 0.0f) {
		multiplierGreen = 0.0f;
	}

	float multiplierBlue = ((colorTriangleY + 0.1) / 0.9) * colorBarBlue;
	multiplierBlue += 0.5f * (1.0f - ((colorTriangleY + 0.1) / 0.9));
	multiplierBlue += (colorTriangleX / 1.2);
	if (multiplierBlue > 1.0f) {
		multiplierBlue = 1.0f;
	} else if (multiplierBlue < 0.0f) {
		multiplierBlue = 0.0f;
	}

	float colors[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			colorBarRed, colorBarGreen, colorBarBlue, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,

			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,

			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,

			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,

			1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,

			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,

			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,

			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,

			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,

			multiplierRed, multiplierGreen, multiplierBlue, 1.0f,
			multiplierRed, multiplierGreen, multiplierBlue, 1.0f,
			multiplierRed, multiplierGreen, multiplierBlue, 0.0f,
			multiplierRed, multiplierGreen, multiplierBlue, 0.0f,
			multiplierRed, multiplierGreen, multiplierBlue, 0.0f,
			multiplierRed, multiplierGreen, multiplierBlue, 1.0f,

			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
	};
	renderToGUI(sizeof(vertices) / sizeof(vertices[0]) / 3, vertices, colors);

	unsigned int usingCube;
	if (changeCubeColor) {
		usingCube = (unsigned char)(multiplierRed * 255.0f) << 24 | (unsigned char)(multiplierGreen * 255.0f) << 16 | (unsigned char)(multiplierBlue * 255.0f) << 8 | (unsigned char)(((colorAlphaPos + 0.1f) / 0.9f) * 255.0f);
		player.inventory[player.selectedSlot] = usingCube;
	} else {
		usingCube = player.inventory[player.selectedSlot];
	}

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = glm::ortho<float>(-((float)settings.windowWidth / (float)settings.windowHeight), (float)settings.windowWidth / (float)settings.windowHeight, -1.0f, 1.0f, -1.0f, 1.0f) * glm::rotate(model, 0.5f, glm::vec3(1.0, 0.0, 0.0)) * glm::translate(model, glm::vec3(((double)settings.windowWidth / (double)settings.windowHeight) - 0.2, 0.9, 0.0)) * glm::rotate(model, 0.785398f, glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(GLmatrix, 1, GL_FALSE, &mvp[0][0]);

	float red = (float)(usingCube >> 24) / 255.0f;
	float green = (float)((usingCube >> 16) & 255) / 255.0f;
	float blue = (float)((usingCube >> 8) & 255) / 255.0f;
	float alpha = (float)(usingCube & 255) / 255.0f;
	float usingCubeColor[] = {
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,
			red * 0.8f, green * 0.8f, blue * 0.8f, alpha,

			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,
			red * 0.6f, green * 0.6f, blue * 0.6f, alpha,

			red, green, blue, alpha,
			red, green, blue, alpha,
			red, green, blue, alpha,
			red, green, blue, alpha,
			red, green, blue, alpha,
			red, green, blue, alpha,
	};
	double usingCubeVertices[] = {
			-0.1, 0.1, 0.1,
			-0.1, -0.1, 0.1,
			0.1, -0.1, 0.1,
			-0.1, 0.1, 0.1,
			0.1, -0.1, 0.1,
			0.1, 0.1, 0.1,

			-0.1, -0.1, 0.1,
			-0.1, 0.1, 0.1,
			-0.1, 0.1, -0.1,
			-0.1, -0.1, 0.1,
			-0.1, 0.1, -0.1,
			-0.1, -0.1, -0.1,

			0.1, 0.1, 0.1,
			0.1, 0.1, -0.1,
			-0.1, 0.1, -0.1,
			-0.1, 0.1, -0.1,
			-0.1, 0.1, 0.1,
			0.1, 0.1, 0.1,
	};
	renderToGUI(18, usingCubeVertices, usingCubeColor);
}
