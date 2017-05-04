
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include<iostream>
using namespace std;



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
bool Ghost::getPacman(int x, int y) {
	return false;
}


void Ghost::chase(int x, int y, int map[21][20]) {


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
			Ypos -= 2;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((dir == RIGHT) && !wallCollide(Xpos, Ypos, DOWN, map) && y>Ypos)
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			dir = DOWN;
			Ypos += 2;
			return;
		}


	if (dir == RIGHT)//right
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			Xpos += 2;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((dir == DOWN) && !wallCollide(Xpos, Ypos, LEFT, map) && x<Xpos)
		while (!wallCollide(Xpos, Ypos, LEFT, map)) {
			// cout << "trying to move through hole to left!" << endl;
			dir = LEFT;
			Xpos -= 2;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((dir == UP) && !wallCollide(Xpos, Ypos, RIGHT, map) && x>Xpos)
		while (!wallCollide(Xpos, Ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			dir = RIGHT;
			Xpos += 2;
			return;
		}

	if (dir == DOWN)//down
		while (!wallCollide(Xpos, Ypos, DOWN, map)) {
			Ypos += 2;
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
