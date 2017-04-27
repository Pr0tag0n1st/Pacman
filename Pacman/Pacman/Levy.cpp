#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include<iostream>
using namespace std;
const int RIGHT = 1;
const int LEFT = 2;
const int UP = 3;
const int DOWN = 4;

int wallCollide(int x, int y, int dir, int map[21][20]);

class Ghost {
public:
	void initGhost(int x, int y, int w, int h);
	void drawGhost();
	bool IsDead();
	void printInfo();
	void killGhost();
	bool Collision(int b1x, int b1y, int b2x, int b2y);
	void Chase();
	bool getPacman(int x, int y);
private:
	int hitwall;
	int Xpos;
	int Ypos;
	int direction;
	int speed;
	bool dead;
	int Color;
};

int main() {
	ALLEGRO_DISPLAY*display = NULL;
	ALLEGRO_BITMAP*pacman = NULL;
	ALLEGRO_BITMAP*wall = NULL;
	ALLEGRO_BITMAP*dot = NULL;
	ALLEGRO_SAMPLE*bite = NULL;
	ALLEGRO_SAMPLE*background = NULL;
	ALLEGRO_SAMPLE_INSTANCE*instance = NULL;
	ALLEGRO_EVENT_QUEUE*event_queue = NULL;
	ALLEGRO_TIMER*timer = NULL;

	int pacman_x = 384;
	int pacman_y = 364;
	bool doexit = false;
	bool key[4]{ false, false, false, false };
	bool redraw = true;
	al_init();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(5);
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
	al_install_keyboard();
	timer = al_create_timer(.02);
	event_queue = al_create_event_queue();
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
	al_set_target_bitmap(pacman);
	al_clear_to_color(al_map_rgb(255, 255, 0));
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(200, 200, 255));
	al_set_target_bitmap(dot);
	al_clear_to_color(al_map_rgb(255, 255, 0));
	al_set_target_bitmap(al_get_backbuffer(display));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	Ghost chandelure;
	chandelure.initGhost(324, 164, 32, 32);
	Ghost dusknoir;
	dusknoir.initGhost(364, 164, 32, 32);
	Ghost gengar;
	gengar.initGhost(404, 164, 32, 32);
	Ghost golurk;
	golurk.initGhost(444, 164, 32, 32);
	//tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && wallCollide(pacman_x, pacman_y, UP, map) == 0) {
				pacman_y -= 4.0;

			}

			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && wallCollide(pacman_x, pacman_y, DOWN, map) == 0) {
				pacman_y += 4.0;
			}
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && wallCollide(pacman_x, pacman_y, LEFT, map) == 0) {
				pacman_x -= 4.0;
			}

			//if the left button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && wallCollide(pacman_x, pacman_y, RIGHT, map) == 0) {
				pacman_x += 4.0;
			}


			if (map[(pacman_y + 16) / 40][(pacman_x + 16) / 40] == 0) {
				map[(pacman_y + 16) / 40][(pacman_x + 16) / 40] = 2;
				bite = al_load_sample("chomp.wav");
				al_play_sample(bite, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}



			if (((key[0] && pacman_x > 220 && pacman_x < 260 && pacman_y == 0) ||
				(key[0] && pacman_x > 360 && pacman_x < 408 && pacman_y == 0)) ||
				(key[0] && pacman_x > 520 && pacman_x < 560 && pacman_y == 0))
			{
				pacman_y = 816;
			}
			if (((key[1] && pacman_x > 220 && pacman_x < 260 && pacman_y == 816) ||
				(key[1] && pacman_x > 360 && pacman_x < 408 && pacman_y == 816)) ||
				(key[1] && pacman_x > 520 && pacman_x < 560 && pacman_y == 816)) {
				pacman_y = 0;
			}
			if ((((key[2] && pacman_x == 0 && pacman_y > 160 && pacman_y < 200) ||
				(key[2] && pacman_x == 0 && pacman_y > 280 && pacman_y < 320)) ||
				(key[2] && pacman_x == 0 && pacman_y > 400 && pacman_y < 440)) ||
				(key[2] && pacman_x == 0 && pacman_y > 600 && pacman_y < 640)) {
				pacman_x = 764;
			}
			if ((((key[3] && pacman_x == 764 && pacman_y > 160 && pacman_y < 200) ||
				(key[3] && pacman_x == 764 && pacman_y > 280 && pacman_y < 320)) ||
				(key[3] && pacman_x == 764 && pacman_y > 400 && pacman_y < 440)) ||
				(key[3] && pacman_x == 764 && pacman_y > 600 && pacman_y < 640)) {
				pacman_x = 0;
			}
			redraw = true;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

				//if the left key has been pressed
			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;
			}
		}
		//has something been released on the keyboard?
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

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 21; j++)
					if (map[j][i] == 1)
						al_draw_bitmap(wall, i * 40, j * 40, 0);
					else if (map[j][i] == 0)
						al_draw_bitmap(dot, i * 40 + 18, j * 40 + 18, 0);


			al_draw_bitmap(pacman, pacman_x, pacman_y, 0);
			cout << pacman_x << "," << pacman_y << endl;
			al_flip_display();


		}
	}

	al_destroy_bitmap(pacman);
	al_destroy_bitmap(wall);
	al_destroy_bitmap(dot);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
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
		cout << "collision!" << endl;
		return 1;
	}
	else
		return 0;
}

void initGhost(int x, int y, int w, int h) {
	int Xpos = x;
	int Ypos = y;
	int width = w;
	int height = h;

}
void drawGhost() {

}
bool IsDead() {
	return false;
}
void printInfo() {

}
void killGhost(){

}
bool Collision(int b1x, int b1y, int b2x, int b2y) {
	return false;
}
void Chase() {

}
bool getPacman(int x, int y) {
	return false;
}