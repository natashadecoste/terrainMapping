#ifndef SPHERE_H
#define SPHERE_H

class Sphere {

public:
	Sphere();
	~Sphere();
	int width, length;
	int destination[2];

	int locationX, locationY;
	void takeStep();
	void choosePoint();
	bool atEnd();
	bool play;
private:




};


#endif