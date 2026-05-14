#include "Cell.h"

Square::Square(CellState st, Vector2 p, AABB box, int sz, Texture2D* texture, bool sel) : 
	state(st), pos(p), aabb(box), size(sz), xo(texture), selected(sel)
	{
		x = pos.x; y = pos.y;
	}

void Square::draw(CellState player) {
	DrawRectangle(x + px, y + px, size - 2 * px, size - 2 * px, DARK_GRAY); //Bevel
	drawSelection(player);
	stamp();
}

void Square::drawSelection(CellState player) const {
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
void Square::stamp() {
	DrawTexturePro(*xo, getPlayer(this->state),
		{ x + 1.5f * px, y + 1.5f * px, (float)size - 2.5f * px, (float)size - 2.5f * px },
		{ 0, 0 }, 0.f, HOVER_BTN);
}

