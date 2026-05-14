#include <raylib.h> 
#include <iostream>
#include <vector>
#include <memory>
#include <array>

enum class CellState { MT, X, O };

struct AABB { 
	Vector2 min{}, max{};
	bool isPointInside(float px, float py) const {
		return (px >= min.x && px <= max.x) && 
		       (py >= min.y && py <= max.y);
	}

	bool isPointInside(Vector2 point) const { return isPointInside(point.x, point.y);  }

};

struct Square {
	CellState state = CellState::MT;
	Vector2 position{};
	AABB aabb{};
	int size{};
	bool selected = false;
	int px_s = 5;

	void draw() const {
		if (!selected) { drawNotSelected(); }
		else {}
	}

	void drawNotSelected() const {
		auto [x, y] = position;
		Vector2 mp = GetMousePosition();

		DrawRectangle(x + px_s, y + px_s, size - 2 * px_s, size - 2 * px_s, { 128,128,128,255 });

		if (aabb.isPointInside(mp)) {
			DrawRectangle(x + px_s, y + px_s, size - 3 * px_s, size - 3 * px_s, { 225,225,225,225 });
		}
		else {
			DrawRectangle(x + px_s, y + px_s, size - 3 * px_s, size - 3 * px_s, { 192,192,192,255 });
		}

		DrawRectangle(x, y, size - px_s, px_s, WHITE);
		DrawRectangle(x, y, px_s, size - px_s, WHITE);
		DrawRectangle(x, size + y - px_s, size, px_s, BLACK);
		DrawRectangle(size + x - px_s, y, px_s, size, BLACK);

	}
};

template<typename S, int _width, int _height>
struct GridManager {
	static inline int squareSize{};
	static inline Vector2 padding{};
	static const inline int gridSize = _width * _height;

	static inline std::array<S, gridSize> shapes;
	
	static void setup(int sqrSize, Vector2 _padding) {
		squareSize = sqrSize;
		padding = _padding;

		auto [xp, yp] = _padding;

		int screenW = GetScreenWidth();
		int screenH = GetScreenHeight();

		float gridW = _width  * sqrSize + (_width  - 1) * xp;
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

			shapes[i] = S{ CellState::MT, pos, bb, sqrSize, false};
		}
	}

	static void draw() { for (auto& shape : shapes) { shape.draw(); } }
};


int main(void) {
	InitWindow(1000, 1000, "Tic Tac Toe");
	SetTargetFPS(40);

	using Grid = GridManager<Square, 3, 3>;
	Grid::setup(200, {25.0f, 25.0f});

	for (;;) {
		if (WindowShouldClose()) break;
		BeginDrawing();
		ClearBackground(SKYBLUE);
		Grid::draw();
		EndDrawing();
	}
}