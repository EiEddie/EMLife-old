#include"hEMLife.h"
#undef main

int main() {
	GameInput input("setting.json");
	input.SetMazeInf();
	
	Game game(60);
	game.EventLoop();
	return 0;
}
