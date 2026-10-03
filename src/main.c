#include "raylib.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define u8  uint8_t
#define i64 int64_t

int quitGame = 0;
int windowWidth = 1200;
int windowHeight = 900;

typedef enum {
	EMPTY = 0,
	PLAYER1,
	PLAYER2,
} Cell;

typedef enum {
	PLACING,
	MOVING,
	WON,
} GamePeriod;

/*
 * The Gameboard cells are indexed in the following way:
 * 0 1 2
 * 7 8 3
 * 6 5 4
 *
 * Player cells use the above indexing.
 */
typedef struct {
	Cell playerTurn;
	Cell cells[9];
	u8 player1Cells[3];
	u8 player2Cells[3];
	GamePeriod period;
} GameState;

GameState
initGameState(void) {
	return (GameState){
	    .playerTurn = PLAYER1,
	    .cells = {0},
	    .player1Cells = {0},
	    .player2Cells = {0},
	    .period = PLACING,
	};
}

u8
gridToSpiral(u8 index) {
	switch (index) {
	case 1:  return 0;
	case 2:  return 1;
	case 3:  return 2;
	case 4:  return 7;
	case 5:  return 8;
	case 6:  return 3;
	case 7:  return 6;
	case 8:  return 5;
	case 9:  return 4;
	default: return 0;
	}
}

char*
i64ToString(i64 number) {
	char* ptr = malloc(30);
	snprintf(ptr, 29, "%ld", number);
	return ptr;
}

static inline bool
checkLineOutside(GameState* state, u8 i) {
	if (state->cells[i] != state->playerTurn) return false;
	if (state->cells[(i + 1) % 8] != state->playerTurn) return false;
	if (state->cells[(i + 2) % 8] != state->playerTurn) return false;
	return true;
}

static inline bool
checkLineInside(GameState* state, u8 i) {
	if (state->cells[i] != state->playerTurn) return false;
	if (state->cells[(i + 4) % 8] != state->playerTurn) return false;
	if (state->cells[8] != state->playerTurn) return false;
	return true;
}

bool
checkWin(GameState* state) {
	for (int i = 0; i < 8; i++) {
		if (checkLineOutside(state, i)) return true;
	}
	for (int i = 0; i < 4; i++) {
		if (checkLineInside(state, i)) return true;
	}
	return false;
}

void
displayBoard(GameState* state) {
	int offset = 300;
	int centerX = windowWidth / 2 - offset;
	int centerY = windowHeight / 2 - offset;
	int cellSize = 25;

	// Display Grid Lines
	DrawRectangleLines(centerX, centerY, offset * 2, offset * 2, GRAY);
	DrawLine(centerX, centerY, centerX + offset * 2, centerY + offset * 2,
	         GRAY);
	DrawLine(centerX + offset, centerY, centerX + offset,
	         centerY + offset * 2, GRAY);
	DrawLine(centerX + 2 * offset, centerY, centerX, centerY + offset * 2,
	         GRAY);
	DrawLine(centerX, centerY + offset, centerX + offset * 2,
	         centerY + offset, GRAY);

	// Display Cells
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int x = centerX + offset * j;
			int y = centerY + offset * i;
			Color color = GRAY;
			if (state->cells[gridToSpiral(i * 3 + j + 1)] ==
			    PLAYER1)
				color = BLUE;
			if (state->cells[gridToSpiral(i * 3 + j + 1)] ==
			    PLAYER2)
				color = RED;
			DrawCircle(x, y, cellSize, color);

			DrawText(i64ToString(i * 3 + j + 1), x - 6, y - 12, 24,
			         WHITE);
		}
	}
}

void
SwapPlayer(GameState* state) {
	if (state->playerTurn == PLAYER1)
		state->playerTurn = PLAYER2;
	else
		state->playerTurn = PLAYER1;
}

void
HandleInput(GameState* state) {
	int key;
	do {
		key = GetKeyPressed();
		switch (key) {
		case KEY_ONE:
			state->cells[0] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_TWO:
			state->cells[1] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_THREE:
			state->cells[2] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_FOUR:
			state->cells[7] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_FIVE:
			state->cells[8] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_SIX:
			state->cells[3] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_SEVEN:
			state->cells[6] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_EIGHT:
			state->cells[5] = state->playerTurn;
			SwapPlayer(state);
			return;
		case KEY_NINE:
			state->cells[4] = state->playerTurn;
			SwapPlayer(state);
			return;
		}
	} while (key != 0);
}

int
main(void) {
	InitWindow(windowWidth, windowHeight, "GREEK");
	GameState state = initGameState();

	while (!WindowShouldClose() && !quitGame) {
		BeginDrawing();
		ClearBackground(BLACK);

		displayBoard(&state);
		HandleInput(&state);

		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
