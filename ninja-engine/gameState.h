#pragma once

class Inventory {
	public:
		bool has_bombs;

		Inventory();
};

//! Saved game data that should persist across different game sessions
class GameState {
	public:
		Inventory _inventory;

		GameState();
};