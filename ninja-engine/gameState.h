#pragma once

class Inventory {
	public:
		bool has_bombs;
		int ring_count;

		Inventory();
};

//! Saved game data that should persist across different game sessions
class GameState {
	public:
		Inventory _inventory;

		GameState();
};