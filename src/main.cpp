#include <EMLife/EMLife.h>
#undef main

int main() {
	GameInput input("assets/setting.json");
	input.SetMazeInf();
	
	Game game(60);
	game.EventLoop();
	return 0;
}
