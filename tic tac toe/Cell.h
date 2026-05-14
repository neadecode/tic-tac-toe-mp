#pragma once
#include <raylib.h>

#define DARK_GRAY Color{128,128,128,255}
#define HOVER_BTN Color{225,225,225,225}
#define BG_BUTTON Color{192,192,192,255}
#define OPAQUE    Color{255,255,255,105}

enum class CellState { MT, X, O };

struct AABB {
	Vector2 min{}, max{};
	bool isPointInside(float px, float py) const {
		return (px >= min.x && px <= max.x) &&
			(py >= min.y && py <= max.y);
	}
	bool isPointInside(Vector2 point) const { return isPointInside(point.x, point.y); }
};

struct Square {
	CellState state = CellState::MT;
	Vector2 pos{};
	AABB aabb{};
	int size		= 0;
	Texture2D* xo	= nullptr;
	bool selected	= false;

	Square() = default;
	Square(CellState st, Vector2 p, AABB box, int sz, Texture2D* texture, bool sel);
	void draw(CellState player);
private:
	int px	= 5;
	float x{}, y{};

	Rectangle getPlayer(CellState s) const {
		if (s == CellState::X) return { 0, 0, 32, 32 };
		else if (s == CellState::O) return { 32, 0, 32, 32 };
		return {};
	}
private:
	void drawSelection(CellState player) const;
	void stamp(); //Only works when the getPlayer returns a valid source. Not Rectangle{}.
};