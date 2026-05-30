#include "raylib.h"
#include <stdio.h>
#include "game_state.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "game/player.h"
#include "game/heightmap.h"
#include "game/environment.h"

PhysicsConfig physicsConfig = {
	.dimensions = PHYSICS_3D,
	.gravity = { 0.0f, -3.5f, 0.0f },
	.enableGravity = true,
	.airDrag = 0.01,
	.groundFriction = 0.5f,
	.enableFriction = false,
	.substeps = 1,
	.fixedTimestep = 1.0f / 60.0f
};

GameState gameLoop(void) {
	GameState state;

	
	Player player;

	float timer = 0.0f;
	collisionInit();
	playerInit(&player);
	physicsInit(physicsConfig);
	staticCubesInit();

	double accumulator = 0.0;
	double previousTime = GetTime();

	while (!WindowShouldClose()) {
		double now = GetTime();
		double deltaTime = now - previousTime;
		previousTime = now;
		if (deltaTime > 0.25) deltaTime = 0.25;
		accumulator += deltaTime;

		checkToggleFullscreen();

		// Begin main game state
		//inputUpdate();   // To be added
		playerUpdate(&player, deltaTime);
		physicsUpdate(deltaTime);
		playerSyncCamera(&player);
		// sceneUpdate();    // To be added

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

	staticCubesUnload();
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

GameState gameBuilder(void) {
	GameState state;

	
	Player player;

	float timer = 0.0f;
	collisionInit();
	playerInit(&player);
	physicsInit(physicsConfig);
	staticCubesInit();

	double accumulator = 0.0;
	double previousTime = GetTime();

	while (!WindowShouldClose()) {
		double now = GetTime();
		double deltaTime = now - previousTime;
		previousTime = now;
		if (deltaTime > 0.25) deltaTime = 0.25;
		accumulator += deltaTime;

		checkToggleFullscreen();

		// Begin main game state
		//inputUpdate();   // To be added
		playerUpdate(&player, deltaTime);
		physicsUpdate(deltaTime);
		playerSyncCamera(&player);
		// sceneUpdate();    // To be added

		BeginDrawing();
			ClearBackground(GRAY);
			DrawFPS(10, 10);

			BeginMode3D(player.camera);
				drawEnvironment();
				drawPlayer(&player);
			EndMode3D();
		EndDrawing();
		// End main game state

	}

	staticCubesUnload();
	CloseWindow();
	return Game_Builder;

}

GameState gameRun(GameState state) {
	switch(state) {
		case Game_Start:
			gameStart();
		case Game_Menu:
			gameMenu();
		case Game_Loop:
			gameLoop();
		case Game_Pause:
			gamePause();
		default:
			printf("PANIC"); // Need to add a proper error here
		}
}