#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace std;
using namespace sf;
#define Max_Size 24

int gamesize = 12;
int gamecolors = 8;
int cellsize = 40; //default Game option is 12 * 12 and the dispaly box is Fixed for all sizes = 480px so in default case cellsize is 480 / 12 = 40
int loading_array[Max_Size][Max_Size]; //For loading Previous Game from File
int flooded[Max_Size][Max_Size] = { 0 };
int recursed[Max_Size][Max_Size] = { 0 };
int max_steps = 100;
int current_step = 0;
RectangleShape shapes[15][15];
Font globl_font;
Music bg_music;

void show_end_msg(String msg) {
	RenderWindow window(VideoMode(800, 600), "Game Ended!", Style::None);
	Texture bg;
	Sprite _bg;
	Text loseing;
	loseing.setFont(globl_font);
	loseing.setString(msg);
	loseing.setPosition(Vector2f(280, 250));
	loseing.setCharacterSize(50);
	bg.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/bg1.jpg");
	_bg.setTexture(bg);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			
	}
		window.clear();
		window.draw(_bg);
		window.draw(loseing);
		window.display();
	}
}
void is_loser(RenderWindow& w, int current) {
	for (int i = 0; i < gamesize;i++) {
		for (int j = 0; j < gamesize;j++) {
			if (flooded[i][j] == 0)
				if (current >= max_steps) {
					show_end_msg("You Lost! :P");
					w.close();
					return;
				}
				else return;
		}
	}
	show_end_msg("\tYou Won! :D\n You Score is " + to_string(current));
}
void intilize_game_window();
Color get_color_by_int(int num);
Color getrandomcolor(int num);
bool is_previous_game_exist() {
	ifstream f;
	f.open("load.txt");
	if (f.fail()) { //In case of file is not exist, Create new one and return false to disable Continue Button
		ofstream create_new("load.txt");
		create_new.close();
		f.close();
		return false;
	}
	char c;
	f.get(c);
	if (f.eof()) { //File is Empty, So no previous Games
		f.close();
		return false;
	}
	string gamesize, gamemap;
	getline(f, gamesize); //Typical Firt line contains Number represents gamesize
	getline(f, gamemap); //Typical Second lind contains series of numbers [0 - 7] represents the color of the map
	f.close();
	int actual_size = atoi(gamesize.c_str());
	for (int i = 0; i < actual_size;i++) {
		for (int j = 0; j < actual_size;j++) {
			loading_array[i][j] = int(gamemap.at(j + i * actual_size));
		}
	}
	f.close();
	return true;
}
void intlize_new(){
	for (int i = 0; i < gamesize; i++) {
		for (int j = 0; j < gamesize; j++) {
			shapes[i][j].setSize(Vector2f(cellsize, cellsize));
			shapes[i][j].setFillColor(getrandomcolor(gamecolors));
			shapes[i][j].setPosition(i * cellsize + 52, j * cellsize + 52); //50 Is the margin
		}
	}
}
void intlize_loaded(){
	for (int i = 0; i < gamesize; i++) {
		for (int j = 0; j < gamesize; j++) {
			shapes[i][j].setSize(Vector2f(20, 20));
			shapes[i][j].setFillColor(get_color_by_int(loading_array[i][j]));
			shapes[i][j].setPosition(i * 15 + 2 * i, j * 15 + 2 * j);
		}
	}
}
void newgame_middleware() {
	intlize_new();
	RenderWindow windowMiddleware(VideoMode(800, 600), "Starting New Game..", Style::None);
	Texture bg, t, s, m, l, h, l4, l5, l6, l7, l8, start, back;
	bg.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/bg1.jpg");
	t.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/tiny.png");
	s.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/smallselected.png");
	m.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/meduim.png");
	l.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/large.png");
	h.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/huge.png");

	l4.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/4.png");
	l5.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/5.png");
	l6.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/6sel.png");
	l7.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/7.png");
	l8.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/8.png");

	start.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/start.png");
	back.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/back.png");

	Sprite _bg, _t, _s, _m, _l, _h, _l4, _l5, _l6, _l7, _l8, _start, _back;
	_bg.setTexture(bg);

	_t.setPosition(Vector2f(25, 100));
	_s.setPosition(Vector2f(275, 100));
	_m.setPosition(Vector2f(525, 100));
	_l.setPosition(Vector2f(150, 200));
	_h.setPosition(Vector2f(400, 200));

	_l4.setPosition(Vector2f(235, 400));
	_l5.setPosition(Vector2f(305, 400));
	_l6.setPosition(Vector2f(375, 400));
	_l7.setPosition(Vector2f(445, 400));
	_l8.setPosition(Vector2f(515, 400));

	_start.setPosition(Vector2f(590, 540));
	_back.setPosition(Vector2f(380, 540));

	while (windowMiddleware.isOpen()) {
		Event event;
		while (windowMiddleware.pollEvent(event)) {
			if (event.type == Event::MouseMoved) {
				int x = event.mouseMove.x;
				int y = event.mouseMove.y;
				if (x > 590 && x < 790 && y > 540 && y < 590) {
					start.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/startsel.png");
				}
				else if (x > 380 && x < 580 && y > 540 && y < 590) {
					back.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/backsel.png");
				}
				else {
					start.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/start.png");
					back.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/back.png");

				}

			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;
					if (x > 590 && x < 790 && y > 540 && y < 590) {
						windowMiddleware.close();
						intilize_game_window();
					}
					else if (x > 380 && x < 580 && y > 540 && y < 590) {
						back.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/backsel.png");
					}
					else {
						start.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/start.png");
						back.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/back.png");

					}

				}
			}
		}

			_t.setTexture(t);
			_s.setTexture(s);
			_m.setTexture(m);
			_l.setTexture(l);
			_h.setTexture(h);

			_l4.setTexture(l4);
			_l5.setTexture(l5);
			_l6.setTexture(l6);
			_l7.setTexture(l7);
			_l8.setTexture(l8);

			_start.setTexture(start);
			_back.setTexture(back);

			windowMiddleware.clear();

			windowMiddleware.draw(_bg);
			windowMiddleware.draw(_t);
			windowMiddleware.draw(_s);
			windowMiddleware.draw(_m);
			windowMiddleware.draw(_l);
			windowMiddleware.draw(_h);
			windowMiddleware.draw(_l4);
			windowMiddleware.draw(_l5);
			windowMiddleware.draw(_l6);
			windowMiddleware.draw(_l7);
			windowMiddleware.draw(_l8);
			windowMiddleware.draw(_start);
			windowMiddleware.draw(_back);

			windowMiddleware.display();
		}
	}
bool prev_game = is_previous_game_exist(); //To take the value of the function that opens file for finding saved games instead of re opening the same file several time
void intilize_main_menu() {
	//if (!globl_font.loadFromFile("C:/f/GLECB.TTF")) cout << "Not loaded"; //TODO Change Directory
	//else cout << "loaded";
	Texture t_bg,b1,b2,b3,b4;
	t_bg.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/bg1.jpg");
	b1.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn1.png");
	if (prev_game)
		b2.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn2.png");
	else 
		b2.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn2dis.png");
	b3.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn3.png");
	b4.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn4.png");

	Sprite bg , _b1,_b2,_b3,_b4;
	bg.setTexture(t_bg);
	bg.setPosition(0, 0);
	_b1.setTexture(b1);
	_b1.setPosition(150, 100);
	_b2.setTexture(b2);
	_b2.setPosition(150, 200);
	_b3.setTexture(b3);
	_b3.setPosition(150, 300);
	_b4.setTexture(b4);
	_b4.setPosition(150, 400);
	RenderWindow window1(VideoMode(800, 600), "Flood it! - Main Menu", Style::None);
	while (window1.isOpen())
	{
		Event event;
		while (window1.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window1.close();
			if (event.type == Event::MouseMoved) {

				int x = event.mouseMove.x;
				int y = event.mouseMove.y;
				if (x > 150 && x < 650 && y > 100 && y < 180) {
					b1.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn1act.png");
					_b1.setTexture(b1);
				}
				else if (x > 150 && x < 650 && y > 200 && y < 280) {
					if (prev_game) {
						b2.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn2act.png");
						_b2.setTexture(b2);
					}
				}
				else if (x > 150 && x < 650 && y > 300 && y < 380) {
					b3.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn3act.png");
					_b3.setTexture(b3);
				}
				else if (x > 150 && x < 650 && y > 400 && y < 480) {
					b4.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn4act.png");
					_b4.setTexture(b4);
				}
				else {
					b1.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn1.png");
					_b1.setTexture(b1);
					if (prev_game) {
						b2.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn2.png");
						_b2.setTexture(b2);
					}
					b3.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn3.png");
					_b3.setTexture(b3);
					b4.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/btn4.png");
					_b4.setTexture(b4);
				}
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == Mouse::Left) {
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;
					if (x > 150 && x < 650 && y > 100 && y < 180) {
						window1.clear();
						window1.close();
						newgame_middleware();
					}
					else if (x > 150 && x < 650 && y > 200 && y < 280) {
						if (prev_game) {
							window1.clear();
							window1.close();
							intlize_loaded();
							intilize_game_window();
						}
					}
					else if (x > 150 && x < 650 && y > 300 && y < 380) {
						window1.clear();

						
		
					}
					else if (x > 150 && x < 650 && y > 400 && y < 480) {
						window1.close();
					}
					else {}
				}
			}
		}
		window1.clear();
		window1.draw(bg);
		window1.draw(_b1);
		window1.draw(_b2);
		window1.draw(_b3);
		window1.draw(_b4);
		window1.display();
	}
}
Color get_color_by_int(int num) {
	switch (num) {
	case 0: return Color::White; break;
	case 1: return Color(0, 0, 255, 140); break;
	case 2: return Color(255, 0, 0, 140); break;
	case 3: return Color(0, 255, 0, 140); break;
	case 4: return Color(255, 255, 0, 140); break;
	case 5: return Color(255, 0, 255, 140); break;
	case 6: return Color(0, 255 , 255, 140); break;
	case 7: return Color(0, 0, 0, 140); break;
	}
}
Color getrandomcolor(int num) {
	int product = rand() % num;
	return get_color_by_int(product);
}
void recurse(int i, int j, Color co) {
	if (i == gamesize || j == gamesize || i < 0 || j < 0) return;
	if (recursed[i][j] == 1) {
		return;
	}
	else recursed[i][j] = 1;
	if (flooded[i][j] == 1) {
		recurse(i, j - 1, co);
		recurse(i, j + 1, co);
		recurse(i + 1, j, co);
		recurse(i - 1, j, co);
	}
	else if (shapes[i][j].getFillColor() == co) {
		flooded[i][j] = 1;
		recurse(i, j - 1, co);
		recurse(i, j + 1, co);
		recurse(i + 1, j, co);
		recurse(i - 1, j, co);
	}
	else return;
}
void clean_and_fill( Color col) {
	for (int i = 0; i < gamesize; i++) {
		for (int j = 0; j < gamesize; j++) {
			recursed[i][j] = 0;
			if (flooded[i][j] == 1) {
				shapes[i][j].setFillColor(col);
			}
		}
	}
}
void action(RenderWindow& w, int color) { //Function to make the action through KeyBoard
	if (shapes[0][0].getFillColor() == get_color_by_int(color)) return;
	recurse(0, 0, get_color_by_int(color));
	is_loser(w, ++current_step);
	clean_and_fill(get_color_by_int(color));
}
void action(RenderWindow& w,Color color) { //Overloaded Function to make the action through mouse Click
	if (shapes[0][0].getFillColor() == color) return;
	recurse(0, 0, color);
	is_loser(w, ++current_step);
	clean_and_fill(color);
}
Color firemouseevent(int x, int y) {
	int i = floor(double(x - 52) / double(cellsize));
	int j = floor(double(y - 52) / double(cellsize));
	return shapes[i][j].getFillColor();
}
void intilize_game_window() {
	bg_music.play();
	RenderWindow window2(VideoMode(800, 600), "Flood it! - Main Menu", Style::None);
	RectangleShape border;
	border.setFillColor(Color::Transparent);
	border.setOutlineThickness(2.0);
	border.setOutlineColor(Color::Yellow);
	border.setSize(Vector2f(484, 484));
	border.setPosition(50, 50);
	Texture t_bg;
	t_bg.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/bg1.jpg");
	Sprite bg;
	bg.setTexture(t_bg);
	bg.setPosition(0, 0);
	Text score;
	score.setFont(globl_font);
	score.setPosition(Vector2f(620, 30));
	score.setCharacterSize(24);

	Text bk, sv_ex, ext;
	bk.setFont(globl_font);
	sv_ex.setFont(globl_font);
	ext.setFont(globl_font);
	bk.setString("Back To Main Menu");
	sv_ex.setString("Save And Exit");
	ext.setString("Exit");
	bk.setCharacterSize(24);
	sv_ex.setCharacterSize(24);
	ext.setCharacterSize(24);
	bk.setPosition(550, 250);
	sv_ex.setPosition(580, 300);
	ext.setPosition(630, 350);
	
	while (window2.isOpen())
	{
		Event event;
		while (window2.pollEvent(event))
		{
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::W) action(window2,0);
				if (event.key.code == Keyboard::B) action(window2, 1);
				if (event.key.code == Keyboard::R) action(window2, 2);
				if (event.key.code == Keyboard::G) action(window2, 3);
				if (event.key.code == Keyboard::Y) action(window2, 4);
				if (event.key.code == Keyboard::M) action(window2, 5);
				if (event.key.code == Keyboard::C) action(window2, 6);
				if (event.key.code == Keyboard::K) action(window2, 7);
			}
			if (event.type == Event::MouseButtonReleased) {
				if (event.mouseButton.button == Mouse::Left) {
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;
					if (x > 52 && x < 532 && y > 52 && y < 532) {
						action(window2, firemouseevent(x, y));
					}
				}
			}
		}
		window2.clear();
		window2.draw(bg);
		score.setString(to_string(current_step) + " / " + to_string(max_steps));
		window2.draw(score);
		window2.draw(border);
		window2.draw(bk);
		window2.draw(sv_ex);
		window2.draw(ext);

		for (int i = 0; i < gamesize; i++) {
			for (int j = 0; j < gamesize; j++) {
				window2.draw(shapes[i][j]);
			}
		}
		window2.display();
	}
}
void intial() {
	bg_music.openFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/bg_m.ogg");
	flooded[0][0] = 1;
	globl_font.loadFromFile("C:/Users/Ahmed Sharaf/Desktop/AUC/Spring 17/CS 110/Projects/FlooditSFML/FlooditSFML/Resources/BAUHS93.TTF");
	srand(time(NULL));
}
int main() {
	intial();
	intilize_main_menu();
	return 0;
}