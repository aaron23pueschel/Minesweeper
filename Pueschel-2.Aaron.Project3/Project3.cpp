

#include <iostream>
#include "Tile.h"
#include "Pointer_matrix.h"
#include "Sfml_matrix.h"
#include <SFML/Graphics.hpp>
#include <random>
#include "Random.h"

//// If you're reading this, my code has most likely broken.
//// Hopefully you will be able to read my messy code and understand what I have done!
//// There are 5 files total:
//// Tile.h, Random.cpp, Random.h, Sfml_matrix.h, Pointer_matrix.h, Project3.cpp
//// This Implentation uses 3 matrices: the binary file (test cases, random),
//// a matrix containing all the numbers and mines, and lastly a matrix of the display window.


using namespace sf;

//// Declare Methods that belong to the class 

void draw_Window_(sf::RenderWindow& window, output_Matrix Game,std::vector<int>counter_etc, bool debug_mode_off);
void print_array(std::vector<std::vector<int>> array2);
std::vector<std::vector<int>>random_board_binary();
std::vector<std::vector<int>>brd_to_array(std::string fileName);
void update_counter_etc(std::vector<int>& array, int counter, int face);



int main()
{
    //// Create matrix objects
    Matrix  rand(random_board_binary());
    Matrix test_1(brd_to_array("boards/testboard1.brd"));
    Matrix test_2(brd_to_array("boards/testboard2.brd"));
    Matrix test_3(brd_to_array("boards/testboard3.brd"));

    //// Initial board is random and mine count is 50
    output_Matrix Game(rand);
    std::vector<int>start_counter = { 50,1};
    int face = 1;
    int count = 0;
    bool debug_mode_off = true;




    ////Let the fun begin....
   
    //// Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper!       Pueschel, Aaron");
    while (window.isOpen())
    {
        //// Display window without event upon opening 
        if (count == 0) { 
            draw_Window_(window, Game,start_counter,debug_mode_off); 
            window.display();
        }
        count=1;


        //// create event

        sf::Event event;
        
        while (window.pollEvent(event))
        {
            
            //// Close window
            if (event.type == sf::Event::Closed)
                window.close();

            //// Check if user won if he/she picked all the empty tiles
            if (Game.flag_everything(Game.check_win_reveal())) { 
                    face = 3; update_counter_etc(start_counter, Game.number_of_bombs - Game.all_flags.size(), debug_mode_off);
                    Game.game_won = true;
                }
            //// Check if user won by defusing all mines
            if (Game.defused_bombs == Game.number_of_bombs&&Game.number_of_bombs==Game.all_flags.size()) {
                face = 3;
                
                Game.sfml_matrix = Game.integer_matrix.get_matrix_vector();
                Game.rewrite_all_flags2();
                Game.game_won = true;
               
                
            }
            //// Update sfml matrix reader
            update_counter_etc(start_counter, Game.number_of_bombs - Game.all_flags.size(), face);
            draw_Window_(window, Game, start_counter, debug_mode_off);


            //// if the mouse is pressed, enter this statement
            if (event.type == sf::Event::MouseButtonPressed)
            {
                
                
               //// LEFT CLICK
                if (event.mouseButton.button == sf::Mouse::Left)
                {


                    //// RESET RANDOM
                    if (std::floor(event.mouseButton.y / 64.0f) == 8 && (std::floor(event.mouseButton.x / 64.0f) == 5))
                    {
                        
                        Game.game_won = false;
                        Matrix testt(random_board_binary());
                        Game = output_Matrix(testt);
                        Game.integer_matrix.create_pointers();
                        Game.game_lost = false;
                        face = 1;
                        Game.set_number_of_mines();
                        update_counter_etc(start_counter, Game.number_of_bombs -Game.all_flags.size(), face);
                        //draw_Window_(window, Game, start_counter,debug_mode_off);
                     
                    }
                     
                   //// DEBUG
                    if (std::floor(event.mouseButton.y / 64.0f) == 8 && (std::floor(event.mouseButton.x / 64.0f) == 7)){
                        if (debug_mode_off) {
                             debug_mode_off = false;
                        }
                        else {
                             debug_mode_off = true;
                            update_counter_etc(start_counter, Game.number_of_bombs - Game.all_flags.size(), face);
                     
                        }
                     }

                    //// TEST 1
                    if (std::floor(event.mouseButton.y / 64.0f) == 8 && (std::floor(event.mouseButton.x / 64.0f) == 8)){
                         Matrix new_array(brd_to_array("boards/testboard1.brd"));
                         Game = output_Matrix(test_1);
                        Game.integer_matrix.create_pointers();
                        Game.set_number_of_mines();
                        face = 1;

                    }
                    //// TEST 2
                    if (std::floor(event.mouseButton.y / 64.0f) == 8 && (std::floor(event.mouseButton.x / 64.0f) == 9)){
                        Game = output_Matrix(test_2);
                        Game.integer_matrix.create_pointers();
                        Game.set_number_of_mines();
                        face = 1;
                    }

                    //// TEST 3
                    if (std::floor(event.mouseButton.y / 64.0f) == 8 && (std::floor(event.mouseButton.x / 64.0f) == 10)){
                        Game = output_Matrix(test_3);
                        Game.integer_matrix.create_pointers();
                        Game.set_number_of_mines();
                        face = 1;
                    }


                    //// UPDATE GAME STATE
                    //// Regular movements, check if the Game has been won or lost
                    if (!Game.game_won&&!Game.game_lost&&std::floor(event.mouseButton.y / 32.0f) < 16 && std::floor(event.mouseButton.x / 32.0f) < 25) {
                        
                        
                        if (!Game.update_sfml_matrix(std::floor(event.mouseButton.y / 32.0f), std::floor(event.mouseButton.x / 32.0f), true)) {
                            face = 2;
                            update_counter_etc(start_counter, Game.number_of_bombs - Game.all_flags.size(), face);
                            Game.reveal_all_tiles();
                            Game.game_lost = true;
                        }
                    }
                    
                }
                //// RIGHT CLICK
                if (event.mouseButton.button == sf::Mouse::Right){

                    //// UPDATE GAME STATE
                    //// Flag, unflag, all counters associated with these
                    if (!Game.game_won&&!Game.game_lost && std::floor(event.mouseButton.y / 32.0f) < 16 && std::floor(event.mouseButton.x / 32.0f) < 25) {
                        Game.update_sfml_matrix(event.mouseButton.y / 32, event.mouseButton.x / 32, false);
                        update_counter_etc(start_counter, Game.number_of_bombs - Game.all_flags.size(), face);

                    }
 
                }

            }
            window.display();
        }    
    }
    return 0;
}


//// Generates a random board, using Random.h class.
//// Will ALWAYS generate a board with 50 mines.
std::vector<std::vector<int>>random_board_binary() {
 std::vector<std::vector<int>> temp;

    //// If the board does not have 50 mines, try again!
    int count_2 = 0;
    while (count_2 < 50) {
        temp.clear();
        for (int i = 0; i < 16; i++) {
            std::vector<int>temp2;
            for (int j = 0; j < 25; j++) {
                temp2.push_back(0);
            }
            temp.push_back(temp2);
        }

        count_2 = 0;
        for (int i = 0; i < 50; i++) {

            temp[Random::Int(0, 15)][Random::Int(0, 24)] = 1;

        }

        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 25; j++) {
                if (temp[i][j] == 1)
                    count_2++;
            }
        }
    }
    return temp;

}



//// Will read the .brd files. Note that this reads line by line

std::vector<std::vector<int>>brd_to_array(std::string fileName)
{

    std::vector<std::vector<int>> temp;
    ifstream stream(fileName);


    string line;
    while (stream >> line)
    {
        for (int i = 0; i < 16; i++) {
            std::vector<int> temp_array_2;
            for (int j = 0; j < 25; j++) {
                string temp_str = "";
                temp_str += line[j];
                temp_array_2.push_back(std::stoi(temp_str));
            }
            temp.push_back(temp_array_2);
            stream >> line;
        }
    }
    return temp;
}


//// Helper method to draw_window()
//// Used in the main method to update the lower half of the window.
//// This includes face, counter and test tiles

void update_counter_etc(std::vector<int>& array, int counter, int face) {
    array.clear();
    array.push_back(counter);
    array.push_back(face);

}


//// DRAWS THE WINDOW
void draw_Window_(sf::RenderWindow& window, output_Matrix Game, std::vector<int>counter_etc, bool debug_mode_off) {

    sf::Texture debug, digits, happy, lose, win, flag, mine, one, two,
        three, four, five, six, seven, eight, test1, test2, test3, hidden, revealed;


    debug.loadFromFile("images/debug.png");
    digits.loadFromFile("images/digits.png");
    happy.loadFromFile("images/face_happy.png");
    lose.loadFromFile("images/face_lose.png");
    win.loadFromFile("images/face_win.png");
    flag.loadFromFile("images/flag.png");
    mine.loadFromFile("images/mine1.png");
    one.loadFromFile("images/number_1.png");
    two.loadFromFile("images/number_2.png");
    three.loadFromFile("images/number_3.png");
    four.loadFromFile("images/number_4.png");
    five.loadFromFile("images/number_5.png");
    six.loadFromFile("images/number_6.png");
    seven.loadFromFile("images/number_7.png");
    eight.loadFromFile("images/number_8.png");
    test1.loadFromFile("images/test_1.png");
    test2.loadFromFile("images/test_2.png");
    test3.loadFromFile("images/test_3.png");
    hidden.loadFromFile("images/tile_hidden.png");
    revealed.loadFromFile("images/tile_revealed.png");

    Sprite revealed_s(revealed);
    revealed_s.setTextureRect(IntRect(0, 0, 32, 32));
    window.clear(Color::White);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 25; j++) {
            revealed_s.setTextureRect(IntRect(0, 0, 32, 32));
            revealed_s.setPosition(j * 32.0, i * 32.0);
            window.draw(revealed_s);
        }
    }



    //// Draw the regular 25 x 16 matrix, note that there is a special case for debug option, 
    //// Debug will override the input sfml matrix and output the bomb

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 25; j++) {
            Sprite hidden_(hidden);
            Sprite mine(mine);
            if (Game.sfml_matrix[i][j] == -1) {
                
                mine.setTextureRect(IntRect(0, 0, 32, 32));
                mine.setPosition(j * 32.0, i * 32.0);
                window.draw(mine);
            }
            else if (Game.sfml_matrix[i][j] == -2) {

                hidden_.setTextureRect(IntRect(0, 0, 32, 32));
                hidden_.setPosition(j * 32.0, i * 32.0);
                window.draw(hidden_);

            }
            else if (Game.sfml_matrix[i][j] == -3) {


                revealed_s.setPosition(j * 32.0, i * 32.0);
                window.draw(revealed_s);
            }
            else if (Game.sfml_matrix[i][j] == 1) {
                Sprite One(one);
                One.setTextureRect(IntRect(0, 0, 32, 32));
                One.setPosition(j * 32.0, i * 32.0);
                window.draw(revealed_s);
                window.draw(One);

            }
            else if (Game.sfml_matrix[i][j] == 2) {
                Sprite Two(two);
                Two.setTextureRect(IntRect(0, 0, 32, 32));
                Two.setPosition(j * 32.0, i * 32.0);
                window.draw(Two);
            }
            else if (Game.sfml_matrix[i][j] == 3) {
                Sprite Three(three);
                Three.setTextureRect(IntRect(0, 0, 32, 32));
                Three.setPosition(j * 32.0, i * 32.0);
                window.draw(Three);
            }
            else if (Game.sfml_matrix[i][j] == 4) {
                Sprite Four(four);

                Four.setPosition(j * 32.0, i * 32.0);
                window.draw(Four);
            }
            else if (Game.sfml_matrix[i][j] == 5) {
                Sprite Five(five);

                Five.setPosition(j * 32.0, i * 32.0);
                window.draw(Five);
            }
            else if (Game.sfml_matrix[i][j] == 6) {
                Sprite Six(six);

                Six.setPosition(j * 32.0, i * 32.0);
                window.draw(Six);
            }
            else if (Game.sfml_matrix[i][j] == 7) {
                Sprite Seven(seven);

                Seven.setPosition(j * 32.0, i * 32.0);
                window.draw(Seven);
            }
            else if (Game.sfml_matrix[i][j] == 8) {
                Sprite Eight(eight);
                Eight.setTextureRect(IntRect(0, 0, 32, 32));
                Eight.setPosition(j * 32.0, i * 32.0);
                window.draw(Eight);
            }
            else if (Game.sfml_matrix[i][j] == -4) {
                Sprite Flag(flag);
                Flag.setPosition(j * 32.0, i * 32.0);
                hidden_.setPosition(j * 32.0, i * 32.0);
                window.draw(hidden_);
                window.draw(Flag);
            }
            ////Debug override
            if (!debug_mode_off) {
                if (Game.integer_matrix.get_matrix_vector()[i][j] == -1) {
                    hidden_.setTextureRect(IntRect(0, 0, 32, 32));
                    hidden_.setPosition(j * 32.0, i * 32.0);
                    mine.setPosition(j * 32.0, i * 32.0);

                    window.draw(hidden_);
                    window.draw(mine);

                }

            }

        }
    }
    
    //// This draws the lower part of the window. 
    //// This includes the counter, reset, debug and test cases.

    Sprite test_2(test2);
    Sprite test_1(test1);
    Sprite test_3(test3);
    Sprite debug_(debug);

    test_3.setPosition(20*32, 16 * 32);
    test_2.setPosition(18 * 32, 16 * 32);
    test_1.setPosition(16 * 32, 16 * 32);
    debug_.setPosition(14 * 32, 16 * 32);
    
    window.draw(test_3);
    window.draw(test_1);
    window.draw(test_2);
    window.draw(debug_);

    string temp_char = "";

   
    if (counter_etc[1] == 1) {
        Sprite happy_(happy);
        happy_.setPosition(10 * 32, 16 * 32);
        window.draw(happy_);
    }
    else if (counter_etc[1] == 2) {
        Sprite lose_(lose);
        lose_.setPosition(10 * 32, 16 * 32);
        window.draw(lose_);
    }
    else {
        Sprite win_(win);
        win_.setPosition(10 * 32, 16 * 32);
        window.draw(win_);
    }


    ////Negative sign
    Sprite digits_(digits);
    if (counter_etc[0] < 0) {

        digits_.setTextureRect(IntRect(10 * 21, 0, 21, 32));
        digits_.setPosition(0, 16 * 32);
        window.draw(digits_);
    }
    

    ////All this is for the counter:
        string temp_string = std::to_string(std::abs(counter_etc[0]));
        if (temp_string.size() == 1) {
            digits_.setTextureRect(IntRect(0, 0, 21, 32));
            digits_.setPosition(21, 16 * 32);
            window.draw(digits_);
            digits_.setPosition(42, 16 * 32);
            window.draw(digits_);
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[0]) * 21, 0, 21, 32));
            digits_.setPosition(63, 16 * 32);
            window.draw(digits_);
            
            

        }
        else if (temp_string.size() == 2) {
            digits_.setTextureRect(IntRect(0, 0, 21, 32));
            digits_.setPosition(21, 16 * 32);
            window.draw(digits_);
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[0]) * 21, 0, 21, 32));
            digits_.setPosition(42, 16 * 32);
            window.draw(digits_);
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[1]) * 21, 0, 21, 32));
            digits_.setPosition(63, 16 * 32);
            window.draw(digits_);
        }

        else if (temp_string.size() == 3) {
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[0]) * 21, 0, 21, 32));
            digits_.setPosition(21, 16 * 32);
            window.draw(digits_);
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[1]) * 21, 0, 21, 32));
            digits_.setPosition(42, 16 * 32);
            window.draw(digits_);
            digits_.setTextureRect(IntRect(std::stoi(temp_char + temp_string[2]) * 21, 0, 21, 32));
            digits_.setPosition(63, 16 * 32);
            window.draw(digits_);
        }     

}
    
