#include "Grid.h"
#include "Cell.h"

int main(void) {
	InitWindow(1000, 1000, "Tic Tac Toe");
	SetTargetFPS(40);

	using Grid = GridManager<Square, 3, 3>;	
	Grid::xo = LoadTexture("this.png");
	Grid::setup(200, {25.0f, 25.0f});

	for (;;) {
		if (WindowShouldClose()) break;
		BeginDrawing();
		ClearBackground(SKYBLUE);

		Grid::update();
		Grid::draw(Grid::currentPlayer);
		EndDrawing();
	}
}

int WinMain(void) {
	main();
}