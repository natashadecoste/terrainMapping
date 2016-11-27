#include "logic.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>

using namespace std;

HeightMap::HeightMap(){
	int x,y;

	cout << "You can pick a terrain size to render, the dimensions must be between 50 and 300. "<<endl;

	
	do{
		cout<<"How wide do you want the terrain?"<<endl;
		cin >> x;

	} while (x > 300 || x < 50);

	do{
		cout<<"How long do you want the terrain?"<<endl;
		cin >> y;

	} while (y > 300 || y < 50);

	xSize = x;
	ySize = y;
	numOfCircles = (xSize+ySize);

	//heightMax = 5;
	cout<< "You can choose the maximum height for your terrain. (Keep it under 21)" <<endl;
	do{
		cout<<"Max Height: ";
		cin >> heightMax;

	} while (heightMax > 20 || heightMax < 1);

	srand (static_cast <unsigned> (time(0)));

}

HeightMap::~HeightMap(){
	//destroyer

}
/*
void HeightMap::mp(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4){
	float displacement;
	//int tx,ty,lx,ly,rx,ry,bx,by;

	displacement = rand() % 2;
	if(x2 == (x1+1)){
		//nuthin'
		cout<<"here"<<endl;
	}
	else{
		terrainMap[(x2-x1)/2][(y3-y1)/2] = (terrainMap[x1][y1]+terrainMap[x2][y2]+terrainMap[x3][y3]+terrainMap[x4][y4])/4 +displacement;
		//assigns the middle

		//top point
		terrainMap[(x2-x1)/2][y1] = (terrainMap[x1][y1]+terrainMap[(x2-x1)/2][(y3-y1)/2]+terrainMap[x2][y2])/3;
		//right point
		terrainMap[x2][(y3-y2)/2] = (terrainMap[x2][y2]+terrainMap[(x2-x1)/2][(y3-y1)/2]+terrainMap[x3][y3])/3;
		//bottom point
		terrainMap[(x2-x1)/2][y3] = (terrainMap[x4][y4]+terrainMap[(x2-x1)/2][(y3-y1)/2]+terrainMap[x3][y3])/3;
		//left point
		terrainMap[x1][(y4-y1)/2] = (terrainMap[x1][y1]+terrainMap[(x2-x1)/2][(y3-y1)/2]+terrainMap[x4][y4])/3;
	
		//mp(x1,y1,(x2-x1)/2,y1,(x2-x1)/2,(y3-y1)/2,x1,(y4-y1)/2);
		
		//mp((x2-x1)/2,y1,x2,y2,x2,(y3-y2)/2,(x2-x1)/2,(y3-y1)/2);
		
		mp(x1,(y4-y1)/2,(x2-x1)/2,(y3-y1)/2,(x2-x1)/2, y3,x4,y4);
		//mp((x2-x1)/2,(y3-y1)/2,x2,(y3-y2/2),x3,y3,(x2-x1)/2,y3);
	}
}

void HeightMap::constructMidpointAlg(){
	terrainMap[0][0] = rand() % 6;
	terrainMap[xSize-1][0] = rand() % 6;
	terrainMap[xSize-1][ySize-1] = rand() % 6;
	terrainMap[0][ySize-1] = rand() % 6;

	mp(0,0,xSize-1,0,xSize-1,ySize-1,0,ySize-1);

}
*/

void HeightMap::createCrater(int x, int y){
	int circlex, circley, circlesize;



	for (int p = 0; p < numOfCircles; p++ ){	//go through the circles to create the terrain
	circlesize = -(rand() % (((xSize+ySize)/2)/4) + 6); //if the circles are too small its just going to look flat with bumps
	circlex = x;
	circley = y;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}

		}


	}//end of the constuction
}


void HeightMap::createCircle(int x, int y){
	int circlex, circley, circlesize;



		//go through the circles to create the terrain
	circlesize = rand() % (((xSize+ySize)/2)/4) + 6; //if the circles are too small its just going to look flat with bumps
	circlex = x;
	circley = y;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}


	}//end of the constuction
}




void HeightMap::constructFaultAlg(){
	double seconds, a,b,c;//,d;
	float displacement;
	int iterations = 1800;
	max = 0;

	//cout<<seconds<<endl;
	for(int t = 0; t < iterations; t++){
		float apt[2] = {rand() % xSize, rand() % ySize };
		float bpt[2] = {rand() % xSize, rand() % ySize };

		a = (bpt[1] - apt[1]);
		b = -(bpt[0] - apt[0]);
		c = - apt[0]*(bpt[1]-apt[1]) + apt[1]*(bpt[0]-bpt[1]);

	 	displacement = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		for (int i = 0 ; i < xSize ; i++){
			for(int j=0; j<ySize; j++){

				if(a*i + b*j - c > 0){
					terrainMap[i][j] += displacement;


				}
				else{
					terrainMap[i][j] -= displacement;
	 			}

	 			if(terrainMap[i][j] > max){
	 				max = terrainMap[i][j];
	 			}


			}
		}//end for loop

	}//end all iterations for loop


}

void HeightMap::constructCircleAlg(){

	int circlex, circley, circlesize;



	for (int p = 0; p < numOfCircles; p++ ){	//go through the circles to create the terrain
	circlesize = rand() % (((xSize+ySize)/2)/4) + 6; //if the circles are too small its just going to look flat with bumps
	circlex = rand () % xSize;
	circley = rand () % ySize;


		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){
				//do something to make the heightmap (get the z (height) value)

				double distBetween = sqrt(pow((circlex-i),2) + pow((circley - j),2));//the distance between (circlex, circley) and the point (i,j)
				double pd = distBetween * 2 / circlesize;

				if (fabs(pd) <= 1.0) {
		
				terrainMap[i][j] +=  heightMax/2 + cos(pd*3.14)*heightMax/2;}

				
		
			}

		}


	}//end of the constuction
}

void HeightMap::print(){
		for(int i=0; i< xSize; i ++){
			for(int j=0; j < ySize; j ++){

				cout << terrainMap[i][j];
				cout << ", ";
		
		}	
		cout << endl;
	}
}



void HeightMap::reset(){
	for(int i=0; i< xSize; i ++){
		for(int j=0; j < ySize; j ++){
			terrainMap[i][j] = 0.0;
		}
	}

}

void HeightMap::printInstructions(){
	cout<<"Controls:"<<endl;
	cout<<"right/left	Arrows move the Camera about the Y axis"<<endl;
	cout<<"up/down		Arrows move the Camera about the X axis"<<endl;
	cout<<"HOME/END		Arrows move the Camera about the Z axis"<<endl;
	cout<<"F1/F2		Moves the cameras along the x axis (F1 in the negative direction, F2 positive)"<<endl;
	cout<<"F3/F4		Moves the cameras up and down (z axis)"<<endl;
	cout<<"'W' 		Switches between drawn planes (default), wireframe and both wire and filled polygons"<<endl;
	cout<<"'T'		Switches to triangular mesh (default)"<<endl;
	cout<<"'Y'		Switches to quad mesh"<<endl;
	cout<<"'L'		Switches the lights on and off (default both on)"<<endl;
	cout<<"'S' 		Switches between shading modes (default Flat Shading)"<<endl;
	cout<<"'F'		Switches to using the Fault Algorithm to make the terrain"<<endl;

}


/*int main(){
	HeightMap n;
	n.createCircle(5,5);
	n.print();
	return 1;
}*/