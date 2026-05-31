#include "engine/rendering/renderer.h"
#include "state/game_state.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	
	bool game = false;
	bool builder = false;
	for(int i = 0; i < argc; i++){
		if(strcmp(argv[i], "g") == 0){
			game = true;
		}
		if(strcmp(argv[i], "b") == 0){
			builder = true;
		}
	}
	createWindow();
	GameState state = Game_Start;
	if(game == true){
		gameRun(state);
	} else if(builder == true) {
		gameBuilder();
	} else {
		gameRun(state);
	}

	
	return 0;
}
