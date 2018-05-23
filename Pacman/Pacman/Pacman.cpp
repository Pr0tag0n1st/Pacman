//libraries
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include<iostream>

using namespace std;

//Directional const ints
const int LEFT = 1;
const int UP = 2;
const int RIGHT = 3;
const int DOWN = 4;

//Project variables
ALLEGRO_DISPLAY*display = NULL;
ALLEGRO_BITMAP*pacman = NULL;
ALLEGRO_BITMAP*wall = NULL;
ALLEGRO_BITMAP*dot = NULL;
ALLEGRO_SAMPLE*bite = NULL;
ALLEGRO_SAMPLE*win = NULL;
ALLEGRO_SAMPLE*lose = NULL;
ALLEGRO_SAMPLE*background = NULL;
ALLEGRO_SAMPLE_INSTANCE*instance = NULL;
ALLEGRO_EVENT_QUEUE*event_queue = NULL;
ALLEGRO_TIMER*timer = NULL;
ALLEGRO_FONT*font = NULL;
int dotseaten =0;
ALLEGRO_BITMAP*dino = NULL;

//Collision function
int wallCollide(int x, int y, int dir, int map[21][20]);

//Ghost class
class Ghost {
public:
	void initGhost(int x, int y, int w, int h);
	void drawGhost1();
	void drawGhost2();
	void drawGhost3();
	void drawGhost4();
	bool IsDead();
	void printInfo();
	void killGhost();
//	int wallCollide(int x, int y, int dir, int map[21][20]);
	void chase1(int x, int y, int map[21][20]);
	void chase2(int x, int y, int map[21][20]);
	void chase3(int x, int y, int map[21][20]);
	void chase4(int x, int y, int map[21][20]);
	bool getPacman(int b1x, int b1y, int b2x, int b2y);
private:
	int hitwall;
	int Xpos;
	int Ypos;
	int dir;
	int speed;
	bool dead;
	int Color;
	int width;
	int height;
};

//Start of main
int main() {
	//Gameplay variables
	bool done = false;
	int pacman_x = 384;
	int pacman_y = 364;
	int lives = 3;
	bool doexit = false;
	bool key[4]{ false, false, false, false };
	bool redraw = true;
	int sprite_x = pacman_x;
	int sprite_y = pacman_y;

	//Animation info (unused)
	const int maxFrame = 4;
	int curframe = 0;
	int frameCount = 4;
	int framedelay = 5;
	int frameWidth = 32;
	int frameHeight = 32;

	//Allegro initialization
	al_init();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_image_addon();
	al_reserve_samples(5);

	//Matrix (map)
	int map[21][20]{
		1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
		0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1,
		0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
		1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
	};

	//Project variable setup
	al_install_keyboard();
	timer = al_create_timer(.02);
	event_queue = al_create_event_queue();
	font = al_create_builtin_font();
	win = al_load_sample("Victory.wav");
	bite = al_load_sample("chomp.wav");
	background = al_load_sample("Pacman.wav");
	instance = al_create_sample_instance(background);
	al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
	al_play_sample_instance(instance);
	display = al_create_display(800, 840);
	pacman = al_create_bitmap(32, 32);
	wall = al_create_bitmap(40, 40);
	dot = al_create_bitmap(4, 4);
	dino = al_load_bitmap("DinoStudios.jpg");
	al_set_target_bitmap(pacman);
	al_clear_to_color(al_map_rgb(255, 255, 0));
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(200, 200, 255));
	al_set_target_bitmap(dot);
	al_clear_to_color(al_map_rgb(255, 255, 0));
	al_set_target_bitmap(al_get_backbuffer(display));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	//Ghost instantiation + initialization
	Ghost chandelure;
	chandelure.initGhost(324, 164, 32, 32);
	Ghost dusknoir;
	dusknoir.initGhost(364, 164, 32, 32);
	Ghost gengar;
	gengar.initGhost(404, 164, 32, 32);
	Ghost golurk;
	golurk.initGhost(444, 164, 32, 32);


	

	al_draw_bitmap(dino, 0, 0, 0);
	al_flip_display();
	al_rest(1.5);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_start_timer(timer);

	//Start game loop but restrict for lose, win, and exit conditions
	while (!doexit && lives !=0 && dotseaten != 198)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		//Movement algorithm with collision
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[0] && wallCollide(pacman_x, pacman_y, UP, map) == 0) {
				pacman_y -= 4.0;

			}

			if (key[1] && wallCollide(pacman_x, pacman_y, DOWN, map) == 0) {
				pacman_y += 4.0;
			}

			if (key[2] && wallCollide(pacman_x, pacman_y, LEFT, map) == 0) {
				pacman_x -= 4.0;
			}

			if (key[3] && wallCollide(pacman_x, pacman_y, RIGHT, map) == 0) {
				pacman_x += 4.0;
			}

			//Create dots and enable them to be eaten
			if (map[(pacman_y + 16) / 40][(pacman_x + 16) / 40] == 0) {
				map[(pacman_y + 16) / 40][(pacman_x + 16) / 40] = 2;
				bite = al_load_sample("chomp.wav");
				al_play_sample(bite, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				dotseaten++;
			}


			//Warp zones
			if (((key[0] && pacman_x > 216 && pacman_x < 264 && pacman_y == 0) ||
				(key[0] && pacman_x > 356 && pacman_x < 412 && pacman_y == 0)) ||
				(key[0] && pacman_x > 516 && pacman_x < 564 && pacman_y == 0))
			{
				pacman_y = 816;
			}
			if (((key[1] && pacman_x > 216 && pacman_x < 264 && pacman_y == 804) ||
				(key[1] && pacman_x > 356 && pacman_x < 412 && pacman_y == 804)) ||
				(key[1] && pacman_x > 520 && pacman_x < 560 && pacman_y == 804)) {
				pacman_y = 0;
			}
			if ((((key[2] && pacman_x == 0 && pacman_y > 156 && pacman_y < 204) ||
				(key[2] && pacman_x == 0 && pacman_y > 276 && pacman_y < 324)) ||
				(key[2] && pacman_x == 0 && pacman_y > 396 && pacman_y < 444)) ||
				(key[2] && pacman_x == 0 && pacman_y > 596 && pacman_y < 644)) {
				pacman_x = 764;
			}
			if ((((key[3] && pacman_x == 764 && pacman_y > 156 && pacman_y < 204) ||
				(key[3] && pacman_x == 764 && pacman_y > 276 && pacman_y < 324)) ||
				(key[3] && pacman_x == 764 && pacman_y > 396 && pacman_y < 444)) ||
				(key[3] && pacman_x == 764 && pacman_y > 596 && pacman_y < 644)) {
				pacman_x = 0;
			}

			//Call ghost AI
			chandelure.chase1(pacman_x, pacman_y, map);
			dusknoir.chase2(pacman_x, pacman_y, map);
			gengar.chase3(pacman_x, pacman_y, map);
			golurk.chase4(pacman_x, pacman_y, map);

			//Losing lives
			if (chandelure.getPacman(32, 32, pacman_x, pacman_y) == 1) {

				al_draw_text(font, al_map_rgb(255, 100, 100), 400, 420, 0, "YOU DIED!");
				pacman_x = 384;
				pacman_y = 364;
				chandelure.initGhost(324, 164, 32, 32);
				dusknoir.initGhost(364, 164, 32, 32);
				gengar.initGhost(404, 164, 32, 32);
				golurk.initGhost(444, 164, 32, 32);

				lives--;
				
			}

			if (dusknoir.getPacman(32, 32, pacman_x, pacman_y) == 1) {

				al_draw_text(font, al_map_rgb(255, 100, 100), 400, 420, 0, "YOU DIED!");
				pacman_x = 384;
				pacman_y = 364;
				chandelure.initGhost(324, 164, 32, 32);
				dusknoir.initGhost(364, 164, 32, 32);
				gengar.initGhost(404, 164, 32, 32);
				golurk.initGhost(444, 164, 32, 32);

				lives--;
			}
			
			
			if (gengar.getPacman(32, 32, pacman_x, pacman_y) == 1) {

				al_draw_text(font, al_map_rgb(255, 100, 100), 400, 420, 0, "YOU DIED!");
				pacman_x = 384;
				pacman_y = 364;
				chandelure.initGhost(324, 164, 32, 32);
				dusknoir.initGhost(364, 164, 32, 32);
				gengar.initGhost(404, 164, 32, 32);
				golurk.initGhost(444, 164, 32, 32);

				lives--;
			}
			if (golurk.getPacman(32, 32, pacman_x, pacman_y) == 1) {

				al_draw_text(font, al_map_rgb(255, 100, 100), 400, 420, 0, "YOU DIED!");
				pacman_x = 384;
				pacman_y = 364;
				chandelure.initGhost(324, 164, 32, 32);
				dusknoir.initGhost(364, 164, 32, 32);
				gengar.initGhost(404, 164, 32, 32);
				golurk.initGhost(444, 164, 32, 32);

				lives = lives--;
			}

			//Set render section to redraw
			redraw = true;
		}

		//Exit game loop if display closes
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}


		//Keypress algorithm (sets up for movement + exiting game)
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;
			}
		}
		//Key release algorithm
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		//If absolutely nothing happens, don't redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//Draw over screen before updates
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//Draw map based on array
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 21; j++)
					if (map[j][i] == 1)
						al_draw_bitmap(wall, i * 40, j * 40, 0);
					else if (map[j][i] == 0)
						al_draw_bitmap(dot, i * 40 + 18, j * 40 + 18, 0);

			//Drawing game objects
			chandelure.drawGhost1();
			dusknoir.drawGhost2();
			gengar.drawGhost3();
			golurk.drawGhost4();
			al_draw_textf(font, ALLEGRO_COLOR(al_map_rgb(100, 100, 100)), 20, 20, 0, "lives:  %d", lives);
			al_draw_bitmap(pacman, pacman_x, pacman_y, 0);
			al_flip_display();

		}
	}
	//Lose condition
	if (lives == 0) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(255, 100, 100), 400, 420, 0, "You have lost.");
		al_stop_sample_instance(instance);
		al_flip_display();
		al_rest(3);
		al_destroy_bitmap(pacman);
		al_destroy_bitmap(wall);
		al_destroy_bitmap(dot);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
	}
	//Win condition
	if (dotseaten == 198) {
		redraw = false;
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_textf(font, al_map_rgb(255, 255, 255), 420, 400, 0, "You win!");
		al_stop_sample_instance(instance);
		al_flip_display();
		win = al_load_sample("Victory.wav");
		al_play_sample(win, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		al_rest(3);
		al_destroy_bitmap(pacman);
		al_destroy_bitmap(wall);
		al_destroy_bitmap(dot);
		al_destroy_timer(timer);
		al_destroy_display(display);
		al_destroy_event_queue(event_queue);
		return 0;
	}

	//Clearing memory
	al_destroy_bitmap(pacman);
	al_destroy_bitmap(wall);
	al_destroy_bitmap(dot);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

//Collision function
int wallCollide(int x, int y, int dir, int map[21][20]) {
	int new_x1;
	int new_x2;
	int new_x3;
	int new_y1;
	int new_y2;
	int new_y3;
	if (dir == RIGHT) {
		new_x1 = x + 5 + 32;
		new_x2 = x + 5 + 32;
		new_x3 = x + 5 + 32;
		new_y1 = y;
		new_y2 = y + 32 / 2;
		new_y3 = y + 32;
	}
	else if (dir == LEFT) {
		new_x1 = x - 5;
		new_x2 = x - 5;
		new_x3 = x - 5;
		new_y1 = y;
		new_y2 = y + 32 / 2;
		new_y3 = y + 32;
	}
	else if (dir == UP) {
		new_x1 = x;
		new_x2 = x + 32 / 2;
		new_x3 = x + 32;
		new_y1 = y - 5;
		new_y2 = y - 5;
		new_y3 = y - 5;
	}
	else if (dir == DOWN) {
		new_x1 = x;
		new_x2 = x + 32 / 2;
		new_x3 = x + 32;
		new_y1 = y + 32 + 5;
		new_y2 = y + 32 + 5;
		new_y3 = y + 32 + 5;
	}
	if (map[new_y1 / 40][new_x1 / 40] == 1 || map[new_y2 / 40][new_x2 / 40] == 1 || map[new_y3 / 40][new_x3 / 40] == 1) {
		//cout << "collision!" << endl;
		return 1;
	}
	else
		return 0;
}

//Ghost functions
void Ghost::initGhost(int x, int y, int w, int h) {
	Xpos = x;
	Ypos = y;
	width = w;
	height = h;
	dir = LEFT;
}
void Ghost::drawGhost1() {
	al_draw_filled_rectangle(Xpos, Ypos, Xpos + width, Ypos + height, al_map_rgb(200, 40, 200));
}
void Ghost::drawGhost2() {
	al_draw_filled_rectangle(Xpos, Ypos, Xpos + width, Ypos + height, al_map_rgb(80, 40, 80));
}
void Ghost::drawGhost3() {
	al_draw_filled_rectangle(Xpos, Ypos, Xpos + width, Ypos + height, al_map_rgb(200, 40, 80));
}
void Ghost::drawGhost4() {
	al_draw_filled_rectangle(Xpos, Ypos, Xpos + width, Ypos + height, al_map_rgb(80, 40, 200));
}
bool Ghost::IsDead() {
	return false;
}
void Ghost::printInfo() {

}
void Ghost::killGhost() {

}
bool Ghost::getPacman(int b1x, int b1y, int b2x, int b2y){
	if ((Xpos + 32 < b2x) || //is b1 to the left of b2
		(Xpos > b2x + 32) || //is b1 to the right of b2
		(Ypos > b2y + 32) || //is b1 below b2
		(Ypos + 32 < b2y) //is b1 above b2
		) {
			return 0;

	}
	else {
		return 1;
	
	}
}

//Individual AI
void Ghost::chase1(int x, int y, int map[21][20]) {

	//Chandelure's AI, just the original code but faster

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////Chandelure

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 5;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, 4, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 5;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			Xpos -= 5;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 5;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, 3, map)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 5;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			Ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 5;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 5;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			Xpos += 5;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 5;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 5;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			Ypos += 5;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<Ypos) && !wallCollide(Xpos, Ypos, UP, map)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>Ypos) && !wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>Xpos) && !wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<Xpos) && !wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(Xpos, Ypos, UP, map)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}
	cout << "x pos " << Xpos << "  Ypos " << Ypos << endl;
}


void Ghost::chase2(int x, int y, int map[21][20]) {

	//Dusknoir's AI, mostly tries moving down but can move up
	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(Xpos, Ypos, 1, map))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(Xpos, Ypos, 2, map))
	cout << "there's a wall above me" << endl;
	if (wallCollide(Xpos, Ypos, 3, map))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(Xpos, Ypos, 4, map))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	//if ((dir == LEFT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
	//	while (!wallCollide(Xpos, Ypos, 2, map)) {
	//		// cout << "trying to move through hole above!" << endl;
	//		dir = UP;
	//		Ypos -= 4;
	//		return;
	//	}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, 4, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			Xpos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, 3, map)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			Ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	////if we're moving right and there's an opening above and pacman is above, move up
	//if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
	//	while (!wallCollide(Xpos, Ypos, 2, map)) {
	//		//   cout << "trying to move through hole above!" << endl;
	//		dir = UP;
	//		Ypos -= 4;
	//		return;
	//	}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			Xpos += 4;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			Ypos += 4;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<Ypos) && !wallCollide(Xpos, Ypos, UP, map)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>Ypos) && !wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>Xpos) && !wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<Xpos) && !wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(Xpos, Ypos, UP, map)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}
	cout << "x pos " << Xpos << "  Ypos " << Ypos << endl;
}

void Ghost::chase3(int x, int y, int map[21][20]) {

	//Gengar's AI, tries moving up but can move down
	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(Xpos, Ypos, 1, map))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(Xpos, Ypos, 2, map))
	cout << "there's a wall above me" << endl;
	if (wallCollide(Xpos, Ypos, 3, map))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(Xpos, Ypos, 4, map))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////Gengar

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, 4, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			Xpos -= 6;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == UP) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			//   cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, 3, map)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			Ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 4;
			return;
		}

//	if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			Xpos += 4;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			Ypos += 4;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<Ypos) && !wallCollide(Xpos, Ypos, UP, map)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	//if ((y>Ypos) && !wallCollide(Xpos, Ypos, DOWN, map)) {
	//	dir = DOWN;
	//	//   cout << "dir is down" << endl;
	//	return;
	//}

	//if pacman is right and there's no wall there, move right
	if ((x>Xpos) && !wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<Xpos) && !wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(Xpos, Ypos, UP, map)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}
	cout << "x pos " << Xpos << "  Ypos " << Ypos << endl;
}

void Ghost::chase4(int x, int y, int map[21][20]) {

	//Golurk's AI, Mostly sticks to the right side of the screen
	/* dirs
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << dir << endl;
	if (wallCollide(Xpos, Ypos, 1, map))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(Xpos, Ypos, 2, map))
	cout << "there's a wall above me" << endl;
	if (wallCollide(Xpos, Ypos, 3, map))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(Xpos, Ypos, 4, map))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)/////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			// cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((dir == LEFT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, 4, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == LEFT)//left
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			Xpos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	//if ((dir == UP) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
	//	while (!wallCollide(Ypos, Xpos, LEFT, map)) {
	//		//   cout << "trying to move through hole to left!" << endl;
	//		dir = LEFT;
	//		Xpos -= 4;
	//		return;
	//	}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, 3, map)) {
			// cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}
	if (dir == UP)//up
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			Ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, UP, map) && y<Ypos)
		while (!wallCollide(Xpos, Ypos, 2, map)) {
			//   cout << "trying to move through hole above!" << endl;
			dir = UP;
			Ypos -= 4;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 4;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			Xpos += 4;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	//if ((dir == DOWN) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
	//	while (!wallCollide(Xpos, Ypos, LEFT, map)) {
	//		// cout << "trying to move through hole to left!" << endl;
	//		dir = LEFT;
	//		Xpos -= 4;
	//		return;
	//	}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 4;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			Ypos += 4;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<Ypos) && !wallCollide(Xpos, Ypos, UP, map)) {
		//  cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>Ypos) && !wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//   cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>Xpos) && !wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<Xpos) && !wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(Xpos, Ypos, UP, map)) {
		// cout << "dir is up" << endl;
		dir = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(Xpos, Ypos, DOWN, map)) {
		dir = DOWN;
		//cout << "dir is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(Xpos, Ypos, RIGHT, map)) {
		dir = RIGHT;
		//  cout << "dir is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(Xpos, Ypos, LEFT, map)) {
		dir = LEFT;
		// cout << "dir is left" << endl;
		return;

	}
	cout << "x pos " << Xpos << "  Ypos " << Ypos << endl;
}
