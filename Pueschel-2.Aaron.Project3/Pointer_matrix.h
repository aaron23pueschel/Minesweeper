#pragma once

#include<vector>
#include "Tile.h"
#include <fstream>
#include <sstream>

//// This is the matrix class that will store all the tiles and allow 
//// the program to work properly! without this class, there could be no number tiles.
//// This class is the backbone of this Project, since it finds all the mines
//// and assigns each tile with its appropriate value.
struct Matrix {
	
	////  class_matrix is the object that holds all of the tiles.
	all_tiles class_matrix;



	/////// Matrix() object  default constructor.
	Matrix()
	{
		
		for (int i = 0; i < 16; i++) {
			std::vector<all_tiles::tile> temp;
			for (int j = 0; j < 25; j++) {
			all_tiles::tile newTile;
				newTile.x_coordinate = i;
				newTile.y_coordinate = j;
				newTile.value = 0;
				temp.push_back(newTile);

			}
			class_matrix.tile_vector.push_back(temp);
		}
		create_pointers();
	}

	//// Custom constructer, takes in a binary integer matrix, eg. the test files, random files etc.
	Matrix(std::vector<std::vector<int>> values)
	{

		for (int i = 0; i < 16; i++) {
			std::vector<all_tiles::tile> temp;
			for (int j = 0; j < 25; j++) {
				all_tiles::tile newTile;
				newTile.value = values[i][j];
				newTile.x_coordinate = i;
				newTile.y_coordinate = j;
				temp.push_back(newTile);

			}
			class_matrix.tile_vector.push_back(temp);
		}

		//// helper functions that assign pointers, translate the binary matrix and assign the
		//// respective number to the tile.
		create_pointers();
		translate();
		alter_values(-1);
	}

	//// Returns the object matrix as a 2D integer array
	std::vector<std::vector<int>> get_matrix_vector() {
		std::vector<std::vector<int>>temp;
		for (int i = 0; i < 16; i++) {
			std::vector<int> temp_array_2;

			for (int j = 0; j < 25; j++) {
				temp_array_2.push_back(class_matrix.tile_vector[i][j].value);
			}
			temp.push_back(temp_array_2);
		}	
		return temp;
	}
	
	//// This is used to reset the board, takes in a binary matrix and creates the
	//// integer matrix object
	void reset_binary_matrix(std::vector<std::vector<int>> values) {
		class_matrix.tile_vector.clear();
		for (int i = 0; i < 16; i++) {
			std::vector<all_tiles::tile> temp;
			for (int j = 0; j < 25; j++) {
				all_tiles::tile newTile;
				newTile.value = values[i][j];
				newTile.x_coordinate = i;
				newTile.y_coordinate = j;
				temp.push_back(newTile);
			}
			class_matrix.tile_vector.push_back(temp);
		}
		translate();
		create_pointers();
		alter_values(-1);

	}

	//// Helper function in alter_values(), 
	//// finds a particular set of tiles and returns its coordinates
	std::vector<std::vector<int>>find_values(int value_) {
		std::vector<std::vector<int>> temp_array;
		for (int i = 0; i < 16; i++) {

			for (int j = 0; j < 25; j++) {
				std::vector<int> temp_array2;

				if (class_matrix.tile_vector[i][j].value == value_) {
					temp_array2.push_back(i);
					temp_array2.push_back(j);
					temp_array.push_back(temp_array2);

				}
			}
		}
		return temp_array;
	}


	//// Print the array, *Ahem* for debbugging purposes
	void print_array()
	{
		for (int i = 0; i < 16; i++) {

			for (int j = 0; j < 25; j++) {
				std::cout << class_matrix.tile_vector[i][j].value << "   ";
			}
			std::cout << std::endl;
		}
	}

	//// Helper function.
	void translate() {
		for (int i = 0; i < 16; i++) {

			for (int j = 0; j < 25; j++) {
				class_matrix.tile_vector[i][j].value *= -1;
			}
		}
		
	}

	//// DO NOT USE THIS METHOD WITHOUT CREATING THE POINTERS FIRST
	//// This is the bread and butter of the numbers.
	//// essentially, go tile by tile and count the number of mines within a 1 block range. Smple.
	void alter_values(int i) {
		for (auto value : find_values(i)) {

			//// All these statements use the pointers to check surrounding tiles for mines.
			if (class_matrix.tile_vector[value[0]][value[1]].down != nullptr&& class_matrix.tile_vector[value[0]][value[1]].down->value >= 0)
				 class_matrix.tile_vector[value[0]][value[1]].down->value += 1; 
			if (class_matrix.tile_vector[value[0]][value[1]].up != nullptr&& class_matrix.tile_vector[value[0]][value[1]].up->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].up->value += 1;
			if (class_matrix.tile_vector[value[0]][value[1]].left != nullptr&& class_matrix.tile_vector[value[0]][value[1]].left->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].left->value += 1;
			if (class_matrix.tile_vector[value[0]][value[1]].right != nullptr&& class_matrix.tile_vector[value[0]][value[1]].right->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].right->value += 1;

			if (class_matrix.tile_vector[value[0]][value[1]].upperLeft != nullptr&& class_matrix.tile_vector[value[0]][value[1]].upperLeft->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].upperLeft->value += 1;
			if (class_matrix.tile_vector[value[0]][value[1]].upperRight != nullptr && class_matrix.tile_vector[value[0]][value[1]].upperRight->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].upperRight->value += 1;
			if (class_matrix.tile_vector[value[0]][value[1]].lowerLeft != nullptr&& class_matrix.tile_vector[value[0]][value[1]].lowerLeft->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].lowerLeft->value += 1;
			if (class_matrix.tile_vector[value[0]][value[1]].lowerRight != nullptr&& class_matrix.tile_vector[value[0]][value[1]].lowerRight->value >= 0)
				class_matrix.tile_vector[value[0]][value[1]].lowerRight->value += 1;

			//matrix.tile_vector[value[0]][value[1]].value = -3;
		}

	}


	//// Go tile by tile and make the pointers. 
	//// Keep in mind that I used the idea of an actual matrix, like the math one... to implement this
	//// Basically, check tiles surrounding and if a tile doesnt exist, assign a null pointer.
	void create_pointers() {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 25; j++) {
				//upper left
				if (i - 1 >= 0 && j - 1 >= 0) {
					class_matrix.tile_vector[i][j].upperLeft = &class_matrix.tile_vector[i - 1][j - 1];
				}
				else {
					class_matrix.tile_vector[i][j].upperLeft = nullptr;
				}


				//up
				if (i - 1 >= 0) {
					class_matrix.tile_vector[i][j].up = &class_matrix.tile_vector[i - 1][j];
				}
				else {
					class_matrix.tile_vector[i][j].up = nullptr;
				}

				//upper right
				if (i - 1 >= 0 && j + 1 < 25) {
					class_matrix.tile_vector[i][j].upperRight = &class_matrix.tile_vector[i - 1][j + 1];
				}
				else {
					class_matrix.tile_vector[i][j].upperRight = nullptr;
				}

				//left
				if (j - 1 >= 0) {
					class_matrix.tile_vector[i][j].left = &class_matrix.tile_vector[i][j - 1];
				}
				else {
					class_matrix.tile_vector[i][j].left = nullptr;
				}

				//right
				if (j + 1 < 25) {
					class_matrix.tile_vector[i][j].right = &class_matrix.tile_vector[i][j + 1];
				}
				else {
					class_matrix.tile_vector[i][j].right = nullptr;
				}

				//down
				if (i + 1 < 16) {
					class_matrix.tile_vector[i][j].down = &class_matrix.tile_vector[i + 1][j];
				}
				else {
					class_matrix.tile_vector[i][j].down = nullptr;
				}

				//lower left
				if (i + 1 < 16 && j - 1 >= 0) {
					class_matrix.tile_vector[i][j].lowerLeft = &class_matrix.tile_vector[i + 1][j - 1];
				}
				else {
					class_matrix.tile_vector[i][j].lowerLeft = nullptr;
				}

				//lower right
				if (i + 1 < 16 && j + 1 < 25) {
					class_matrix.tile_vector[i][j].lowerRight = &class_matrix.tile_vector[i + 1][j + 1];
				}
				else {
					class_matrix.tile_vector[i][j].lowerRight = nullptr;
				}
			}
		}
	}
};

//// std::endl;