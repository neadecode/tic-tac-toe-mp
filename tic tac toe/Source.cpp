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

#define DARK_GRAY Color{128,128,128,255}
#define HOVER_BTN Color{225,225,225,225}
#define BG_BUTTON Color{192,192,192,255}
#define OPAQUE    Color{255,255,255,105}

struct Square {
	CellState state = CellState::MT;
	Vector2 pos{};
	AABB aabb{};
	int size{};
	bool selected = false;
	Texture2D* xo = nullptr;
	int px_s = 5;
	Rectangle src{};
	CellState plays = CellState::X;

	Rectangle getTexSrc(CellState s) const {
		if (s == CellState::X) return { 0, 0, 32, 32 };
		else if (s == CellState::O) return { 32, 0, 32, 32 };
		return {};
	}

	void draw() {
		auto [x, y] = pos;

		src = getTexSrc(state);
		
		DrawRectangle(x + px_s, y + px_s, size - 2 * px_s, size - 2 * px_s, DARK_GRAY);
		(!selected) ? drawNotSelected() : drawSelected();
		drawPersistant();
	}

	void drawNotSelected() const {
		auto [x, y] = pos;
		Vector2 mp = GetMousePosition();

		if (aabb.isPointInside(mp)) {
			DrawRectangle(x + px_s, y + px_s, size - 3 * px_s, size - 3 * px_s, HOVER_BTN);
			DrawTexturePro(
				*xo,
				getTexSrc(plays),
				{ x + px_s, y+ px_s, (float)size- 2*px_s, (float)size- 2*px_s },
				{ 0, 0 }, 
				0.f, 
				OPAQUE
			);
		}
		else {
			DrawRectangle(x + px_s, y + px_s, size - 3 * px_s, size - 3 * px_s, BG_BUTTON);
		}

		DrawRectangle(x, y, size - px_s, px_s, WHITE);
		DrawRectangle(x, y, px_s, size - px_s, WHITE);
		DrawRectangle(x, size + y - px_s, size, px_s, BLACK);
		DrawRectangle(size + x - px_s, y, px_s, size, BLACK);

	}

	void drawSelected() const {
		auto [x, y] = pos;

		DrawRectangle(x + 2*px_s, y + 2*px_s, size - 3 * px_s, size - 3 * px_s, BG_BUTTON);
		DrawTexturePro(
			*xo,
			getTexSrc(plays),
			{ x + 2* px_s, y + 2* px_s, (float)size - 3 * px_s, (float)size - 3 * px_s },
			{ 0, 0 },
			0.f,
			OPAQUE
		);

		DrawRectangle(x, size + y - px_s, size, px_s, WHITE);
		DrawRectangle(size + x - px_s, y, px_s, size, WHITE);
		DrawRectangle(x, y, size, px_s, BLACK);
		DrawRectangle(x, y, px_s, size, BLACK);
	}

	void drawPersistant() const {
		auto [x, y] = pos;
		DrawTexturePro(
			*xo, src,
			{ x + 2 * px_s, y + 2 * px_s, (float)size - 3 * px_s, (float)size - 3 * px_s },
			{ 0, 0 },
			0.f,
			HOVER_BTN
		);
	}
};

template<typename S, int _width, int _height>
struct GridManager {
	static inline CellState currentPlayer = CellState::X;
	static inline Texture2D xo{};
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

			shapes[i] = S{ CellState::MT, pos, bb, sqrSize, false, &xo};
			shapes[i].plays = currentPlayer;
		}
	}

	static void draw() { for (auto& shape : shapes) { shape.draw(); } }
	
	static void handleClick(Vector2 mp) {
		for (auto& shape : shapes) {
			if (shape.state == CellState::MT) {
				shape.selected = (shape.aabb.isPointInside(mp) && IsMouseButtonDown(MOUSE_BUTTON_LEFT));
			}
			if (shape.aabb.isPointInside(mp) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shape.state == CellState::MT) {
				shape.state = currentPlayer;
				shape.selected = true;

				currentPlayer = (currentPlayer == CellState::X) ? CellState::O : CellState::X;
				shape.plays = currentPlayer;
			}
		}
		
	}
private:
	GridManager() = default;
};


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

		Vector2 mp = GetMousePosition();
		Grid::handleClick(mp);
		Grid::draw();
		EndDrawing();
	}
}