#include "engine/rendering/renderer.h"
#include "state/game_state.h"
#include <stdio.h>

int main(void) {
	createWindow();
	GameState state = Game_Start;
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
