#include <iostream>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "bullet.h"

using namespace std;

bool wallCollide(int x, int y, int angle, int dir, int size, int map[64][48]);

const float FPS = 60;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int player1_SIZE = 32;
const int player2_SIZE = 32;

enum MYKEYS {//explain
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_M
};
enum MYKEYS2 {
	KEY_W, KEY_S, KEY_A, KEY_D, KEY_V
};
enum DIRECTIONS {
	RIGHT, LEFT, UP, DOWN
};


int main() {

	//the map
	int level[64][48];
	//initialize to all 0s
	for(int m = 0; m < 64; m++)
		for (int n = 0; n < 48; n++) {
			//fill in perimeter
			if (m == 0 || n == 0)
				level[m][n] = 1;
			else if (m == 63 || n == 47)
				level[m][n] = 1;
			//draw interior maze walls
			else if (m == 25 && n < 28)
				level[m][n] = 1;
			else if (m > 15 && n == 35)
				level[m][n] = 1;
			else if (m == 45 && n < 20)
				level[m][n] = 1;
			else if (m < 15 && n == 15)
				level[m][n] = 1;
			else if (m == 35 && n > 40)
				level[m][n] = 1;
			else if (m == 8 && n > 35)
				level[m][n] = 1;
			else if (m > 50 && n == 20)
				level[m][n] = 1;
			//leave rest of map blank
			else
				level[m][n] = 0;
		}
	//print for TESTING!!
	/*for (int m = 0; m < 64; m++)
		for (int n = 0; n < 48; n++)
			cout << level[m][n];*/



	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *player1 = NULL;
	ALLEGRO_BITMAP *player2 = NULL;
	float player1_x = SCREEN_W / 2.0 - player1_SIZE / 2.0;
	float player1_y = SCREEN_H / 2.0 - player1_SIZE / 2.0;
	float player2_x = SCREEN_W / 3.0 - player2_SIZE / 2.0;
	float player2_y = SCREEN_H / 3.0 - player2_SIZE / 2.0;
	bool key[5] = { false, false, false, false, false };
	bool key2[5] = { false, false, false, false, false };
	bool redraw = true;
	bool doexit = false;
	double angle = 0;
	double angle2 = 0;
	int justFired = 0;

	al_init();
	al_init_image_addon();
	al_init_primitives_addon();

	al_install_keyboard();

	timer = al_create_timer(1.0 / FPS);

	display = al_create_display(SCREEN_W, SCREEN_H);

	player1 = al_load_bitmap("tank.png");

	al_convert_mask_to_alpha(player1, al_map_rgb(255, 255, 255));

	player2 = al_load_bitmap("tank2.png");

	al_convert_mask_to_alpha(player2, al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	////instantiate (stamp class objects) for class bullet
	bullet b1;
	b1.initBullet(1);//explain
	bullet b2;
	b2.initBullet(2);
	bullet b3;
	b3.initBullet(3);
	bullet b4;
	b4.initBullet(4);
	bullet b5;
	b5.initBullet(5);
	bullet b6;
	b6.initBullet(6);
	bullet b7;
	b7.initBullet(7);
	bullet b8;
	b8.initBullet(8);
	bullet b9;
	b9.initBullet(9);
	bullet b10;
	b10.initBullet(10);

	////create a vector named ammo that can hold bullet objects
	vector <bullet> ammo;//explain
	////create an iterator for the vector
	vector <bullet> ::iterator i;

	////put the bullets into the vector
	ammo.push_back(b1);
	ammo.push_back(b2);
	ammo.push_back(b3);
	ammo.push_back(b4);
	ammo.push_back(b5);
	ammo.push_back(b6);
	ammo.push_back(b7);
	ammo.push_back(b8);
	ammo.push_back(b9);
	ammo.push_back(b10);

	while (!doexit) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//stops bullets from firing all at once
			justFired++;


			////if the bullet isn't dead, move it
			for(i = ammo.begin(); i < ammo.end(); i++)
			  if(i ->isDead() == false)
			     i->Move();

			////if the bullet hits the side of the screen, kill it
			for(i = ammo.begin(); i < ammo.end(); i++)
			  if(i->getX() > SCREEN_W || i->getX() < 0 || i->getY() > SCREEN_H || i->getY() < 0)
			i->kill();


			////check bullet collision and ricochet if hit wall
			for(i = ammo.begin(); i < ammo.end(); i++)
			 if (i->isDead() == false && wallCollide(i->getX(), i->getY(), UP, i->getAngle(), 1, level) && i->JustBounced > 10) {
			     i->bounce();
			     i->JustBounced = 0;
			  }

			//reset angle so it doesn't eventually overflow
			if (angle < 0)
				angle = 360;
			if (angle > 360)
				angle = 0;

			cout << " angle is " << (angle) << endl;

			if (key[KEY_UP] && !wallCollide(player1_x, player1_y, UP, angle, 16, level)) {
				//player1_y -= 4.0;
				player1_y += 2 * sin(3.14*angle / 180);
				player1_x += 2 * cos(3.14*angle / 180);
			}

			if (key[KEY_DOWN] && !wallCollide(player1_x, player1_y, DOWN, angle, 16, level)) {
				//player1_y += 4.0;
				player1_y -= 2 * sin(3.14*angle / 180);
				player1_x -= 2 * cos(3.14*angle / 180);
			}

			if (key[KEY_LEFT]) {
				//player1_x -= 4.0;
				angle -= 2;
			}

			if (key[KEY_RIGHT]) {
				//player1_x += 4.0;
				angle += 2;
			}

			if (key[KEY_M]) {
				//cout << "fire bullet" << endl;
				for (i = ammo.begin(); i != ammo.end(); ++i) {
				//	cout << "checking bullet" << endl;
					if (i->isDead() && justFired > 20) {
					i->fire(player1_x, player1_y, angle + 180);
				//		cout << "fired: ";
						i->printInfo();
						justFired = 0;
						break;
					}
				}
			}

			if (angle2 < 0)
				angle2 = 360;
			if (angle2 > 360)
				angle2 = 0;

			cout << " angle 2 is " << (angle2) << endl;

			if (key2[KEY_W] && !wallCollide(player2_x, player2_y, UP, angle2, 16, level)) {
				//player1_y -= 4.0;
				player2_y += 4 * sin(3.14*angle2 / 180);
				player2_x += 4 * cos(3.14*angle2 / 180);
			}

			if (key2[KEY_S] && !wallCollide(player2_x, player2_y, DOWN, angle2, 16, level)) {
				//player1_y += 4.0;
				player2_y -= 4 * sin(3.14*angle2 / 180);
				player2_x -= 4 * cos(3.14*angle2 / 180);
			}

			if (key2[KEY_A]) {
				//player1_x -= 4.0;
				angle2 -= 2;
			}

			if (key2[KEY_D]) {
				//player1_x += 4.0;
				angle2 += 2;
			}

			if (key2[KEY_V]) {
				cout << "fire bullet" << endl;
				for (i = ammo.begin(); i != ammo.end(); ++i) {
					//	cout << "checking bullet" << endl;
					if (i->isDead() && justFired > 20) {
						i->fire(player2_x, player2_y, angle2 + 180);
						//		cout << "fired: ";
						i->printInfo();
						justFired = 0;
						break;
					}
				}
			}

			redraw = true;
		}//end event timer
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_M:
				key[KEY_M] = true;
				break;

			case ALLEGRO_KEY_W:
				key2[KEY_W] = true;
				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = true;
				break;

			case ALLEGRO_KEY_A:
				key2[KEY_A] = true;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = true;
				break;

			case ALLEGRO_KEY_V:
				key2[KEY_V] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			case ALLEGRO_KEY_M:
				key[KEY_M] = false;
				break;

			case ALLEGRO_KEY_W:
				key2[KEY_W] = false;
				break;

			case ALLEGRO_KEY_S:
				key2[KEY_S] = false;
				break;

			case ALLEGRO_KEY_A:
				key2[KEY_A] = false;
				break;

			case ALLEGRO_KEY_D:
				key2[KEY_D] = false;
				break;

			case ALLEGRO_KEY_V:
				key2[KEY_V] = false;
				break;
			}
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			//draw map
			for (int m = 0; m < 64; m++)
				for (int n = 0; n < 48; n++)
					if (level[m][n] == 1)
						al_draw_filled_rectangle(m * 10, n * 10, m * 10 + 10, n * 10 + 10, al_map_rgb(0, 0, 255));

			//draw player
			al_draw_rotated_bitmap(player1, 16, 16, player1_x, player1_y, ((angle*3.14) / 180), 0);
			al_draw_rotated_bitmap(player2, 16, 16, player2_x, player2_y, ((angle2*3.14) / 180), 0);
			////draw live bullets
			for (i = ammo.begin(); i < ammo.end(); i++)
				if (i->isDead() == false)
					i->drawBullet();

			


			al_flip_display();
		}
	}

	al_destroy_bitmap(player1);
	al_destroy_bitmap(player2);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}//end main

bool wallCollide(int x, int y, int dir, int angle, int size, int map[64][48]) {
	//cout << "beginning collision function" <<endl;
	int newX;
	int newY;
	if (dir == UP) {
		newX = x - (4 + size) * cos(3.14*angle / 180);
		newY = y - (4 + size) * sin(3.14*angle / 180);
	}
	else {
		newX = x - (4 + size) * cos(3.14*angle / 180);
		newY = y - (4 + size) * sin(3.14*angle / 180);
	}

	//cout << "checking collision at: " << newX << " , " << newY <<endl;

	if (map[newX / 10][newY / 10] == 1)
		return 1;


	else return false;
}