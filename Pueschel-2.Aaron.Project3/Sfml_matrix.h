#pragma once
//#include <SFML/Graphics.hpp>
#include<vector>
#include "Tile.h"
#include <fstream>
#include <sstream>
#include "Pointer_matrix.h"
#include <set>





//// This Class controls what will be displayed to the window.
//// ... With the exception of one case... More on that later ;)



struct output_Matrix {
	

	//// These variables hold the game state: whether the game is won, lost, how many mines, 
	//// What mines have been flagged, which mines have been defused... you get the idea.
	//// additionally, it holds all the tiles as well as an object from "Pointer_matrix.h"

	int defused_bombs = 0;
	int number_of_bombs;
	bool revealed = false;
	bool game_won = false;
	bool game_lost = false;
	std::vector<std::vector<int>>sfml_matrix;
	std::set<std::vector<int>> all_flags;
	std::set<std::vector<int>> flagged_mines_set;
	Matrix  integer_matrix;

	
	//// Constuctor,  takes in an output_matrix object from Pointer_matrix.h
	//// Initialize all values in sfml_matrix to -2.
	//// If you look at the main file, you will see that:
	//// flag-> -4; revealed tiles->(-3 || 0) ; hidden tiles->-2; 
	//// mine->-1; 1-8 are their respective number tiles.

	output_Matrix(Matrix intMatrix) {
		integer_matrix = intMatrix;
		for (int i = 0; i < 16; i++) {
			std::vector<int>temp;
			for (int j = 0; j < 25; j++) {
				temp.push_back(-2);
			}
			sfml_matrix.push_back(temp);
		}
		set_number_of_mines();
	}

	//// helper file when revealing all flags... pretty sure its obsolete since I made a pt.2
	//// look below.
	void rewrite_all_flags() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 25; j++) {
				if (integer_matrix.class_matrix.tile_vector[i][j].flagged) {
					sfml_matrix[i][j] = -4;
					
				}

			}
		}
	}

	//// Uses a set to determine which tiles are flagged, this way when I
	//// call the function, my code wont break because there are two of the same coordinates.
	//// Trust me, it did, hence, rewrite_all_flags pt.2
	void rewrite_all_flags2() {
		for (auto set : all_flags) {
			sfml_matrix[set[0]][set[1]] = -4;
		}
	}

	//// Uses the class from Pointer_Matrix.h to reveal all the... bombs? 
	//// I use this method when the user loses.
	std::vector<std::vector<int>> reveal_all_tiles() {
		std::vector<std::vector<int>> old_array;
		for (int i = 0; i < 16; i++) {
			std::vector<int> temp_arr;
			for (int j = 0; j < 25; j++) {
				temp_arr.push_back(sfml_matrix[i][j]);

				if (integer_matrix.get_matrix_vector()[i][j] == -1) {
					sfml_matrix[i][j] = integer_matrix.get_matrix_vector()[i][j];
				}
				
			}
			old_array.push_back(temp_arr);
		}
		return old_array;
	}

	//// Counts all the mines in the input file and sets it equal to the number_of_mines variable above.
	//// This method will tell the counter what to display.
	void set_number_of_mines() {
		int temp_count = 0;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 25; j++) {
				if (integer_matrix.get_matrix_vector()[i][j] == -1) {
					temp_count++;
				}

			}
		}
		number_of_bombs = temp_count;
		
	}

	
	//// Takes an input (Tests, random etc ) and restores all matrices.
	//// this is to avoid creating multiple objects.
	void reset_binary_matrix_sfml(std::vector<std::vector<int>> values) {
		integer_matrix.reset_binary_matrix(values);
		sfml_matrix.clear();
		for (int i = 0; i < 16; i++) {
			std::vector<int>temp;
			for (int j = 0; j < 25; j++) {
				temp.push_back(-2);
			}
			sfml_matrix.push_back(temp);
		}

	}

	//// check if player won by selecting all the squares
	bool check_win_reveal() {
		int count = 0;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 25; j++) {
				if (sfml_matrix[i][j] >=0) {
					count++;
				}
				if (sfml_matrix[i][j] == -3) {
					count++;
				}
			}
		}

		if (400 - number_of_bombs == count) { return true; }
		else return false;

	}

	//// Flags all the tiles that have a -1 underneath, this is used when the player has won.
	
	bool flag_everything(bool won_by_default) {

		if (won_by_default) {
			int count = 0;
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 25; j++) {
					if (integer_matrix.get_matrix_vector()[i][j] == -1) {
						sfml_matrix[i][j] = -4;
						std::vector<int> temp_arr;
						temp_arr.push_back(i);
						temp_arr.push_back(j);
						all_flags.insert(temp_arr);
						flagged_mines_set.insert(temp_arr);
						
					}
				}
			}
			
			return true;
		}
		return false;

	}



	//// VERY IMPORTANT
	//// Takes in coordinates from the mouse and makes the appropriate updates to the 
	//// sfml matrix. This method uses most of the methods above to change the game state
	//// based on player input.
	//// returns false if the player lost.
	bool update_sfml_matrix(int x, int y, bool left_click) {

		

		////left click on a bomb, lose game
		if (sfml_matrix[x][y] == -2 && left_click && integer_matrix.get_matrix_vector()[x][y] == -1) {
			game_lost = true;

			return false;
		}
		////right click on a bomb, defuse bomb
		if (sfml_matrix[x][y] == -2 && !left_click && integer_matrix.get_matrix_vector()[x][y] == -1) {
			std::vector<int> flag_coordinates;
			flag_coordinates.push_back(x);
			flag_coordinates.push_back(y);
			all_flags.insert(flag_coordinates);
			flagged_mines_set.insert(flag_coordinates);
			
			defused_bombs= flagged_mines_set.size();
			sfml_matrix[x][y] = -4;
			return true;
		}
		
		/////right click or flag a square that is not a mine
		if (sfml_matrix[x][y] == -2 && !left_click && integer_matrix.get_matrix_vector()[x][y] != -1) {
			std::vector<int> flag_coordinates;
			flag_coordinates.push_back(x);
			flag_coordinates.push_back(y);
			all_flags.insert(flag_coordinates);
			
			integer_matrix.class_matrix.tile_vector[x][y].flagged = true;
			rewrite_all_flags();
			return true;
		}
		/////////unflag
		if (sfml_matrix[x][y] == -4 && !left_click ) {
			std::vector<int> flag_coordinates;
			flag_coordinates.push_back(x);
			flag_coordinates.push_back(y);
			if (all_flags.find(flag_coordinates) != all_flags.end()) { all_flags.erase(flag_coordinates); }
			integer_matrix.class_matrix.tile_vector[x][y].flagged = false;
			
			sfml_matrix[x][y] = -2;
			if (flagged_mines_set.find(flag_coordinates) != flagged_mines_set.end()) { 
				flagged_mines_set.erase(flag_coordinates);
				defused_bombs = flagged_mines_set.size();
			}

			return true;
		}
		//reveal normal number value
		if (sfml_matrix[x][y] == -2 && left_click && integer_matrix.get_matrix_vector()[x][y] >0) {
			sfml_matrix[x][y] = integer_matrix.get_matrix_vector()[x][y];
			rewrite_all_flags();
			return true;
		}
		/////reveal local zeroes


		if (sfml_matrix[x][y] == -2 && left_click && integer_matrix.get_matrix_vector()[x][y] == 0) {
			reveal_local_zeros(x, y);
			rewrite_all_flags();
			return true;

			
		}
		return true;
		
	}
	
	//// method to find all the local zeroes and reveal the ones next to it.
	void reveal_local_zeros(int x, int y) {

		auto tile_ptr = integer_matrix.class_matrix.tile_vector[x][y];
		check_nearby_tiles(&tile_ptr);
	}

	//// reveals all the surrounding tiles, if a tile is zero or -3;
	//// uses pointers to check tile values
	void reveal_surrounding_tiles(all_tiles::tile* tile) {
		
		
		
		//up
		if (tile->up != nullptr && tile->up->value != -1 &&sfml_matrix[tile->x_coordinate-1][tile->y_coordinate] != -4) {
			sfml_matrix[tile->x_coordinate - 1][tile->y_coordinate] = tile->up->value != 0 ? tile->up->value : -3;
		}
		//down
		if (tile->down != nullptr && tile->down->value != -1 && sfml_matrix[tile->x_coordinate + 1][tile->y_coordinate]!=4)
		sfml_matrix[tile->x_coordinate+1][tile->y_coordinate]=tile->down->value != 0 ? tile->down->value : -3;
		//left
		if (tile->left != nullptr && tile->left->value != -1 && sfml_matrix[tile->x_coordinate][tile->y_coordinate - 1]!=-4)
		sfml_matrix[tile->x_coordinate][tile->y_coordinate-1]= tile->left->value != 0 ? tile->left->value : -3;
		//right
		if (tile->right != nullptr && tile->right->value != -1&& sfml_matrix[tile->x_coordinate][tile->y_coordinate + 1]!=-4)
		sfml_matrix[tile->x_coordinate][tile->y_coordinate+1]= tile->right->value != 0 ? tile->right->value : -3;
		//upper left
		if (tile->upperLeft != nullptr && tile->upperLeft->value != -1&& sfml_matrix[tile->x_coordinate - 1][tile->y_coordinate - 1]!=-4)
		sfml_matrix[tile->x_coordinate-1][tile->y_coordinate-1]= tile->upperLeft->value != 0 ? tile->upperLeft->value : -3;
		//upper right
		if (tile->upperRight != nullptr && tile->upperRight->value != -1 && sfml_matrix[tile->x_coordinate - 1][tile->y_coordinate + 1]!=-4)
		sfml_matrix[tile->x_coordinate-1][tile->y_coordinate+1]= tile->upperRight->value != 0 ? tile->upperRight->value : -3;
		//lower left
		if (tile->lowerLeft != nullptr && tile->lowerLeft->value != -1&& sfml_matrix[tile->x_coordinate + 1][tile->y_coordinate - 1]!=-4)
		sfml_matrix[tile->x_coordinate+1][tile->y_coordinate-1]= tile->lowerLeft->value != 0 ? tile->lowerLeft->value : -3;
		//lower right
		if (tile->lowerRight != nullptr && tile->lowerRight->value != -1&& sfml_matrix[tile->x_coordinate + 1][tile->y_coordinate + 1]!=-4)
		sfml_matrix[tile->x_coordinate+1][tile->y_coordinate+1]= tile->lowerRight->value != 0 ? tile->lowerRight->value : -3;


	}

	//// Recursive function! 
	//// checks the surrounding tiles recursively, does not check the same tile twice! 
	//// program must terminate.
	//// used to reveal all of the zeroes

	all_tiles::tile* check_nearby_tiles(all_tiles::tile* tile) {
		if (tile->value == 0&&sfml_matrix[tile->x_coordinate][tile->y_coordinate]!=-4) {
			reveal_surrounding_tiles(tile);
			sfml_matrix[tile->x_coordinate][tile->y_coordinate] = -3;
			tile->checked = true;

			//down
			if (tile->down != nullptr && tile->down->value == 0 && !tile->down->checked && !tile->down->flagged)
			{

				sfml_matrix[tile->down->x_coordinate][tile->down->y_coordinate] = -3;
				tile->down->checked = true;
				check_nearby_tiles(tile->down);
			}
			//up
			if (tile->up != nullptr && tile->up->value == 0 && !tile->up->checked && !tile->up->flagged)
			{
				sfml_matrix[tile->up->x_coordinate][tile->up->y_coordinate] = -3;
				tile->up->checked = true;
				check_nearby_tiles(tile->up);
			}
			//left
			if (tile->left != nullptr && tile->left->value == 0 && !tile->left->checked && !tile->left->flagged)
			{
				sfml_matrix[tile->left->x_coordinate][tile->left->y_coordinate] = -3;
				tile->left->checked = true;
				check_nearby_tiles(tile->left);
			}
			//right
			if (tile->right != nullptr && tile->right->value == 0 && !tile->right->checked && !tile->right->flagged)
			{
				sfml_matrix[tile->right->x_coordinate][tile->right->y_coordinate] = -3;
				tile->right->checked = true;
				check_nearby_tiles(tile->right);
			}
			//upper left
			if (tile->upperLeft != nullptr && tile->upperLeft->value == 0 && !tile->upperLeft->checked && !tile->upperLeft->flagged)
			{
				sfml_matrix[tile->upperLeft->x_coordinate][tile->upperLeft->y_coordinate] = -3;
				tile->upperLeft->checked = true;
				check_nearby_tiles(tile->upperLeft);
			}
			//upper right
			if (tile->upperRight != nullptr && tile->upperRight->value == 0 && !tile->upperRight->checked && !tile->upperRight->flagged)
			{
				sfml_matrix[tile->upperRight->x_coordinate][tile->upperRight->y_coordinate] = -3;
				tile->upperRight->checked = true;
				check_nearby_tiles(tile->upperRight);
			}
			//lower left
			if (tile->lowerLeft != nullptr && tile->lowerLeft->value == 0 && !tile->lowerLeft->checked && !tile->lowerLeft->flagged)
			{
				sfml_matrix[tile->lowerLeft->x_coordinate][tile->lowerLeft->y_coordinate] = -3;
				tile->lowerLeft->checked = true;
				check_nearby_tiles(tile->lowerLeft);
			}
			//lower right
			if (tile->lowerRight != nullptr && tile->lowerRight->value == 0 && !tile->lowerRight->checked && !tile->lowerRight->flagged)
			{
				sfml_matrix[tile->lowerRight->x_coordinate][tile->lowerRight->y_coordinate] = -3;
				
				tile->lowerRight->checked = true;
				check_nearby_tiles(tile->lowerRight);
				
			}

		}
		return nullptr;
	}

};