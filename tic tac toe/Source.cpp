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
	int size	    = 0;
	Texture2D* xo   = nullptr;
	bool selected   = false;
private:
	int px = 5;
	float x = 0.0f;   
	float y = 0.0f;
public:
	Square() = default;
	Square(CellState st, Vector2 p, AABB box, int sz, Texture2D* texture, bool sel): 
		state(st), pos(p), aabb(box), size(sz), xo(texture), selected(sel)
	{ x = pos.x; y = pos.y; }
private:
	Rectangle getPlayer(CellState s) const {
		if (s == CellState::X) return { 0, 0, 32, 32 };
		else if (s == CellState::O) return { 32, 0, 32, 32 };
		return {};
	}
public:
	void draw(CellState player) {
		DrawRectangle(x + px, y + px, size - 2 * px, size - 2 * px, DARK_GRAY); //Bevel
		drawSelection(player);
		stamp();
	}
private:
	void drawSelection(CellState player) const {
		Vector2 mp = GetMousePosition();
		bool hovering = (aabb.isPointInside(mp));

		Color innerColor = (hovering) ? HOVER_BTN : BG_BUTTON;
		Rectangle playerPress = (selected) ?
			Rectangle{ x + 1.5f * px, y + 1.5f * px, (float)size - 2.5f * px, (float)size - 2.5f * px } :
			Rectangle{ x + px, y + px, (float)size - 2 * px, (float)size - 2 * px };

		DrawRectangle(x + px, y + px, size - 3 * px, size - 3 * px, innerColor); //BG

		if (!selected) { //Lines out
			DrawRectangle(x, y, size - px, px, WHITE);
			DrawRectangle(x, y, px, size - px, WHITE);
			DrawRectangle(x, size + y - px, size, px, BLACK);
			DrawRectangle(size + x - px, y, px, size, BLACK);
		}
		if (hovering && !selected) { //Player out
			DrawTexturePro(*xo, getPlayer(player), playerPress, { 0, 0 }, 0.f, OPAQUE);
		}
		if (selected) { //In
			if (state == CellState::MT) DrawTexturePro(*xo, getPlayer(player), playerPress, { 0, 0 }, 0.f, OPAQUE);
			DrawRectangle(x, size + y - px, size, px, WHITE);
			DrawRectangle(size + x - px, y, px, size, WHITE);
			DrawRectangle(x, y, size, px, BLACK);
			DrawRectangle(x, y, px, size, BLACK);
		} 
	}

	//Only works when the getPlayer returns a valid source. Not Rectangle{}.
	void stamp() {
		DrawTexturePro(*xo, getPlayer(this->state), 
			{ x + 1.5f * px, y + 1.5f * px, (float)size - 2.5f * px, (float)size - 2.5f * px },
			{ 0, 0 }, 0.f, HOVER_BTN);
	}
};

template<typename S, int _width, int _height>
class GridManager {
public:
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
#pragma region Initializing shapes from position
		for (int i = 0; i < gridSize; ++i) {
			int gridX = i % _width;
			int gridY = i / _width;

			float x = startX + gridX * (sqrSize + xp);
			float y = startY + gridY * (sqrSize + yp);
	
			Vector2 pos = { x, y };
			AABB bb = { { x, y }, { x + sqrSize, y + sqrSize } };

			shapes[i] = S{CellState::MT, pos, bb, sqrSize, &xo, false};
		}
#pragma endregion
	}

	static void draw(CellState player) { for (auto& shape : shapes) { shape.draw(player); } }
	
	static void update(Vector2 mp) {
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
		Grid::update(mp);
		Grid::draw(Grid::currentPlayer);
		EndDrawing();
	}
}

int WinMain(void) {
	main();
}