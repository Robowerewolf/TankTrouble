#pragma once

class bullet {
private:
	int xpos;
	int ypos;
	bool dead;
	int flip;
	int name;
	int givenangle;
	int bouncecount;

public:
	int JustBounced;
	void initBullet(int n);
	bool isDead();
	void Move();
	void fire(int x, int y, double angle);
	void kill();
	int getX();
	int getY();
	void drawBullet();
	void printInfo();
	void bounce();
	int getAngle();
};