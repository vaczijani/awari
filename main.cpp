#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HOLES 6
#define MAX_HOLES (2*(HOLES)+2)
#define PLAYER_FIRST 0
#define PLAYER_LAST (PLAYER_FIRST+HOLES-1)
#define PLAYER_HOME (PLAYER_LAST+1)
#define CPU_FIRST (PLAYER_HOME+1)
#define CPU_LAST (CPU_FIRST+HOLES-1)
#define CPU_HOME (CPU_LAST+1)
#define INIT_PEBBLES 4
#define ALL_PEBBLES ((HOLES*2)*INIT_PEBBLES)

uint8_t holes[MAX_HOLES];


void print_table() {
	printf("%4s", "");
	for (int i = 0; i < HOLES; ++i)
		printf("%4d", holes[CPU_LAST-i]);
	printf("\n");
	printf("%4d", holes[CPU_HOME]);
	for (int i = 0; i < HOLES; ++i)
		printf("%4s", "");
	printf("%4d", holes[PLAYER_HOME]);
	printf("\n");
	printf("%4s", "");
	for (int i = 0; i < HOLES; ++i)
		printf("%4d", holes[PLAYER_FIRST+i]);	
	printf("\n");
}


void init_table() {
	for (int i = 0; i < HOLES; ++i) {
		holes[PLAYER_FIRST+i] = INIT_PEBBLES;
		holes[CPU_FIRST+i] = INIT_PEBBLES;
	}
}

bool is_hole_empty(int hole) { return holes[hole] == 0; }

bool is_my_side(int hole, int first, int last) { return hole >= first && hole <= last; }

int my_home(int last) { return last + 1; }

int opponent_home(int last) { return (my_home(last) + HOLES + 1) % MAX_HOLES; }

bool is_valid_move(int hole, int first, int last) { return is_my_side(hole, first, last) && !is_hole_empty(hole); }

bool is_side_empty(int first, int last) {
	for (int i = first; i <= last; ++i)
		if (!is_hole_empty(i)) return false;
	return true;
}

int opponent_hole(int hole, int last) { return ((last + HOLES + 1) - hole) % MAX_HOLES; }

bool is_game_ended() { return is_side_empty(PLAYER_FIRST, PLAYER_LAST) || is_side_empty(CPU_FIRST, CPU_LAST); }

int get_player_move() {
	int move;
	scanf("%d", &move);
	return move-1;
}

bool make_move(int move, int first, int last) {
	if (!is_valid_move(move, first, last)) return false;

	int taken = holes[move];
	holes[move] = 0;
	int next_hole = move;
	while (taken > 0) {
		do {
			next_hole = (next_hole + 1) % MAX_HOLES;
		} while(next_hole == opponent_home(last));
		++holes[next_hole];
		--taken;
	}
	if (holes[next_hole] == 1 && is_my_side(next_hole, first, last)) {
		holes[my_home(last)] += holes[next_hole] + holes[opponent_hole(next_hole, last)];
		holes[next_hole] = 0;
		holes[opponent_hole(next_hole, last)] = 0;
	}
	if (next_hole == my_home(last)) return true;
	return false;
}

int main() {
	init_table();
	while (!is_game_ended()) {
		bool repeat = false;
		int move;
		bool valid_move = false;
		// player move
		do {
			print_table();
			do {
				printf("your move (1-6):");
				move = get_player_move();
				valid_move = is_valid_move(move, PLAYER_FIRST, PLAYER_LAST);
				if (!valid_move) printf("invalid move!\n");
			} while (!valid_move);
			repeat = make_move(move, PLAYER_FIRST, PLAYER_LAST);
			if (repeat) printf("your turn again!\n");
		} while (repeat && !is_game_ended());
		// cpu move
		if (is_game_ended()) break;
		repeat = false;
		do {
			print_table();
			printf("my move:");
			do {
				move = (rand() % HOLES) + CPU_FIRST;
				valid_move = is_valid_move(move, CPU_FIRST, CPU_LAST);
			} while (!valid_move);
			printf("%d\n", move);
			repeat = make_move(move, CPU_FIRST, CPU_LAST);
			if (repeat) printf("my turn again!\n");
		} while (repeat && !is_game_ended());
	}
	print_table();
	if (holes[PLAYER_HOME] > holes[CPU_HOME])
		printf("You won\n");
	else if (holes[PLAYER_HOME] < holes[CPU_HOME])
		printf("You lost\n");
	else
		printf("Draw\n");
}
