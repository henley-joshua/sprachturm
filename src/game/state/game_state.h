#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
	Game_Loop,
	Game_Menu,
	Game_Start,
	Game_Pause
} GameState;

GameState gameLoop(void);
GameState gameMenu(void);
GameState gameStart(void);
GameState gamePause(void);

#endif