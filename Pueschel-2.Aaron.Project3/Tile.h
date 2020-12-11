#pragma once
#include <iostream>
#include<vector>

struct all_tiles {



	/// Tile class holds the pointers to the other variables. 
	/// Additionally, it holds the information of whether the tile is flagged
	/// and its coordinates on the board.
	struct tile {
		int x_coordinate;
		int y_coordinate;
		bool checked = false;
		bool flagged = false;
		tile* upperLeft;
		tile* upperRight;
		tile* lowerLeft;
		tile* lowerRight;
		tile* up;
		tile* left;
		tile* right;
		tile* down;
		int value;
	};

	std::vector<std::vector<tile>>tile_vector;
	
};
	
