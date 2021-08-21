#include"hEMLife.h"
#undef main

int main(int argc, char* argv[]) {
	Game game(60);
	game.EventLoop();
	return 0;
}
