#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
	Game_Loop,
	Game_Menu,
	Game_Start,
	Game_Pause,
	Game_Builder
} GameState;

GameState gameLoop(void);
GameState gameMenu(void);
GameState gameStart(void);
GameState gamePause(void);
GameState gameBuilder(void);
GameState gameRun(GameState state);

#endif