#pragma once
#include "Cell.h"
#include <array>

template<typename S, int _width, int _height>
class GridManager {
public:
	static inline CellState currentPlayer = CellState::X;
	static inline Texture2D xo{};
	static inline int squareSize{};
	static inline Vector2 padding{};
	static constexpr inline int gridSize = _width * _height;

	//TODO: Change this to an unordered_map for server.
	static inline std::array<S, gridSize> shapes; 

	static void setup(int sqrSize, Vector2 _padding);
	static void draw(CellState player);
	static void update();
private:
	GridManager() = default;
};

using Grid = GridManager<Square, 3, 3>;