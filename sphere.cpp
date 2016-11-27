#include "sphere.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>

using namespace std;




	
Sphere::Sphere(){
		srand (static_cast <unsigned> (time(0)));

		play = true;

		locationX = 0;
		locationY = 0;

		destination[0] = 0;
		destination[1] = 0;

		//cout<<"Made a sphere"<<endl;
}

 

Sphere::~Sphere(){
		//cout<<"destroyed"<<endl;

}

void Sphere::choosePoint(){
		destination[0] = rand() % width;
		destination[1] = rand () % length;

}

bool Sphere::atEnd(){
		if ((locationX == destination[0]) && (locationY == destination[1])){
			return true;
		}
		return false;
	}

void Sphere::takeStep(){
	if(play){
		if(locationX > destination[0]){
			locationX --;
		}
		else if(locationX < destination[0]){
			locationX ++;
		}
		else{
			//nothing
		}


		if(locationY > destination[1]){
			locationY --;
		}
		else if(locationY < destination[1]){
			locationY ++;
		}
		else{
			//nothing
		}

		
	}

	play = !play;

}

/*int main(){
	Sphere s = Sphere(10,10);
	s.choosePoint();
	cout<<s.destination[0]<<endl;
	cout<<s.destination[1]<<endl;
		s.choosePoint();
	cout<<s.destination[0]<<endl;
	cout<<s.destination[1]<<endl;
		s.choosePoint();
	cout<<s.destination[0]<<endl;
	cout<<s.destination[1]<<endl;
	s.takeStep();
	cout<<endl;
	cout<< s.locationX <<endl;
	cout<< s.locationY <<endl;
	return 1;
}
*/

