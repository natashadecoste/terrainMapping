#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "logic.h"
#include "sphere.h"


HeightMap map;

//for the camera
float camPos[] = {map.xSize/2, -map.ySize/1.5, 10*map.heightMax};
float angleX = 0.0f;
float angleY = 0.0f;


//to do the lighting for Lighting 1
float light_pos0[] =  {map.xSize + 1700,  map.ySize/2 ,  2900,  1.0};
float amb0[4]  = {0.2, 0.2, 0.2, 1}; 
float diff0[4] = {0.8, 0.8, 0.8, 1}; 
float spec0[4] = {0.5, 0.5, 0.5, 1}; 

//to do lighting for the second light
float light_pos1[] =  {-1700.0,  map.ySize/2 ,  2900 ,  1.0};
float amb1[4]  = {0.2, 0.2, 0.2, 1}; 
float diff1[4] = {0.8, 0.8, 0.8, 1}; 
float spec1[4] = {0.5, 0.5, 0.5, 1}; 


//The material.... want matte so high diffuse, low specular
float m_amb[] = {0.000, 0.392, 0.000, 1}; 
float m_diff[] = {0.133, 0.545, 0.133, 1}; 
float m_spec[] = {0.486, 0.988, 0.000, 1}; 
float shiny = 0.078125; 

//material for ball
float m_ambS[] = {0.458, 0.002, 0.040, 1}; 
float m_diffS[] = {1.000, 0.271, 0.000, 1}; 
float m_specS[] = {0.486, 0.988, 0.600, 1}; 
float shinyS = 27; 

//to use for the normals
float norm[3] = {0,0,1};	//init to the default
float norm2[3] = {0,0,1};	//for doing the norms of quads
float v1 [3] = {0,0,0};
float v2 [3] = {0,0,0};
float v3 [3] = {0,0,0,};

//states for the display
int state[3] ={1,1,1};
int lightState = 1;

//the sphere
Sphere s;

void drawSphere(){
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_ambS); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diffS); 
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_specS); 
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shinyS); 

	if(s.atEnd()){
		s.choosePoint();
		s.takeStep();
	}
	else{
		s.takeStep();
	}

	glPushMatrix();
	glTranslatef(s.locationX, s.locationY, map.terrainMap[s.locationX][s.locationY]+1);
	glColor3f(0, 0, 1);
	glutSolidSphere(2, 10, 10);
	glPopMatrix();

	glPopMatrix();


}




void init(void)
{
	glClearColor(0.118, 0.565, 1.000, 1);
	glColor3f(1, 1, 1);


	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, 1, 1, 500);


	//ENABLING THE LIGHTING YAY
	glEnable(GL_LIGHTING); 
	
	//initially lightstate 1
	glEnable(GL_LIGHT0); 
	glEnable(GL_LIGHT1); 

	//add components
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

	//add components for the second light
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1); 
    

}


void FPS(int val){
		glutPostRedisplay();
		glutTimerFunc(5,FPS,0); //1 sec
}

void normalFormTri(){
	//assign to global for current normal form
	//do things
	float U [3]= {v3[0]-v2[0], v3[1]-v2[1], v3[2]-v2[2]}; //v2 - v1
	float V [3]= {v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]}; //v3 - v1

	norm[0]=((U[1]*V[2])-(U[3]*V[1]))/1;
	norm[1]= ((U[2]*V[0])-(U[0]*V[2]))/1;
	norm[2]= ((U[0]*V[1])-(U[1]*V[0]))/1;
}

void normalFormQuad(int i, int j){
	//assign v1, v2, v3 
	v1[0] = i;
	v1[1] = j;
	v1[2] = map.terrainMap[i][j];
	v3 [0] = i;
	v3 [1] = j +1;
	v3 [2] = map.terrainMap[i][j+1];
	v2[0] = i+1;
	v2[1] = j;
	v2[2] = map.terrainMap[i+1][j];
	normalFormTri();
	norm2[0] = norm[0];
	norm2[1] = norm[1];
	norm2[2] = norm[2];

	//then the second tri
	v1[0]= v2[0];
	v1[1]= v2[1];
	v1[2]= v2[2];

	v2[0] = i+1;
	v2[1] = j+1;
	v2[2] = map.terrainMap[i+1][j+1];
	normalFormTri();

	norm[0] = (norm[0]+norm2[0])/2;
	norm[1] = (norm[1]+norm2[1])/2;
	norm[2] = (norm[2]+norm2[2])/2;

}

void vertexNormQuad(int x, int y){
	float n1 [3] = {0,0,1};
	float n2 [3] = {0,0,1};
	float n3 [3] = {0,0,1};
	float n4 [3] = {0,0,1};

	if(x ==0){ //along the left side
		if(y == 0){	//bottom left
			normalFormQuad(x+1,y);
		

		}
		else if(y == map.ySize-1){ //top left
			normalFormQuad(x,y);
		

		}
		else{//just along the left side
			normalFormQuad(x,y);
			n1 [0] = norm[0]; 
			n1 [1] = norm[1];
			n1 [2] = norm[2];

			normalFormQuad(x+1,y);
			n2 [0] = norm[0]; 
			n2 [1] = norm[1];
			n2 [2] = norm[2];


			norm[0] = ((n1[0]+n2[0])/2);
			norm[1] = ((n1[1]+n2[1])/2);
			norm[2] = ((n1[2]+n2[2])/2);

		
		}

	}//end if it along the left side
	else if( x == map.xSize-1){//it is along the right side
		if(y == 0){	//bottom right
			normalFormQuad(x+1,y-1);
		
		}
		else if(y == map.ySize-1){ //top right
			normalFormQuad(x,y-1);
		
		}
		else{//just along the right side
			normalFormQuad(x+1,y-1);
			n1 [0] = norm[0]; 
			n1 [1] = norm[1];
			n1 [2] = norm[2];

			normalFormQuad(x,y-1);
			n2 [0] = norm[0]; 
			n2 [1] = norm[1];
			n2 [2] = norm[2];

			

			norm[0] = ((n1[0]+n2[0])/2);
			norm[1] = ((n1[1]+n2[1])/2);
			norm[2] = ((n1[2]+n2[2])/2);
			


		}


	}//end if it is along the right side
	
	else if(y == 0){//along the bottom
		normalFormQuad(x+1,y-1);
		n1 [0] = norm[0]; 
		n1 [1] = norm[1];
		n1 [2] = norm[2];

		normalFormQuad(x+1,y);
		n2 [0] = norm[0]; 
		n2 [1] = norm[1];
		n2 [2] = norm[2];


		norm[0] = ((n1[0]+n2[0])/2);
		norm[1] = ((n1[1]+n2[1])/2);
		norm[2] = ((n1[2]+n2[2])/2);
		

	}//end left bound
	else if(y == map.ySize-1){ //along the top
		normalFormQuad(x,y-1);
		n1 [0] = norm[0]; 
		n1 [1] = norm[1];
		n1 [2] = norm[2];

		normalFormQuad(x,y);
		n2 [0] = norm[0]; 
		n2 [1] = norm[1];
		n2 [2] = norm[2];


		norm[0] = ((n1[0]+n2[0])/2);
		norm[1] = ((n1[1]+n2[1])/2);
		norm[2] = ((n1[2]+n2[2])/2);

		


	}//end right bound

	else{ //needs to do all 4
		normalFormQuad(x+1,y-1);
		n1 [0] = norm[0]; 
		n1 [1] = norm[1];
		n1 [2] = norm[2];

		normalFormQuad(x+1,y);
		n2 [0] = norm[0]; 
		n2 [1] = norm[1];
		n2 [2] = norm[2];

		normalFormQuad(x,y-1);
		n3 [0] = norm[0]; 
		n3 [1] = norm[1];
		n3 [2] = norm[2];

		normalFormQuad(x,y);
		n4 [0] = norm[0]; 
		n4 [1] = norm[1];
		n4 [2] = norm[2];

		norm[0] = ((n1[0]+n2[0]+n3[0]+n4[0])/4);
		norm[1] = ((n1[1]+n2[1]+n3[1]+n4[1])/4);
		norm[2] = ((n1[2]+n2[2]+n3[2]+n4[2])/4);
	}

}

void vertexNormTri(int x, int y){
	float n1 [3] = {0,0,1};
	float n2 [3] = {0,0,1};
	float n3 [3] = {0,0,1};
	float n4 [3] = {0,0,1};
	float n5 [3] = {0,0,1};
	float n6 [3] = {0,0,1};
	float n7 [3] = {0,0,1};
	//float n8 [3] = {0,0,1};


	if(x ==0){ //along the left side
		if(y == 0){	//bottom left
			v1[0] = x+1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y+1];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x+1][y+1];

			normalFormTri();
			norm[0] = (n1[0]+norm[0])/2; 
			norm[1] = (n1[1]+norm[1])/2;
			norm[2] = (n1[2]+norm[2])/2;


		

		}
		else if(y == map.ySize-1){ //top left

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x+1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second point
			v1[0] = x+1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x+1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			norm[0] = (n1[0]+norm[0])/2; 
			norm[1] = (n1[1]+norm[1])/2;
			norm[2] = (n1[2]+norm[2])/2;



		

		}
		else{//just along the left side
			v1[0] = x+1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y+1];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x+1][y+1];

			normalFormTri();
			n2[0] = norm[0]; 
			n2[1] = norm[1];
			n2[2] = norm[2];
			
			//now the bottom
			/**/

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			n3[0] = norm[0]; 
			n3[1] = norm[1];
			n3[2] = norm[2];
			//second point
			v1[0] = x+1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x+1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			norm[0] = (n1[0]+n2[0]+n3[0]+norm[0])/4; 
			norm[1] = (n1[1]+n2[1]+n3[1]+norm[1])/4;
			norm[2] = (n1[2]+n2[2]+n3[2]+norm[2])/4;
		
		}

	}//end if it along the left side
	else if( x == map.xSize-1){//it is along the right side
		if(y == 0){	//bottom right
			v1[0] = x;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x][y+1];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x-1;
			v3[1] = y;
			v3[2] = map.terrainMap[x-1][y];

			normalFormTri();
			norm[0] = (n1[0]+norm[0])/2; 
			norm[1] = (n1[1]+norm[1])/2;
			norm[2] = (n1[2]+norm[2])/2;
		
		}
		else if(y == map.ySize-1){ //top right
			v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x-1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			norm[0] = (n1[0]+norm[0])/2; 
			norm[1] = (n1[1]+norm[1])/2;
			norm[2] = (n1[2]+norm[2])/2;
		
		}
		else{//just along the right side
			v1[0] = x;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x][y+1];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x-1;
			v3[1] = y;
			v3[2] = map.terrainMap[x-1][y];

			normalFormTri();
			n2[0] = norm[0]; 
			n2[1] = norm[1];
			n2[2] = norm[2];
			
			//bottom


			v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n3[0] = norm[0]; 
			n3[1] = norm[1];
			n3[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x-1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			norm[0] = (n1[0]+n2[0]+n3[0]+norm[0])/4; 
			norm[1] = (n1[1]+n2[1]+n3[1]+norm[1])/4;
			norm[2] = (n1[2]+n2[2]+n3[2]+norm[2])/4;



		}


	}//end if it is along the right side
	
	else if(y == 0){//along the bottom
			v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x-1][y+1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x-1][y+1];


			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			n2[0] = norm[0]; 
			n2[1] = norm[1];
			n2[2] = norm[2];
			//right side

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x+1][y+1];

			normalFormTri();
			n3[0] = norm[0]; 
			n3[1] = norm[1];
			n3[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y+1];


			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			norm[0] = (n1[0]+n2[0]+n3[0]+norm[0])/4; 
			norm[1] = (n1[0]+n2[0]+n3[0]+norm[1])/4;
			norm[2] = (n1[0]+n2[0]+n3[0]+norm[2])/4;

		

	}//end left bound
	else if(y == map.ySize-1){ //along the top
			v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x-1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			n2[0] = norm[0]; 
			n2[1] = norm[1];
			n2[2] = norm[2];
			//right side

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x+1][y-1];

			normalFormTri();
			n3[0] = norm[0]; 
			n3[1] = norm[1];
			n3[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x+1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			norm[0] = (n1[0]+n2[0]+n3[0]+norm[0])/4; 
			norm[1] = (n1[0]+n2[0]+n3[0]+norm[1])/4;
			norm[2] = (n1[0]+n2[0]+n3[0]+norm[2])/4;


	}//end right bound

	else{ //needs to do all 4
		v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x-1][y-1];

			normalFormTri();
			n1[0] = norm[0]; 
			n1[1] = norm[1];
			n1[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x-1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			n2[0] = norm[0]; 
			n2[1] = norm[1];
			n2[2] = norm[2];
			//right side

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x+1][y-1];

			normalFormTri();
			n3[0] = norm[0]; 
			n3[1] = norm[1];
			n3[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y-1;
			v1[2] = map.terrainMap[x+1][y-1];


			v3[0] = x;
			v3[1] = y-1;
			v3[2] = map.terrainMap[x][y-1];

			normalFormTri();
			n4[0] = norm[0]; 
			n4[1] = norm[1];
			n4[2] = norm[2];
			
			//to do the top

			v1[0] = x-1;
			v1[1] = y;
			v1[2] = map.terrainMap[x-1][y];

			v2[0] = x;
			v2[1] = y;
			v2[2] = map.terrainMap[x][y];

			v3[0] = x-1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x-1][y+1];

			normalFormTri();
			n5[0] = norm[0]; 
			n5[1] = norm[1];
			n5[2] = norm[2];
			//second triangle
			v1[0] = x-1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x-1][y+1];


			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			n6[0] = norm[0]; 
			n6[1] = norm[1];
			n6[2] = norm[2];
			//right side

			v1[0] = x+1;
			v1[1] = y;
			v1[2] = map.terrainMap[x+1][y];


			v3[0] = x+1;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x+1][y+1];

			normalFormTri();
			n7[0] = norm[0]; 
			n7[1] = norm[1];
			n7[2] = norm[2];
			//second triangle
			v1[0] = x+1;
			v1[1] = y+1;
			v1[2] = map.terrainMap[x+1][y+1];


			v3[0] = x;
			v3[1] = y+1;
			v3[2] = map.terrainMap[x][y+1];

			normalFormTri();
			norm[0] = (n1[0]+n2[0]+n3[0]+n4[0]+n5[0]+n6[0]+n7[0]+norm[0])/8; 
			norm[1] = (n1[1]+n2[1]+n3[1]+n4[1]+n5[1]+n6[1]+n7[1]+norm[1])/8; 
			norm[2] = (n1[2]+n2[2]+n3[2]+n4[2]+n5[2]+n6[2]+n7[2]+norm[2])/8; 

	}

}


void drawTerrainTriWire(){
	glColor3f(0.753, 0.753, 0.753);
		for(int i =0; i< map.xSize; i++){
			for(int j = 0; j< map.ySize; j++ ){
					glBegin(GL_LINE_LOOP);
					v1[0] = i;
					v1[1] = j;
					v1[2] = map.terrainMap[i][j];
					glVertex3fv(v1);

					v3 [0] = i;
					v3 [1] = j +1;
					v3 [2] = map.terrainMap[i][j+1];
					glVertex3fv(v3);


					v2[0] = i+1;
					v2[1] = j;
					v2[2] = map.terrainMap[i+1][j];
					glVertex3fv(v2);
					glEnd();
					//bottom triangle
					glBegin(GL_LINE_LOOP);
					v1[0]= v2[0];
					v1[1]= v2[1];
					v1[2]= v2[2];

					v2[0] = i+1;
					v2[1] = j+1;
					v2[2] = map.terrainMap[i+1][j+1];
					glVertex3fv(v1);
					glVertex3fv(v2);					
					glVertex3fv(v3);
					glEnd();
			}
			
		}//end of both for loops

	
}

void drawTerrainTri2(){	//change this !
		glShadeModel(GL_SMOOTH);
		for(int i =0; i< map.xSize; i++){
			for(int j = 0; j< map.ySize; j++ ){
					glBegin(GL_TRIANGLES);

					vertexNormTri(i+1,j);
					glNormal3fv(norm);
					v1[0] = i+1;
					v1[1] = j;
					v1[2] = map.terrainMap[i+1][j];
					glVertex3fv(v1);
					





					vertexNormTri(i+1, j+1);
					glNormal3fv(norm);
					v2[0] = i+1;
					v2[1] = j+1;
					v2[2] = map.terrainMap[i+1][j+1];
					glVertex3fv(v2);
					

					//normalFormTri();
					vertexNormTri(i,j);
					glNormal3fv(norm);
					v3 [0] = i;
					v3 [1] = j;
					v3 [2] = map.terrainMap[i][j];
					glVertex3fv(v3);

					glEnd();

					//bottom triangle
					glBegin(GL_TRIANGLES);

					vertexNormTri(i,j);
					glNormal3fv(norm);
					v1[0] = i;
					v1[1] = j;
					v1[2] = map.terrainMap[i][j];
					glVertex3fv(v1);




					
					vertexNormTri(i+1,j+1);
					glNormal3fv(norm);
					v2[0] = i+1;
					v2[1] = j+1;
					v2[2] = map.terrainMap[i+1][j+1];
					glVertex3fv(v2);
					
					
					vertexNormTri(i,j+1);
					glNormal3fv(norm);
					v3[0] = i;
					v3[1] = j+1;
					v3[2] = map.terrainMap[i][j+1];
					glVertex3fv(v3);
					glEnd();
			}
			
		}//end of both for loops
		

	
}

void drawTerrainTri(){

		glShadeModel(GL_FLAT);
		for(int i =0; i< map.xSize; i++){
			for(int j = 0; j< map.ySize; j++ ){
					glBegin(GL_TRIANGLES);
					v1[0] = i+1;
					v1[1] = j;
					v1[2] = map.terrainMap[i+1][j];
					glVertex3fv(v1);
					


					v3 [0] = i;
					v3 [1] = j;
					v3 [2] = map.terrainMap[i][j];



					v2[0] = i+1;
					v2[1] = j+1;
					v2[2] = map.terrainMap[i+1][j+1];
					glVertex3fv(v2);
					

					normalFormTri();
					glNormal3fv(norm);
					glVertex3fv(v3);

					glEnd();

					//bottom triangle
					glBegin(GL_TRIANGLES);
					v1[0] = v3[0];
					v1[1] = v3[1];
					v1[2] = v3[2];

					v3[0] = i;
					v3[1] = j+1;
					v3[2] = map.terrainMap[i][j+1];


					glVertex3fv(v1);
					
					glVertex3fv(v2);
					normalFormTri();
					glNormal3fv(norm);
					
					glVertex3fv(v3);
					glEnd();
			}
			
		}//end of both for loops

	
}

void drawTerrainQuadWire(){
	glColor3f(0.753, 0.753, 0.753);
	for(int i =0; i< map.xSize; i++){
			for(int j = 0; j< map.ySize; j++ ){
				glBegin(GL_LINE_LOOP);
				glVertex3f(i,j,map.terrainMap[i][j]);
				glVertex3f(i,j+1,map.terrainMap[i][j+1]);
				glVertex3f(i+1,j+1,map.terrainMap[i+1][j+1]);
				glVertex3f(i+1,j,map.terrainMap[i+1][j]);
				glEnd();
			}
			
		}//end of both for loops
};


void drawTerrainQuad(){	//for drawing quads
	glShadeModel(GL_FLAT);

		for(int i =0; i< map.xSize; i++){
			glBegin(GL_QUAD_STRIP);
			
			for(int j = 0; j< map.ySize; j++ ){
					//if(j>0){

					glVertex3f(i,j,map.terrainMap[i][j]);
					normalFormQuad(i,j);
					glNormal3fv(norm);
					glVertex3f(i+1,j,map.terrainMap[i+1][j]);

				
			}
		glEnd();	
		}//end of both for loops
}

void drawTerrainQuad2(){	//for drawing with smooth
	glShadeModel(GL_SMOOTH);

		for(int i =0; i< map.xSize; i++){
			glBegin(GL_QUAD_STRIP);
			
			for(int j = 0; j< map.ySize; j++ ){
					vertexNormQuad(i,j);
					glNormal3fv(norm);
					glVertex3f(i,j,map.terrainMap[i][j]);
					vertexNormQuad(i+1,j);
					glNormal3fv(norm);
					glVertex3f(i+1,j,map.terrainMap[i+1][j]);

			}
		glEnd();	
		}//end of both for loops
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0); 
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1); 


	glPushMatrix();
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	



	gluLookAt(camPos[0], camPos[1], camPos[2], map.xSize/2,map.ySize/2,map.heightMax, 0,0,1);
    
	switch(state[2]){
		case 1: // full render
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
			if(state[1] == 1) {
				if(state[0] ==1){
					drawSphere();
							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainTri();
				}
				else{
					drawSphere();

							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainTri2();
				}
			}
			else {
				if(state[0] == 1){
					//flat shading
					drawSphere();
							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainQuad();
				}
				else{
					drawSphere();
							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainQuad2();
				}
				
			}
			glutSwapBuffers();
			break;
		case 2: //wire frame
			
			if(state[1] == 1){
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
				drawSphere();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //gl_FILL
				drawTerrainTri();
			}
			else{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
				drawSphere();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //gl_FILL
				drawTerrainQuad();
			}
			glutSwapBuffers();
			break;
		case 3://both
			if(state[1] ==1 ){
				drawSphere();
				drawTerrainTriWire();
				if(state[0]==1){
					//flat
					drawSphere();
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainTri();
				}
				else{
					drawSphere();
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
											//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainTri2();
				}

			}
			else {
				drawSphere();
				drawTerrainQuadWire();
				if(state[0] ==1){
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
											//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainQuad();
					//flat
				}
				else {
					
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //gl_FILL
							//applying the material 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_amb); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_diff); 
				    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  m_spec); 
				    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  shiny); 
					drawTerrainQuad2();

				}
				
			}
			glutSwapBuffers();
			break;

	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //gl_FILL

	
	glutSwapBuffers();
	glPopMatrix();
}



void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 'q':
		case 27:
			exit (0);
			break;

		case'r':
			//TODO add reset
			map.reset();
			map.constructCircleAlg();
			glutPostRedisplay();
			break;
		case'f':
		case'F':
			map.reset();
			map.constructFaultAlg();
			camPos[2] = map.max + 100;
			glutPostRedisplay();
			break;
/*		case'm':
		case'M':
			map.reset();
			map.constructMidpointAlg();
			camPos[2] = map.max + 100;
			glutPostRedisplay();
			break;*/
		case 'w':
		case 'W':
		//toggles wirefram
			if(state[2] == 1){	//default mode 
				state[2] ++;
				}
			else if(state[2] == 2){ //it is in wire frame mode
				state[2] ++;
				}
			else{
				state[2] = 1;	//state is 3
			}
			glutPostRedisplay();
			break;
		case 't':
		case 'T':
			state[1] = 1;
			glutPostRedisplay();
			//switch to triangles
			break;
		case 'y':
		case 'Y':
			state[1] = 2;
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			if(state[0] == 2){
				state[0] =1;
			}
			else {
				state[0] = 2;
			}
			glutPostRedisplay();
			break;
		case 'l':
		case 'L':
			switch(lightState){
				case 1:	//light are all on
					glDisable(GL_LIGHT0);
					lightState = 2;
					glutPostRedisplay();
					break;
				case 2:
					glDisable(GL_LIGHT1);
					lightState = 3;
					glutPostRedisplay();
					break;
				case 3:	//they are both disables
					glEnable(GL_LIGHT0);
					lightState = 4;
					glutPostRedisplay();
					break;
				case 4:
					glEnable(GL_LIGHT1);
					lightState = 1;
					glutPostRedisplay();
					break;




		}


	}
}

/*void mouse(int btn, int state, int x, int y){
	switch(btn){
		case GLUT_LEFT_BUTTON:
			printf("%i \n",x);
			printf("%i \n",y);
			map.createCircle(x,y);
			//map.reset();
			glutPostRedisplay();
			break;
		case GLUT_RIGHT_BUTTON:
			map.createCrater(x,y);
			glutPostRedisplay();
			break;
	}
	
}*/

void special(int key, int x, int y)
{
	/* arrow key presses move the camera */

	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0] -= 0.5;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0] += 0.5;
			break;

		case GLUT_KEY_UP:
			camPos[2] += 0.5;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] -= 0.5;
			break;
		
		case GLUT_KEY_HOME:
			camPos[1]-=0.5;
			break;

		case GLUT_KEY_END:
			camPos[1]+=0.5;
			
			break;
		case GLUT_KEY_F1:
			light_pos0[1] -= 5;
			light_pos1[1] -= 5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F2:
			light_pos0[1] += 5;
			light_pos1[1] += 5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_F3:
			light_pos0[2] += 5;
			light_pos1[2] += 5;
			glutPostRedisplay();
		break;
		case GLUT_KEY_F4:
			light_pos0[2] -= 5;
			light_pos1[2] -= 5;
			glutPostRedisplay();
		break;

	}
	glutPostRedisplay();
}






int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	s.width = map.xSize;
	s.length = map.ySize;

	map.constructCircleAlg();
	//map.print();
	//map.constructFaultAlg();
	map.printInstructions();



	
	
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Terrain - Assignment 2");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutTimerFunc(0 , FPS, 0);

	glEnable(GL_DEPTH_TEST);
	init();


	//second window for the overview


	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}
