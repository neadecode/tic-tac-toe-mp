#include "Grid.h"

template<typename S, int _width, int _height>
void GridManager<S, _width, _height>::setup(int sqrSize, Vector2 _padding) {
	squareSize = sqrSize;
	padding = _padding;

	auto [xp, yp] = _padding;

	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();

	float gridW = _width * sqrSize + (_width - 1) * xp;
	float gridH = _height * sqrSize + (_height - 1) * yp;

	float startX = (screenW - gridW) / 2.0f;
	float startY = (screenH - gridH) / 2.0f;

	for (int i = 0; i < gridSize; ++i) {
		int gridX = i % _width;
		int gridY = i / _width;

		float x = startX + gridX * (sqrSize + xp);
		float y = startY + gridY * (sqrSize + yp);

		Vector2 pos = { x, y };
		AABB bb = { { x, y }, { x + sqrSize, y + sqrSize } };

		shapes[i] = S{ CellState::MT, pos, bb, sqrSize, &xo, false };
	}
}

template<typename S, int _width, int _height>
void GridManager<S, _width, _height>::draw(CellState player) { for (auto& shape : shapes) { shape.draw(player); } }

template<typename S, int _width, int _height>
void GridManager<S, _width, _height>::update() {
	Vector2 mp = GetMousePosition();
	for (auto& shape : shapes) {
		if (shape.state == CellState::MT) {
			shape.selected = (shape.aabb.isPointInside(mp) && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
		}
		if (shape.aabb.isPointInside(mp) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shape.state == CellState::MT) {
			shape.state = currentPlayer;
			shape.selected = true;
			currentPlayer = (currentPlayer == CellState::X) ? CellState::O : CellState::X;
		}
	}
}

template class GridManager<Square, 3, 3>;