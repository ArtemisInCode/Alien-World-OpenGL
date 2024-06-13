//  ========================================================================
//  COSC363: Computer Graphics (2023);  University of Canterbury.
//  FILE NAME: main.cpp
//
//  Use left/right arrow keys to rotate camera left/right
//  Use up/down arrow keys to move camera forward/back
//  ========================================================================
#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <cmath> 
#include <climits>
#include <list>
#include <GL/freeglut.h>
#include <random>
#include "loadTGA.h"
#include "loadBMP.h"
using namespace std;

// https://www.desmos.com/calculator/imu6gndlfx for the spotlight math


//--Globals ---------------------------------------------------------------

//Camera parameters

float angle = 180;
float look_x;
float look_y = 5; 
float look_z;
float eye_x = 0;
float eye_y = 3;
float eye_z = -30;  

// Environment set up
float skyWidth = 300;
float fWidth = 75; // Floor width/length

GLUquadric *q;

int tick = 0;		//Timer counter

float noseAng = 180;
float bridgeAngle = 0;
float rocketHgt = 0;
float meltRad = 0.0;

bool lightFlip = true;
int16_t lightSwitch = 0;
float rL1r = 1.0;
float rL1g = 0.0;
float rL2r = 1.0;
float rL2g = 0.0;
float rL3r = 1.0;
float rL3g = 0.0;
float rL4r = 1.0;
float rL4g = 0.0;
float rL5r = 1.0;
float rL5g = 0.0;
float rL6r = 1.0;
float rL6g = 0.0;
float rL7r = 1.0;
float rL7g = 0.0;
float rL8r = 1.0;
float rL8g = 0.0;
float rL9r = 1.0;
float rL9g = 0.0;

bool takeoff = false;

float droneWidth = 50;
float droneX = 0;
float droneZ = 0;
float droneDir = 0;
float droneSweep = 0.0;
bool droneSUp = true;
int leg = 1;
bool legFirst = true;

float periphsX[150];
float periphsZ[150];
float periphsS[150];

// Aliens
float alienAng = 0;
bool aUp = true;
float alien1X = 5;
float alien1Y = 24;
float alienTheta = 0;
float alienDirUp = true;


GLuint texId[15];

float white[4]  = {1.0, 1.0, 1.0, 1.0};
float grey[4] = {0.2, 0.2, 0.2, 1.0};
float yellow[4] = {1.0, 1.0, 0.0, 1.0};

// Sweep curve vertices
const int N = 75;  // Total number of vertices on the base curve

float vx_init[N] = { 12.5, 13, 13.25, 13.75, 14, 14.25, 14.5, 11, 9.5, 8, 6, 6.25, 7, 7.5, 8, 8.3, 8.6, 8.8, 8.9, 9, 8.9, 8.8, 8.6, 8.3, 8, 7.4, 6.5, 6, 4.8, 3, 2.5, 4, 5, 5.5, 5.8, 6, 6.3, 6.5, 6.6, 6.7, 6.7, 6.7, 6.7, 6.7, 6.8, 6.9, 7, 7.3, 7.7, 8.2, 8.8, 9.2, 9.7, 10.1, 10.6, 10.9, 11.2, 11.3, 11.1, 10, 8.8, 6, 5, 4, 3.7, 3.5, 3.4, 3.3, 3.2, 3.1, 3, 2.5, 1.7, 1, 0};
float vy_init[N] = { 0, 1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 ,28 ,29 ,30 ,31 ,32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,41, 42, 43 ,44 ,45 ,46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 64};


// float nx_init[N] = { 0.8944271909999159, 0.9363291775690445, 0.9363291775690445, 0.9363291775690445, 0.9701425001453319, 0.9701425001453319, 0.0, -0.3713906763541037, -0.5547001962252291, -0.49613893835683387, 0.0, 0.8944271909999159, 0.847998304005088, 0.8944271909999159, 0.9284766908852592, 0.9578262852211515, 0.9701425001453319, 0.9889363528682973, 0.9950371902099893, 1.0, 0.9950371902099893, 0.9889363528682973, 0.9701425001453319, 0.9578262852211515, 0.9119215051751063, 0.8, 0.8192319205190405, 0.7619393177594593, 0.5547001962252291, 0.6561787149247866, 0.8944271909999159, 0.6246950475544243, 0.8, 0.9284766908852594, 0.9701425001453319, 0.9701425001453319, 0.9701425001453319, 0.9889363528682975, 0.9950371902099893, 0.9987523388778446, 1.0, 1.0, 1.0, 0.9987523388778446, 0.9950371902099893, 0.9950371902099893, 0.9805806756909203, 0.9438583563660174, 0.9119215051751065, 0.8762159086766469, 0.8944271909999159, 0.9119215051751067, 0.9119215051751063, 0.9119215051751063, 0.9284766908852592, 0.9578262852211515, 0.9805806756909201, 0.9987523388778446, 0.8384436163006369, 0.6561787149247869, 0.4472135954999579, 0.46574643283262224, 0.7071067811865475, 0.838443616300637, 0.9701425001453319, 0.9889363528682975, 0.9950371902099893, 0.9950371902099893, 0.9950371902099893, 0.9950371902099893, 0.9578262852211513, 0.838443616300637, 0.8, 0.0};

// float ny_init[N] = { -0.4472135954999579, -0.3511234415883917, -0.3511234415883917, -0.3511234415883917, -0.24253562503633297, -0.24253562503633297, 1.0, 0.9284766908852594, 0.8320502943378437, 0.8682431421244593, 1.0, -0.4472135954999579, -0.52999894000318, -0.4472135954999579, -0.371390676354104, -0.2873478855663453, -0.24253562503633297, -0.14834045293024495, -0.09950371902099857, 0.0, 0.09950371902099857, 0.14834045293024495, 0.24253562503633297, 0.2873478855663453, 0.410364677328798, 0.6, 0.5734623443633284, 0.6476484200955405, 0.8320502943378437, 0.7546055221635046, -0.4472135954999579, -0.7808688094430304, -0.6, -0.37139067635410367, -0.24253562503633297, -0.24253562503633297, -0.24253562503633297, -0.14834045293024453, -0.09950371902099901, -0.049937616943892496, 0.0, 0.0, 0.0, -0.04993761694389205, -0.09950371902099901, -0.09950371902099901, -0.1961161351381838, -0.33035042472810616, -0.41036467732879767, -0.481918749772156, -0.4472135954999579, -0.4103646773287974, -0.410364677328798, -0.410364677328798, -0.371390676354104, -0.2873478855663453, -0.19611613513818418, 0.04993761694389205, 0.5449883505954143, 0.7546055221635045, 0.8944271909999159, 0.8849182223819825, 0.7071067811865475, 0.5449883505954141, 0.24253562503633297, 0.14834045293024475, 0.09950371902099901, 0.09950371902099879, 0.09950371902099879, 0.09950371902099901, 0.28734788556634544, 0.5449883505954141, 0.6, 1.0};

float nx_init[N] = {0.894427, 0.936329, 0.936329, 0.936329, 0.970143, 0.970143, 0, -0.371391, -0.5547, -0.496139, 0, 0.894427, 0.847998, 0.894427, 0.928477, 0.957826, 0.970143, 0.988936, 0.995037, 1, 0.995037, 0.988936, 0.970143, 0.957826, 0.911922, 0.8, 0.819232, 0.761939, 0.5547, 0.656179, 0.894427, 0.624695, 0.8, 0.928477, 0.970143, 0.970143, 0.970143, 0.988936, 0.995037, 0.998752, 1, 1, 1, 0.998752, 0.995037, 0.995037, 0.980581, 0.943858, 0.911922, 0.876216, 0.894427, 0.911922, 0.911921, 0.911921, 0.928477, 0.957826, 0.980581, 0.998752, 0.838444, 0.656179, 0.447214, 0.465746, 0.707107, 0.838444, 0.970143, 0.988936, 0.995037, 0.995037, 0.995037, 0.995037, 0.957826, 0.838444, 0.8, 0, -0.707107 };

float ny_init[N] = {-0.447214, -0.351123, -0.351123, -0.351123, -0.242536, -0.242536, 1, 0.928477, 0.83205, 0.868243, 1, -0.447214, -0.529999, -0.447214, -0.371391, -0.287348, -0.242536, -0.14834, -0.0995036, 0, 0.0995036, 0.14834, 0.242536, 0.287348, 0.410365, 0.6, 0.573462, 0.647648, 0.83205, 0.754606, -0.447214, -0.780869, -0.6, -0.371391, -0.242536, -0.242536, -0.242536, -0.14834, -0.0995036, -0.0499376, 0, 0, 0, -0.0499378, -0.0995039, -0.0995036, -0.196116, -0.33035, -0.410365, -0.481919, -0.447214, -0.410365, -0.410365, -0.410365, -0.37139, -0.287348, -0.196116, 0.0499373, 0.544988, 0.754606, 0.894427, 0.884918, 0.707107, 0.544988, 0.242536, 0.14834, 0.0995037, 0.0995037, 0.0995037, 0.0995037, 0.287348, 0.544988, 0.6, 1, 0.707107 };

std::random_device rd;
std::mt19937 gen(rd());


struct particle		//A particle 
{
	int t;			//Life time  (0 - 200)
	float col;		//Color  (0 - 1)
	float size;		//Size   (5 - 25)
	float pos[3];	//Position
	float vel[3];	//Velocity
};

list<particle> snowParList;	//List of particles
list<particle> fireParList;	//List of particles

//--------Textures----------------------------
void loadTextures(void) {
    glGenTextures(15, texId);

    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("../skybox/frost_lf.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("../skybox/frost_ft.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("../skybox/frost_rt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("../skybox/frost_bk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("../skybox/frost_up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("../skybox/frost_dn.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, texId[6]);
    loadTGA("../textures/gound.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glBindTexture(GL_TEXTURE_2D, texId[7]);
    loadTGA("../textures/line_metal.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glBindTexture(GL_TEXTURE_2D, texId[8]);
    loadTGA("../textures/rocks_3.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[9]);
    loadTGA("../textures/grate.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[10]);
    loadTGA("../textures/tunnel_corners.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glBindTexture(GL_TEXTURE_2D, texId[11]);
    loadBMP("../textures/Glow.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, texId[12]);
    loadTGA("../textures/line_conc.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glBindTexture(GL_TEXTURE_2D, texId[13]);
    loadTGA("../textures/lake.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glBindTexture(GL_TEXTURE_2D, texId[14]);
    loadTGA("../textures/snow_2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//--------SKYBOX----------------------------
void skybox() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);

    ////////////////////// LEFT ///////////////////////

    glBindTexture(GL_TEXTURE_2D, texId[0]);
	
	glBegin(GL_QUADS);
    glNormal3f(1.0, 0.0, 0.0);

		
		glTexCoord2f(1.0, 0.0);        
        glVertex3f(-skyWidth, skyWidth, skyWidth);
		glTexCoord2f(1.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, skyWidth);
		glTexCoord2f(0.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, -skyWidth);
		glTexCoord2f(0.0, 0.0);        
        glVertex3f(-skyWidth, skyWidth, -skyWidth);
		
	glEnd();

    ////////////////////// FRONT ///////////////////////

    glBindTexture(GL_TEXTURE_2D, texId[1]);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, 1.0);
    
		
		glTexCoord2f(1.0, 0.0);        
        glVertex3f(-skyWidth, skyWidth, -skyWidth);
		glTexCoord2f(1.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, -skyWidth);
		glTexCoord2f(0.0, 1.0);        
        glVertex3f(skyWidth, -skyWidth, -skyWidth);
		glTexCoord2f(0.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, -skyWidth);
		
	glEnd();

    ////////////////////// RIGHT //////////////////////

    glBindTexture(GL_TEXTURE_2D, texId[2]);
	
	glBegin(GL_QUADS);
    glNormal3f(-1.0, 0.0, 0.0);

		glTexCoord2f(1.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, -skyWidth);
		glTexCoord2f(1.0, 1.0);        
        glVertex3f(skyWidth, -skyWidth, -skyWidth);
		glTexCoord2f(0.0, 1.0);        
        glVertex3f(skyWidth, -skyWidth, skyWidth);
		glTexCoord2f(0.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, skyWidth);
	
	glEnd();

    ////////////////////// BACK ///////////////////////

    glBindTexture(GL_TEXTURE_2D, texId[3]);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 0.0, -1.0);

		glTexCoord2f(1.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, skyWidth);
		glTexCoord2f(1.0, 1.0);        
        glVertex3f(skyWidth, -skyWidth, skyWidth);
		glTexCoord2f(0.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, skyWidth);
		glTexCoord2f(0.0, 0.0);        
        glVertex3f(-skyWidth, skyWidth, skyWidth);
	
    glEnd();


    ///////////////////////// UP ///////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	
	glBegin(GL_QUADS);
    glNormal3f(0.0, -1.0, 0.0);
	
		glTexCoord2f(1.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, -skyWidth);
        glTexCoord2f(0.0, 0.0);        
        glVertex3f(skyWidth, skyWidth, skyWidth);
        glTexCoord2f(0.0, 1.0);        
        glVertex3f(-skyWidth, skyWidth, skyWidth);
        glTexCoord2f(1.0, 1.0);        
        glVertex3f(-skyWidth, skyWidth, -skyWidth);
    
    glEnd();

    /////////////////////// DOWN //////////////////////////
    
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    
    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
		
		glTexCoord2f(1.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, -skyWidth);
        glTexCoord2f(0.0, 1.0);        
        glVertex3f(-skyWidth, -skyWidth, skyWidth);
        glTexCoord2f(0.0, 0.0);        
        glVertex3f(skyWidth, -skyWidth, skyWidth);
        glTexCoord2f(1.0, 0.0);        
        glVertex3f(skyWidth, -skyWidth, -skyWidth);
	
	glEnd();
	
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glDisable(GL_TEXTURE_2D);
}


// Snow

//------Draws a single particle as two texture mapped quads-----
void drawSnowParticle(float col, float size, float px, float py, float pz)
{
	glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glColor3f(col, col, col);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[11]);

	glPushMatrix();
		glTranslatef(px, py, pz);
		glScalef(size, size, size);

		glBegin(GL_QUADS);
		//A quad on the xy-plane
			glTexCoord2f(0, 0);
			glVertex3f(-0.5, -0.5, 0);
			glTexCoord2f(1, 0);
			glVertex3f(0.5, -0.5, 0);
			glTexCoord2f(1, 1);
			glVertex3f(0.5, 0.5, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-0.5, 0.5, 0);

		//A quad on the yz-plane
			glTexCoord2f(0, 0);
			glVertex3f(0, -0.5, -0.5);
			glTexCoord2f(1, 0);
			glVertex3f(0, 0.5, -0.5);
			glTexCoord2f(1, 1);
			glVertex3f(0, 0.5, 0.5);
			glTexCoord2f(0, 1);
			glVertex3f(0, -0.5, 0.5);
		glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}


//------Creates a new particle and initializes its data fields-----
void newSnowParticle() {

    std::uniform_int_distribution<> distribP(-fWidth, fWidth);
    std::uniform_int_distribution<> distribV(-0.03, 0.03);
	particle p = { 0 };

	p.pos[0] = (float) distribP(gen);
	p.pos[1] = 100;   
	p.pos[2] = (float) distribP(gen);

	p.vel[0] = (float) distribV(gen);;
	p.vel[1] = -0.3;
	p.vel[2] = (float) distribV(gen);

	p.col = 1;
	p.size = 1;

	snowParList.push_back(p);
}


//-----Updates the particle queue-------
void updateSnowQueue()
{
	const int LIFETIME = 500;
	list<particle>::iterator it;
	particle p;
	int tval;
	float delta;
	//Remove particles that have passed lifetime

	if (!snowParList.empty())
	{
		p = snowParList.front();
		if (p.t > LIFETIME) snowParList.pop_front();
	}

	for (it = snowParList.begin(); it != snowParList.end(); it++)
	{
		tval = it->t;
		it->t = tval + 1;
		delta = (float)tval / (float)LIFETIME;

		for (int i = 0; i < 3; i++)
			(it->pos[i]) += it->vel[i];

		it->col = 1 - delta;		// (1 - 0)
	}

    if(tick % 1 == 0) newSnowParticle();   //Create a new particle every sec.
}

// Fire

//------Draws a single particle as two texture mapped quads-----
void drawFireParticle(float col, float size, float px, float py, float pz)
{
	glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0, col, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[11]);

	glPushMatrix();
		glTranslatef(px, py, pz);
		glScalef(size, size, size);

		glBegin(GL_QUADS);
		//A quad on the xy-plane
			glTexCoord2f(0, 0);
			glVertex3f(-0.5, -0.5, 0);
			glTexCoord2f(1, 0);
			glVertex3f(0.5, -0.5, 0);
			glTexCoord2f(1, 1);
			glVertex3f(0.5, 0.5, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-0.5, 0.5, 0);

		//A quad on the yz-plane
			glTexCoord2f(0, 0);
			glVertex3f(0, -0.5, -0.5);
			glTexCoord2f(1, 0);
			glVertex3f(0, 0.5, -0.5);
			glTexCoord2f(1, 1);
			glVertex3f(0, 0.5, 0.5);
			glTexCoord2f(0, 1);
			glVertex3f(0, -0.5, 0.5);
		glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

//------Creates a new particle and initializes its data fields-----
void newFireParticle()
{
    std::uniform_int_distribution<> distribP(-2, 2);
    std::uniform_int_distribution<> distribPZ(-4, 4);
    std::uniform_int_distribution<> distribV(-0.03, 0.03);
	particle p = { 0 };

	p.pos[0] = (float) distribP(gen) + 5;
	p.pos[1] = rocketHgt;    //This point is at the middle of the engines
	p.pos[2] = (float) distribPZ(gen) + 15;

	p.vel[0] = (float) distribV(gen);
	p.vel[1] = -0.4;
	p.vel[2] = (float) distribV(gen);

	p.col = 1;
	p.size = 3;

	fireParList.push_back(p);
}


//-----Updates the particle queue-------
void updateFireQueue()
{
	const int LIFETIME = 10;
	list<particle>::iterator it;
	particle p;
	int tval;
	float delta;
	//Remove particles that have passed lifetime

	if (!fireParList.empty())
	{
		p = fireParList.front();
		if (p.t > LIFETIME) fireParList.pop_front();
	}

	for (it = fireParList.begin(); it != fireParList.end(); it++)
	{
		tval = it->t;
		it->t = tval + 1;
		delta = (float)tval / (float)LIFETIME;

		for (int i = 0; i < 3; i++)
			(it->pos[i]) += it->vel[i];

		it->size = delta * 7 + 3;	//(3 - 10)
		it->col = 1 - delta;		// (1 - 0)
	}

    if(tick % 1 == 0) newFireParticle();   //Create a new particle every sec.
}

// Aliens

void drawAlien(void) {
    glPushMatrix();
    glColor3f(0.0, 0.8, 0.53);

    //Head
	glPushMatrix();
		glTranslatef(0, 1.8, 0);
        glScalef(0.4, 0.4, 0.4);
		glutSolidCube(1);
	glPopMatrix();

	//Torso
	glPushMatrix();
		glTranslatef(0, 1.35, 0);
		glScalef(0.4, 0.5, 0.3);
		glutSolidCube(1);
	glPopMatrix();

	//Right leg
	glPushMatrix();
		glTranslatef(-0.1, 1, 0);
		glRotatef(-alienAng, 1, 0, 0);
		glTranslatef(0.1, -1, 0);
		glTranslatef(-0.1, 0.55, 0);
		glScalef(0.1, 1.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();

	//Left leg
	glPushMatrix();
		glTranslatef(0.1, 1, 0);
		glRotatef(alienAng, 1, 0, 0);
		glTranslatef(-0.1, -1, 0);
		glTranslatef(0.1, 0.55, 0);
		glScalef(0.1, 1.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();

	//Right arm
	glPushMatrix();
		glTranslatef(-0.25, 1.6, 0);
		glRotatef(alienAng, 1, 0, 0);
		glTranslatef(0.25, -1.6, 0);
		glTranslatef(-0.25, 1.15, 0);
		glScalef(0.1, 1.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();

	//Left arm
	glPushMatrix();
		glTranslatef(0.25, 1.6, 0);
		glRotatef(-alienAng, 1, 0, 0);
		glTranslatef(-0.25, -1.6, 0);
		glTranslatef(0.25, 1.15, 0);
		glScalef(0.1, 1.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();

    

    glPopMatrix();
}


//--------Rocket---------------------------
//Draws the body of the rocket
void drawBody(void) {
    glPushMatrix();
    glTranslatef(0, 22, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 2.0, 2.0, 20.0, 20, 10);
    glPopMatrix();
}

//Draws the head of the rocket
void drawNose(void) {
    glPushMatrix();
    glTranslatef(0, 22, 0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 2.0, 1.0, 2.0, 20.0, 5.0);
    glPopMatrix();
}

void drawNoseTip(void) {
    glPushMatrix();
    glTranslatef(0, 23, -1);
    glTranslatef(1.0, 1.0, 0.0);
    glRotatef(noseAng, 1, 0, 0);
    glTranslatef(-1.0, -1.0, 0.0);
    gluCylinder(q, 1.0, 0.1, 2.0, 20.0, 5.0);
    glPopMatrix();
}

void drawRLights(void) {
    glPushMatrix();
    
    //Light1
    glColor3f(rL1r, rL1g, 0.0);
    glTranslatef(1.8, 20.0, 0.0);
    glutSolidSphere(0.4, 20, 3);
    
    
    //Light2
    glColor3f(rL2r, rL2g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);
    
    //Light3
    glColor3f(rL3r, rL3g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);

    //Light4
    glColor3f(rL4r, rL4g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);

    //Light5
    glColor3f(rL5r, rL5g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);
    
    //Light6
    glColor3f(rL6r, rL6g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);
    
    //Light7
    glColor3f(rL7r, rL7g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);

    //Light8
    glColor3f(rL8r, rL8g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);

    //Light9
    glColor3f(rL9r, rL9g, 0.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutSolidSphere(0.4, 20, 3);
    
    glPopMatrix();
}


void drawBoostersBody(void) {
    glPushMatrix();
    
    glPushMatrix();
    glTranslatef(0, 17, 3);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 1.0, 1.0, 15.0, 20, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 17, -3);
    glRotatef(90, 1, 0, 0);
    gluCylinder(q, 1.0, 1.0, 15.0, 20, 1);
    glPopMatrix();

    glPopMatrix();
}

void drawBoostersNose(void) {
    glPushMatrix();

    glPushMatrix();
    glTranslatef(0, 17, 3);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.1, 2.0, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 17, -3);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.1, 2.0, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

void drawBoostersEngine(void) {
    glPushMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 3);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.2, 3.0, 10.0, 4.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -3);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.2, 3.0, 10.0, 4.0);
    glPopMatrix();

    glPopMatrix();
}


//Draws engines of the rocket
void drawEngines(void) {
    
    glPushMatrix();
    
    glPushMatrix();
    glTranslatef(0.7, 0, 0.7);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.0, 3.0, 10.0, 4.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7, 0, -0.7);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.0, 3.0, 10.0, 4.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7, 0, -0.7);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.0, 3.0, 10.0, 4.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7, 0, 0.7);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 1.0, 0.0, 3.0, 10.0, 4.0);;
    glPopMatrix();

    glPopMatrix();
}

void drawBoosters(void) {
    glPushMatrix();
    
    glColor3f(0.5, 0.5, 0.5);
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    drawBoostersBody();
    drawBoostersNose();
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    glColor3f(1.0, 1.0, 1.0);
    drawBoostersEngine();
    glPopMatrix();
}

void walkingAlien(void) {
    glPushMatrix();

    glTranslatef(alien1X, alien1Y, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    drawAlien();

    glPopMatrix();
}

void drawRocket(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);

    glTranslatef(5.0, rocketHgt, 0.0);
    glBindTexture(GL_TEXTURE_2D, texId[14]);
    glColor3f(0.5, 0.5, 0.5);
    drawBody();
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    drawNose();
    drawNoseTip();
    drawRLights();
    glPushMatrix();
    glTranslatef(-3.6, 0.0, 0.0);
    drawRLights();
    glPopMatrix();
    drawBoosters();
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    glColor3f(1.0, 1.0, 1.0);
    drawEngines();
    glPushMatrix();
    glTranslatef(-10.0, .0, 0.0);
    walkingAlien();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


//----------Tower -------------------

void drawTowerBody(void) {

    float toRadians = M_PI / 180.0;   //Conversion from degrees to radians
	float angStep = 10.0 * toRadians;  //Rotate base curve in 10 deg steps
	int nSlices = 36;				  //36 slices at 10 deg intervals

	float vx[N], vy[N], vz[N];   //vertex positions
	float wx[N], wy[N], wz[N];
	float nx[N], ny[N], nz[N];   //normal vectors
	float mx[N], my[N], mz[N];

	float dist = 0;

	for (int i = 0; i < N; i++)		//Initialize data everytime the frame is refreshed
	{
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = 0;
		nx[i] = nx_init[i];
		ny[i] = ny_init[i];
		nz[i] = 0;
	}

    for (int j = 0; j < nSlices; j++)
	{
		for (int i = 0; i < N; i++)
		{								// Transform things
			wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
			wy[i] = vy[i];
			wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];
			mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
			my[i] = ny[i];
			mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
		}

		glBegin(GL_QUAD_STRIP);
		for (int i = 0; i < N; i++)
		{
			glNormal3f(nx[i], ny[i], nz[i]);
			glTexCoord2f((float)j/nSlices, (float)5*i/(N-1));
			glVertex3f(vx[i], vy[i], vz[i]);
			glNormal3f(mx[i], my[i], mz[i]);
			glTexCoord2f((float)(j+1)/nSlices, (float)5*i/(N-1));
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();

		for (int i = 0; i < N; i++)    //Update vertices
		{
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
			nx[i] = mx[i];
			ny[i] = my[i];
			nz[i] = mz[i];
		}
	}
    glDisable(GL_TEXTURE_2D);
}

void drawTanks(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[12]);
    glTranslatef(-20, 0.0, -10.0);
    glColor3f(0.7, 0.7, 0.7);
    glPushMatrix();
    glRotatef(-90, 1, 0.0, 0);
    gluCylinder(q, 2.0, 2.0, 10.0, 20.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.0, 0.0, 2.0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 2.0, 2.0, 20.0, 20.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.0, 0, 2.0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 2.0, 2.0, 15.0, 20.0, 10.0);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


void drawTBridge(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[9]);
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(6.0, 24.0, 0.0);
    glTranslatef(-3.0, -0.5, -1.0);
    glRotatef(-bridgeAngle, 0.0, 0.0, 1.0);
    glTranslatef(3.0, 0.5, 1.0);

    glBegin(GL_QUADS);

    // Front wall
        glTexCoord2f(0,1);
        glVertex3f(-3.0, 1.0, 1.0);
        glTexCoord2f(0,0);
        glVertex3f(-3.0, 0.0, 1.0);
        glTexCoord2f(1,0);
        glVertex3f(3.0, 0.0, 1.0);
        glTexCoord2f(1,1);
        glVertex3f(3.0, 1.0, 1.0);

    // Back wall
        glTexCoord2f(0,1);
        glVertex3f(-3.0, 1.0, -1.0);
        glTexCoord2f(0,0);
        glVertex3f(-3.0, 0.0, -1.0);
        glTexCoord2f(1,0);
        glVertex3f(3.0, 0.0, -1.0);
        glTexCoord2f(1,1);
        glVertex3f(3.0, 1.0, -1.0);

    // Bottom
        glTexCoord2f(0,1);
        glVertex3f(-3.0, 0.0, -1.0);
        glTexCoord2f(0,0);
        glVertex3f(-3.0, 0.0, 1.0);
        glTexCoord2f(1,0);
        glVertex3f(3.0, 0.0, 1.0);
        glTexCoord2f(1,1);
        glVertex3f(3.0, 0.0, -1.0);
    
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//Draws Tower
void drawTower(void) {
    
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-5.0, 0.0, 0.0);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.4, 0.5, 0.5);
    drawTowerBody();
    glPopMatrix();
    // Idk why but I can't move the textures for the others here
    glPushMatrix();
    drawTanks();
    glPopMatrix();

    glPushMatrix();
    drawTBridge();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void drawLaunchMelt(void) {
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glColor3f(0.7, 0.7, 0.7);
    glTranslatef(5.0, 0.01, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    gluDisk(q, 0.0, meltRad, 20, 5);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

//Draws rocket and tower set up
void drawRocketSet(void) {
    
    glPushMatrix();

    glTranslatef(0, 0, 15);
	drawRocket();
    drawLaunchMelt();
    drawTower();

	glPopMatrix();
}

// Second Alien
void groundAlien(void) {
    glPushMatrix();
    glTranslatef(-25.0, 0.0, 5.0);
    glRotatef(alienTheta, 0.0, 1.0, 0.0);
    glTranslatef(5.0, 0.0, 4.0);
    drawAlien();
    glTranslatef(0.8, 0.0, 0.4);
    drawAlien();
    glPopMatrix();
}

// Environment peripherals

void drawRock(size_t i) {
    glPushMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(15.0, 0.0, -5);
    gluSphere(q, periphsS[i], 4.0, 4.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void assignPeriphs(void) {
    std::uniform_int_distribution<> distrib(-fWidth+20, fWidth-20);
    std::uniform_int_distribution<> distribS(0.01, 1.5);
    for (size_t i = 0; i < 100; i++) {
        periphsX[i] = (float)distrib(gen);
        periphsZ[i] = (float)distrib(gen);
        periphsS[i] = (float)distribS(gen);
    }
}

void drawPeriphs(void) {
    glPushMatrix();
    std::uniform_int_distribution<> distrib(-fWidth, fWidth);
    for (size_t i = 0; i < 150; i++) {
        
        glPushMatrix();
        glTranslatef(periphsX[i], 0.0, periphsZ[i]);
        drawRock(i);
        glPopMatrix();
    }
    glPopMatrix();
}


// Drone

void drawDBody(void) {
    glPushMatrix();

    glTranslatef(0.0, 0.2, 0.0);
    glScalef(0.4, 0.4, 0.4);
    glutSolidCube(1.0);

	glPopMatrix();
}

void drawDArms(void) {

    glPushMatrix();
    glTranslatef(0.3, 0.3, 0.3);
    glRotatef(-45, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.1, 0.1);
    
    glutSolidCube(1.0);
	glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3, 0.3, 0.3);
    glRotatef(45, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.1, 0.1);
    
    glutSolidCube(1.0);
	glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.3, 0.3, -0.3);
    glRotatef(-45, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.1, 0.1);
    
    glutSolidCube(1.0);
	glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3, 0.3, -0.3);
    glRotatef(45, 0.0, 1.0, 0.0);
    glScalef(0.5, 0.1, 0.1);
    
    glutSolidCube(1.0);
	glPopMatrix();
}

void drawDBlades(void) {

    glPushMatrix();
    glTranslatef(0.4, 0.4, 0.4);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.1, 0.2, 10, 20);
	glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.4, 0.4, 0.4);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.1, 0.2, 10, 20);
	glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.4, 0.4, -0.4);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.1, 0.2, 10, 20);;
	glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4, 0.4, -0.4);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.1, 0.2, 10, 20);
	glPopMatrix();
}

void drawDrone(void) {
    glPushMatrix();
    float spotDir[] = {0., -1, droneSweep};
    float spotPosn[] = {0, 0, 0, 1};

    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(droneX, 50.0, droneZ);
    glRotatef(droneDir, 0, 1.0, 0);

    drawDBody();
    drawDArms();
    drawDBlades();

    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
    glLightfv(GL_LIGHT1, GL_POSITION, spotPosn);
    
	glPopMatrix();
}

// Drone Spotlight Calculations for animation
void droneSearch(void) {
    switch (leg)
    {
    case 1:
        if (legFirst) {
            if (droneX < droneWidth)
            {
                droneX ++;
            } else {
                legFirst = false;
                droneDir = -60;
            }
        } else {
            if (droneX > droneWidth/2) {
                droneZ -= 0.6;
                droneX = 0.6*droneZ + droneWidth;
            } else {
                leg = 2;
                legFirst = true;
                droneDir = 60;
            }
        }
        break;
    case 2:
        if (legFirst) {
            if (droneX > -droneWidth/2) {
                droneZ += 0.6;
                droneX = -0.6*droneZ;
            } else {
                legFirst = false;
                droneDir = 0;
            }
        } else {
            if (droneX < droneWidth/2) {
                droneX ++;
            } else {
                leg = 3;
                legFirst = true;
                droneDir = -60;
            }
        }
        break;
    case 3:
        if (legFirst) {
            if (droneX > -droneWidth/2) {
                droneZ -= 0.6;
                droneX = 0.6*droneZ;
            } else {
                legFirst = false;
                droneDir = 60;
            }
        } else {
            if (droneX > -droneWidth) {
                droneZ += 0.6;
                droneX = -0.6*droneZ - droneWidth;
            } else {
                leg = 1;
                legFirst = true;
                droneDir = 0;
            }
        }
        break;
    
    default:
        break;
    }
}

// Lake
void drawLake(void) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[13]);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        // Bottom
        glTexCoord2f(0,1);
        glVertex3f(5.0, 0.0, 5.0);
        glTexCoord2f(0,0);
        glVertex3f(5.0, 0.0, -50.0);
        glTexCoord2f(1,0);
        glVertex3f(60.0, 0.0, -50.0);
        glTexCoord2f(1,1);
        glVertex3f(60.0, 0.0, 5.0);
    
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}


// Shadows

void drawBoosterShadows(void) {
    glPushMatrix();
    drawBoostersBody();
    drawBoostersNose();
    drawBoostersEngine();
    glPopMatrix();
}

void drawRocketShadows(void) {
    glPushMatrix();

    glTranslatef(5.0, rocketHgt, 0.0);
    drawBody();
    drawNose();
    drawNoseTip();
    drawBoosterShadows();
    drawEngines();
    glPopMatrix();
}

void drawTankShadow(void) {
    glPushMatrix();
    glTranslatef(-20, 0.0, -10.0);
    glPushMatrix();
    glRotatef(-90, 1, 0.0, 0);
    gluCylinder(q, 2.0, 2.0, 10.0, 20.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.0, 0.0, 2.0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 2.0, 2.0, 20.0, 20.0, 10.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-2.0, 0, 2.0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(q, 2.0, 2.0, 15.0, 20.0, 10.0);
    glPopMatrix();
    glPopMatrix();
}

void drawTowerShadow(void) {
    //Draws Tower
    glPushMatrix();
    glTranslatef(-5.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.4, 0.5, 0.5);
    drawTowerBody();
    glPopMatrix();
    drawTankShadow();
    glPopMatrix();
}

//Draws rocket and tower set up shadows
void drawRocketSetShadow(void) {
    glPushMatrix();
    glTranslatef(0, 0, 15);
	drawRocketShadows();
    drawTowerShadow();
	glPopMatrix();
}

void drawShadows(float shadowMat[16]) {
    
    glDisable(GL_LIGHTING);
    glPushMatrix();
		glMultMatrixf(shadowMat);
		glColor4f(0.2, 0.2, 0.2, 1.0);
		drawRocketSetShadow();
	glPopMatrix();
    glEnable(GL_LIGHTING);
}

void lightSwitching(void) {
    switch (lightSwitch)
    {
    case 2:
        rL1r = 0.0;
        rL1g = 1.0;
        break;
    case 4:
        rL2r = 0.0;
        rL2g = 1.0;
        break;
    case 6:
        rL3r = 0.0;
        rL3g = 1.0;
        break;
    case 8:
        rL4r = 0.0;
        rL4g = 1.0;
        break;
    case 10:
        rL5r = 0.0;
        rL5g = 1.0;
        break;
    case 12:
        rL6r = 0.0;
        rL6g = 1.0;
        break;
    case 14:
        rL7r = 0.0;
        rL7g = 1.0;
        break;
    case 16:
        rL8r = 0.0;
        rL8g = 1.0;
        break;
    case 18:
        rL9r = 0.0;
        rL9g = 1.0;
        break;

    case 20:
        rL1r = 1.0;
        rL1g = 0.0;
        break;
    case 22:
        rL2r = 1.0;
        rL2g = 0.0;
        break;
    case 24:
        rL3r = 1.0;
        rL3g = 0.0;
        break;
    case 26:
        rL4r = 1.0;
        rL4g = 0.0;
        break;
    case 28:
        rL5r = 1.0;
        rL5g = 0.0;
        break;
    case 30:
        rL6r = 1.0;
        rL6g = 0.0;
        break;
    case 32:
        rL7r = 1.0;
        rL7g = 0.0;
        break;
    case 34:
        rL8r = 1.0;
        rL8g = 0.0;
        break;
    case 36:
        rL9r = 1.0;
        rL9g = 0.0;
        lightSwitch = 0.0;
        break;
    default:
        break;
    }
}


//----------draw a floor plane-------------------
void drawFloor() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[6]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -fWidth; x <= fWidth; x ++)
	{
		for(int z = -fWidth; z <= fWidth; z ++)
		{
			glColor3f(1.0, 1.0, 1.0);

            glTexCoord2f((float)(x+fWidth)/(2*fWidth), (float)(z+fWidth)/(2*fWidth));
			glVertex3f(x, -0.1, z);

            glTexCoord2f((float)(x+fWidth)/(2*fWidth), (float)(z+fWidth+1)/(2*fWidth));
			glVertex3f(x, -0.1, z+1);

            glTexCoord2f((float)(x+fWidth+1)/(2*fWidth), (float)(z+fWidth+1)/(2*fWidth));
			glVertex3f(x+1, -0.1, z+1);

            glTexCoord2f((float)(x+fWidth+1)/(2*fWidth), (float)(z+fWidth)/(2*fWidth));
			glVertex3f(x+1, -0.1, z);
		}
	}
	glEnd();

    glDisable(GL_TEXTURE_2D);
}

//TODO turn into own funcs
//-- Timer -------------------------------------------------
void myTimer(int value)
{
    if (takeoff) {
        if (alien1X < 10) {
            alien1X += 0.1;
            if (lightSwitch <35)
            {
                lightSwitch ++;
            }
        } else if (bridgeAngle > -90) {
            alien1Y -= 0.2;
            bridgeAngle --;
            noseAng ++;
            meltRad += 0.075;
            if (lightSwitch > 0 && lightFlip){
                lightSwitch --;
                lightFlip != lightFlip;
            } else {
                lightFlip != lightFlip;
            }
            
        } else {
            rocketHgt ++;
            meltRad -= 0.025;
        }
    } else {
        if (alienDirUp) {
            if (alien1X < 9) {
                alien1X += 0.05;
            } else {
                alienDirUp = false;
            } 
        } else {
            if (alien1X > 4) {
                alien1X -= 0.05;
            } else {
                alienDirUp = true;
            }
        }
        lightSwitch +=2;
    }
    // if (rocketHgt > skyWidth) {
    //     takeoff = false;
    // }
    lightSwitching();
    droneSearch();

    if (droneSUp) {
        if (droneSweep < 0.1) {
            droneSweep +=0.01;
        } else {
            droneSUp = false;
        }
    } else {
        if (droneSweep > -0.1) {
            droneSweep -=0.01;
        } else {
            droneSUp = true;
        }
    }
    //Alien walking movement
    if (alienAng == 20)
	{
		aUp = false;
	} else if (alienAng == -20)
	{
		aUp = true;
	}
	
	if (aUp)
	{
		alienAng += 2;
	} else {
		alienAng -= 2;
	}
    alienTheta += 0.5;

    tick++;
    if (tick == INT_MAX) tick = 0;

    updateSnowQueue();
    updateFireQueue();

    glutPostRedisplay();
	glutTimerFunc(50, myTimer, 0);
}


void display()  
{
	float lpos[4] = {100., 100., 100., 1.0};  //light's position
    float shadowMat[16] = {lpos[1], 0, 0, 0, -lpos[0], 0, -lpos[2], -1, 0, 0, lpos[1], 0, 0, 0, 0, lpos[1]};

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);	

    glEnable(GL_LIGHTING);

    skybox();
	drawFloor();

	glPushMatrix();
    drawDrone();
    groundAlien();
    drawLake();
    drawPeriphs();
    drawShadows(shadowMat);
    drawRocketSet();
    
    list<particle>::iterator it;
    for (it = snowParList.begin(); it != snowParList.end(); it++)
    {
        drawSnowParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
    }
    if (takeoff) {
        for (it = fireParList.begin(); it != fireParList.end(); it++)     {
        drawFireParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
        }
    }

	glPopMatrix();
	glutSwapBuffers();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour
    loadTextures();

    q = gluNewQuadric();

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);    //Default, only for LIGHT0
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);   //Default, only for LIGHT0

    glLightfv(GL_LIGHT1, GL_DIFFUSE, yellow);    //Default, only for LIGHT1
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);   //Default, only for LIGHT1
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 40);

    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    gluQuadricDrawStyle(q, GLU_FILL);
    gluQuadricTexture(q, GL_TRUE);

    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60., 1., 1., 1000.);  //The camera view volume  

    assignPeriphs();
    glutTimerFunc(50, myTimer, 0);
}

//------------ Special key event callback ---------------------------------
void special(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
	else if(key == GLUT_KEY_RIGHT) angle += 0.1;
	else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		eye_x -= 1*sin(angle);
		eye_z += 1*cos(angle);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		eye_x += 1*sin(angle);
		eye_z -= 1*cos(angle);
	}

	look_x = eye_x + 100*sin(angle);
	look_z = eye_z - 100*cos(angle);
	glutPostRedisplay();
}


//------------ Space key event callback ---------------------------------
// To enable Rocket Launch
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		takeoff = true;
		break;
    case 'w':
        eye_y ++;
        look_y ++;
        break;
    case 's':
        eye_y --;
        look_y --;
        break;
	
	default:
		break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height) {

    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Sets viewport to size of window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // To prevent artifacts
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 1000.0); // Set FoV (degrees), window aspect ratio, new and far planes
    glMatrixMode(GL_MODELVIEW);
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize (800, 800); 
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Alien World");
    initialize();

    glutDisplayFunc(display);
    glutSpecialFunc(special); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
