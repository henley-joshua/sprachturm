#include <stdio.h>
#include "raylib.h"
#include "game_state.h"
#include "engine/physics/collision.h"
#include "engine/physics/physics.h"
#include "engine/rendering/renderer.h"
#include "engine/rendering/camera.h"
#include "game/init/game_init.h"
#include "game/player.h"
#include "game/heightmap.h"
#include "game/environment.h"
#include "game/free_cam.h"
#include "engine/input/input.h"
#include "engine/ui/button.h"
#include "engine/builder/builder.h"

GameState gameLoop(void) {
	GameState state;
	Player player;
	GamemodeState gameMode = PLAYER_MODE;

	gameInit(&player);
	float timer = 0.0f;

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
		updateEnvironment();
		inputUpdate();
		if (gameMode == PLAYER_MODE) {
			gameMode = playerUpdate(&player, deltaTime);
		}
		else if (gameMode == FREE_CAM_MODE) {
			gameMode = freeCamUpdate(&player, deltaTime);
		}
		physicsUpdate(deltaTime);
		if (gameMode == PLAYER_MODE) playerSyncCamera(&player);
		// sceneUpdate();    // To be added

		BeginDrawing();
			ClearBackground(GRAY);
			DrawFPS(10, 10);

			BeginMode3D(player.camera);
				drawEnvironment();
				if (gameMode == PLAYER_MODE) {
					drawPlayer(&player);
				}
			EndMode3D();
		EndDrawing();
		// End main game state

	}

	staticCubesUnload();
	dynamicCubesUnload();
	CloseWindow();
	return Game_Loop;
}




GameState gameMenu(void) {
	GameState state;
	bool isButton = false;
	Rectangle buttonDim = {40, 40, 300, 100};
	Vector2 pos = {40, 40}; 		// Part of fun
	Vector2 vel = {3, 2}; 		// Part of fun

	while(!WindowShouldClose() && !isButton) {
		BeginDrawing();
			ClearBackground(BLACK);
//============================================================================
// Purely for fun
			




			if(pos.x <= 0 || pos.x >= GetScreenWidth() - buttonDim.width){
				vel.x *= -1;
			}
			if(pos.y <= 0 || pos.y >= GetScreenHeight() - buttonDim.height){
				vel.y *= -1;
			}

			pos.x += vel.x;
			pos.y += vel.y;
			
			
			isButton = makeButton(buttonDim, WHITE, GOLD, "Josh Sucks", 30);
			
			buttonDim = (Rectangle){pos.x, pos.y, 300, 100};
//============================================================================

			

		EndDrawing();	}
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
			DrawText("Ronnie Stinks", 200, 200, 40, WHITE);
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
  Asset asset = builderInit("/home/usr/sprachturm/assets/textures/test.glb");

	gameInit(&player);
	float timer = 0.0f;

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
		inputUpdate();
		playerUpdate(&player, deltaTime);
		physicsUpdate(deltaTime);
		playerSyncCamera(&player);
		// sceneUpdate();    // To be added

		BeginDrawing();
			ClearBackground(GRAY);
			DrawFPS(10, 10);

			BeginMode3D(player.camera);
				drawEnvironment();
        
        runBuilder(&asset);
				drawPlayer(&player);
			EndMode3D();
		EndDrawing();
		// End main game state

	}

	//staticCubesUnload();
  
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
