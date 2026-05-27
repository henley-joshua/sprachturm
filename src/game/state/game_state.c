#include "raylib.h"
#include "game_state.h"
#include "engine/physics/collision.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "game/player.h"
#include "engine/scene/scene.h"

GameState gameLoop(void) {
	GameState state;

	Player player;

	float timer = 0.0f;
	playerInit(&player);

	while (!WindowShouldClose()) {
		timer += GetFrameTime();
		checkToggleFullscreen();

		// Begin main game state
		updatePlayerMovement(&player);
		BeginDrawing();
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(player.camera);
				drawEnvironment();
				drawPlayer(&player);
			EndMode3D();
		EndDrawing();
		// End main game state

	}

	CloseWindow();
	return Game_Loop;
}




GameState gameMenu(void) {
	GameState state;
	bool isButton = false;
	Rectangle button = {190, 250, 300, 100};
	while(!WindowShouldClose() && !isButton) {
		BeginDrawing();
			ClearBackground(BLACK);
			DrawRectangle(button.x, button.y, button.width, button.height, WHITE);
			DrawText("Press Start", 200, 300, 40, GOLD);
		EndDrawing();
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(GetMousePosition(), button) && 
		   IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			isButton = true;
		}
	}
	return Game_Loop;
} 

GameState gameStart(void) {
	GameState state;
	float duration = 5.0f;
	float elapsed = 0.0f;
	float load = 0.0f;

	while(!WindowShouldClose() && elapsed < duration) {
		float dt = GetFrameTime();
		elapsed += dt;
		if(load < 240) {
			load += 1;
		}
		BeginDrawing();
			ClearBackground(GOLD);
			DrawText("Josh Stinks", 200, 200, 40, WHITE);
			DrawRectangle(200, 250, load, 30, WHITE);
		EndDrawing();
	}
	return Game_Menu;
}

GameState gamePause(void) {
	GameState state;

	return state;
}