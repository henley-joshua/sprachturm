#include "raylib.h"
#include "engine/physics/collision.h"
#include "engine/rendering/camera.h"
#include "utils/macros.h"
#include <stdio.h>

int startGameLoop(void) {

	BorderCollision collisions;
	int colorIndex = 0;
	float timer = 0.0f;
	float interval = 0.1f;
	int xVelocity = 1;
	int yVelocity = 1;
	int xCurrPosition = 190;
	int yCurrPosition = 200;
	int yRectCurrPosition = 201;
	int rectWidth = 160;
	int rectHeight = 17;
	Rectangle myRectangle = {xCurrPosition, yRectCurrPosition, rectWidth, rectHeight};
	Color colors[8] = {LIGHTGRAY, GOLD, ORANGE, GREEN, BROWN, BLACK, MAROON, DARKGRAY};

	while (!WindowShouldClose()) {
		timer += GetFrameTime();
		if (timer >= interval) {
			timer = 0.0f;
			colorIndex = (colorIndex + 1) % ARRAY_LEN(colors);
		}

		collisions = checkBorderCollision(myRectangle);
		if (collisions.left || collisions.right) xVelocity *= -1;
		if (collisions.top || collisions.bottom) yVelocity *= -1;

		// Update rectangle position
		myRectangle.x = xCurrPosition + xVelocity;
		myRectangle.y = yCurrPosition + yVelocity;

		// Update text position
		xCurrPosition = xCurrPosition + xVelocity;
		yCurrPosition = yCurrPosition + yVelocity;


		BeginDrawing();
			ClearBackground(BLACK);
			enterFreeCam();
			DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
