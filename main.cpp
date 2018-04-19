//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: Vase.cpp
//  USERCODE: jwy31
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadTGA.h"
#include "loadBMP.h"

using namespace std;

GLUquadricObj* qobj = gluNewQuadric();

float secondTheta = 0;
float waterDropY = 20;
float lightTheta = 10;
float earthTheta = 0;
float humanTheta = 15;
float humanY = -35;

bool persp = false;

float angle=0, look_x=25, look_y=20, look_z=0, eye_x=-10, eye_y=12, eye_z=35;//eye_z=100;  //Camera parameters
GLuint texId[20];

float BLACK[4]  = {0, 0, 0, 1.0};
float WHITE[4]  = {1.0, 1.0, 1.0, 1.0};
float GREY[4] = {0.2, 0.2, 0.2, 1.0};

struct vertex {
    float x;
    float y;
    float z;
};

typedef struct
{
    float life;                 // Particle's lifespan, max is 1
    float fade;                 // fade speed
    float x;                    // x position
    float y;                    // y position
    float z;                    // z position
    float xdir;                 // speed in x direction
    float ydir;                 // speed in y direction
    float zdir;                 // speed in z direction
}Particles;

int MAX = 5000;
Particles particle[5000];


//----------------------------------------------------------------------
//      HELPER FUNCTIONS
//----------------------------------------------------------------------

//  Set Surface Normal Vector
void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

//----------------------------------------------------------------------
//      DRAW FUNCTIONS
//----------------------------------------------------------------------

void human() {

    glPushMatrix();
        glTranslatef(-2, humanY, 12.5);
        glRotatef(90, 0, 1, 0);
        glScalef(1.5, 1.8, 1.5);
        //HEAD
        glPushMatrix();
          glColor4f(1.,0.6,0.5,1.0);
          glTranslatef(0, 7.7, 0);
          glutSolidSphere(1,20,20);
        glPopMatrix();

        //EYE 1
        glPushMatrix();
            glTranslatef(0.4, 8, 0.7);
            glutSolidSphere(0.2,20,20);
        glPopMatrix();

        //EYE 2
        glPushMatrix();
            glTranslatef(-0.4, 8, 0.7);
            glutSolidSphere(0.2,20,20);
        glPopMatrix();

        //MOUTH
        glPushMatrix();
           glTranslatef(0, 7.55, 0.7);
            glScalef(3, 0.4, 1.8);
            glutSolidSphere(0.2,20,20);
        glPopMatrix();

        //BODY
        glPushMatrix();
          glColor4f(.9,0.2,.3,0.0);
          glTranslatef(0, 5.5, 0);
          glScalef(3, 3, 1.4);
          glutSolidCube(1);
        glPopMatrix();

        //LEG 1
        glPushMatrix();
            glColor4f(1.,1.,1.,0.0);
            glTranslatef(-0.8,4,0);
            glRotatef(-humanTheta, 1, 0, 0);
            glTranslatef(0.8, -4, 0);
            glTranslatef(-0.8, 2.2, 0);
            glScalef(1, 4.4, 1);
            glutSolidCube(1);
        glPopMatrix();

        //LEG 2
        glPushMatrix();
            glTranslatef(-0.8, 4, 0);
            glRotatef(humanTheta, 1, 0, 0);
            glTranslatef(0.8, -4, 0);
            glTranslatef(0.8, 2.2, 0);
            glScalef(1, 4.4, 1);
            glutSolidCube(1);
        glPopMatrix();

        //ARM 1
        glPushMatrix();
            glColor4f(1.,1.,1.,0.0);
            glTranslatef(-2, 6.5, 0);
            glRotatef(humanTheta-140, 1, 0, 0);
            glTranslatef(2, -6.5, 0);
            glTranslatef(-2, 5, 0);
            glScalef(1, 4, 1);
            glutSolidCube(1);
        glPopMatrix();

        //ARM 2
        glPushMatrix();
            glTranslatef(2,6.5,0);
            glRotatef(-humanTheta-140, 1, 0, 0);
            glTranslatef(-2, -6.5, 0);
            glTranslatef(2, 5, 0);
            glScalef(1, 4, 1);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

// Water Drop -into bowl
void waterDrop()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLACK);
    glPushMatrix();
        glColor3f(0.118, 0.57, 1);
        glTranslatef(17, waterDropY, 20);
        glutSolidSphere(0.25, 10, 10);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);
}

// Bowl -surface of rotaion
void bowl()
{
    waterDrop();

    static int N = 50;
    float angStep = 10.0*3.1415926/180.0;

    /*
    float vx[N] = {0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 4, 5, 5.6, 6.1, 6.8, 7.1, 7.5, 8, 8.4, 8.7, 9, 9.3,
              9.8, 10, 10.2, 10.4, 10.6, 10.9, 11, 11.1, 11.2, 11.3, 11.4, 11.3, 11.2, 11.1, 11, 10.5, 9.5, 8.2, 7, 6.2,
              6, 6.2, 6.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5};
    float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
               39, 40, 41, 42, 43, 43, 42, 41, 40, 39};
    float vz[N] = {0};
    * */

    float vx[N], vy[N], vz[N];

    // init v
    for (int i = 0; i < N; i++) {
        vz[i] = 0;
        if (i < 10) {
            vx[i] = i;
            vy[i] = 0;
        } else if (i >= 10 && i <= 25) {
            vx[i] = i;
            vy[i] = pow((i-10)/5.0, 2.0);
        } else if (i > 25 && i < 40) {
            vx[i] = 50 - i;
            vy[i] = pow((25-(i-25))/5.0, 2.0) + 1.5;
        } else if (i >= 40) {
            vx[i] = 50 - i;
            vy[i] = 0 + 1.5;
        }
    }
    float wx[N], wy[N], wz[N];

    glColor3f(0.5, 0, 0.5);
    glPushMatrix();
        glTranslatef(17, 6, 20);
        glScalef(0.10, 0.10, 0.10);
        for (int j = 0; j < 36; j++) {
            // w is a rotation of v
            for (int i = 0; i < N; i++) {
                wx[i] = vx[i] * cos(angStep) + vz[i] * sin(angStep);
                wy[i] = vy[i];
                wz[i] = -vx[i] * sin(angStep) + vz[i] * cos(angStep);
            }

            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < N; i++) {
                if (i > 0) normal(vx[i-1], vy[i-1], vz[i-1],
                                    vx[i], vy[i], vz[i],
                                    wx[i], wy[i], wz[i]);
                    glVertex3f(vx[i], vy[i], vz[i]);
                    glVertex3f(wx[i], wy[i], wz[i]);
            }
            //glVertex3f(vx[0], vy[0], vz[0]);
            //glVertex3f(wx[0], wy[0], wz[0]);
            glEnd();

            // v becomes w
           for (int i = 0; i < N; i++) {
                vx[i] = wx[i];
                vy[i] = wy[i];
                vz[i] = wz[i];
            }
        }
    glPopMatrix();
}

// Desk
void desk()
{
    glPushMatrix();
        glTranslatef(3, 0, 16.9);
        glTranslatef(9, 3, 4);
        glScalef(18, 6, 8);
        glutSolidCube(1);
    glPopMatrix();
}

// Bed
void bed()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLACK);
    glPushMatrix();
        glColor4f(0.5, 0.5, 0.5, 1.0);
        glTranslatef(34, 2, 13);
        glScalef(12, 4, 23);
        glutSolidCube(1);
    glPopMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);
}

// Clock
void floorClock()
{
    glPushMatrix();
        glTranslatef(15, 0, 0);
        // second hand
        glColor3f(0.8, 0.2,  1.);
        glPushMatrix();
            glTranslatef(2.5, 7, 5.3);
            glRotatef(-secondTheta, 0, 0, 1);
            glTranslatef(0, 0.9, 0.05);
            glScalef(0.2, 1.8, 0.1);
            glutSolidCube(1);
        glPopMatrix();

        // face cylinder
        glColor3f(0.4, 0.4,  1.);
        glPushMatrix();
            glTranslatef(2.5, 7, 0.3);
            gluCylinder(qobj, 2, 2, 5, 40, 40);
            glPushMatrix();
                glTranslatef(0, 0, 5);
                gluDisk(gluNewQuadric(), 0, 2, 40, 40);
            glPopMatrix();
        glPopMatrix();

        // top cylinder
        glColor3f(0.4, 0.4,  0.);
        glPushMatrix();
            glTranslatef(2.5, 7, 0.05);
            gluCylinder(qobj, 2.5, 2.5, 5, 40, 40);
            gluDisk(gluNewQuadric(), 0, 2.5, 40, 40);
            glPushMatrix();
                glTranslatef(0, 0, 5);
                gluDisk(gluNewQuadric(), 0, 2.5, 40, 40);
            glPopMatrix();
        glPopMatrix();

        // base cube
        glPushMatrix();
            glTranslatef(2.5, 3.5, 2.5);
            glScalef(5, 7, 5);
            glutSolidCube(1);
        glPopMatrix();
    glPopMatrix();
}

// House
void house()
{
    bed();
    desk();

    float lbb[3] = {0, 0, 0.1};
    float rbb[3] = {40, 0, 0.1};
    float ltb[3] = {0, 30, 0.1};
    float rtb[3] = {40, 30, 0.1};
    float lbf[3] = {0, 0, 25};
    float rbf[3] = {40, 0, 25};
    float ltf[3] = {0, 20, 25};
    float rtf[3] = {40, 20, 25};

    float mat_col[] = {1, 0.5, 0, 1};
    glColor3f(1, 0.5, 0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLACK);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_col);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texId[7]);
    glBegin(GL_QUADS);
        // front face
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0);   glVertex3f(lbf[0], lbf[1], lbf[2]);
        glTexCoord2f(1, 0);   glVertex3f(rbf[0], rbf[1], rbf[2]);
        glTexCoord2f(1, 1);   glVertex3f(rtf[0], rtf[1], rtf[2]);
        glTexCoord2f(0, 1);   glVertex3f(ltf[0], ltf[1], ltf[2]);
    glEnd();


    glColor3f(1, 0.5, 0);
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    glBegin(GL_QUADS);
        // rear face
        glNormal3f(0, 0, 1);
        glTexCoord2f(0, 0);   glVertex3f(lbb[0], lbb[1], lbb[2]);
        glTexCoord2f(1, 0);   glVertex3f(rbb[0], rbb[1], rbb[2]);
        glTexCoord2f(1, 1);   glVertex3f(rtb[0], rtb[1], rtb[2]);
        glTexCoord2f(0, 1);   glVertex3f(ltb[0], ltb[1], ltb[2]);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    glBegin(GL_QUADS);
        // roof
        normal(ltf[0], ltf[1], ltf[2], rtf[0], rtf[1], rtf[2], rtb[0], rtb[1], rtb[2]);
        glTexCoord2f(0, 0);   glVertex3f(ltf[0], ltf[1], ltf[2]);
        glTexCoord2f(1, 0);   glVertex3f(rtf[0], rtf[1], rtf[2]);
        glTexCoord2f(1, 1);   glVertex3f(rtb[0], rtb[1], rtb[2]);
        glTexCoord2f(0, 1);   glVertex3f(ltb[0], ltb[1], ltb[2]);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glBegin(GL_QUADS);
    for(int i = 0; i < 40; i++)
    {
        for(int j = 0;  j < 25; j++)
        {
            glColor3f(0.6, 1.0, 0.8);
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glVertex3f(i+1, 0, j+1);
            glVertex3f(i+1, 0, j);
        }
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    // ladder
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 10);   glVertex3f(0, 0, 8);
        glTexCoord2f(0, 0);   glVertex3f(0, -100, 8);
        glTexCoord2f(1, 0);   glVertex3f(0, -100, 17);
        glTexCoord2f(1, 10);   glVertex3f(0, 0, 17);
    glEnd();

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);
    glDisable(GL_TEXTURE_2D);

    // struts
    glColor3f(0.64, 0.43, 0.0);
    glPushMatrix();
        glTranslatef(3, -0.1, 0);
        glTranslatef(1, -1, 14);
        glScalef(2, 2, 28);
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(36, -0.1, 0);
        glTranslatef(1, -1, 14);
        glScalef(2, 2, 28);
        glutSolidCube(1);
    glPopMatrix();
}

// Cliff
void cliff()
{
    float lb[3] = {-1000, -500, 0};
    float rb[3] = {1000, -500, 0};
    float rt[3] = {1000, 50, 0};
    float lt[3] = {-1000, 50, 0};

    glColor3f(0, 0, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, BLACK);    // turn off specular highlights

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[6]);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);         glVertex3f(lb[0], lb[1], lb[2]);
        glTexCoord2f(5.079, 0);     glVertex3f(rb[0], rb[1], rb[2]);
        glTexCoord2f(5.079, 1);     glVertex3f(rt[0], rt[1], rt[2]);
        glTexCoord2f(0, 1);         glVertex3f(lt[0], lt[1], lt[2]);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);
}

// Hanging Light
void hangingLight()
{
    float spot_pos[] = {0.0, 0.0, 0.0, 1.0};
    float spot_dir[] = {0, -1, 0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);

    glPushMatrix();
        glTranslatef(20, 15, 12.5);
        /*
        glPushMatrix();
            glTranslatef(0, -10, 0);
            glutSolidTeapot(4);
        glPopMatrix();
        * */
        glTranslatef(0, 10, 0);
        glRotatef(lightTheta, 1, 0, 0);
        glTranslatef(0, -10, 0);

        // set spotlight pos and dir
        glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

        // holding wire
        glLineWidth(2.5);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
            glVertex3f(0.0, 10.0, 0.0);
            glVertex3f(0.0, 0.0, 0.0);
        glEnd();

        // lightbulb
        glColor3f(1.0, 1.0, 0.4);
        glPushMatrix();
            glTranslatef(0, 0.8, 0);
            glutSolidSphere(1.2, 40, 40);

        glPopMatrix();
        // shade cone
        glColor3f(0.9, 0.1, 0.4);
        glPushMatrix();
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            gluCylinder(qobj, 3, 0, 3, 40, 40);
        glPopMatrix();
    glPopMatrix();
}

// Globe
void globe()
{
    // init frame
    /*
    int N = 40;
    float fy[N];

    for (int x = 0.0; x < N; x++) {
        fy[x] = sqrt(pow(x, 2.0) - pow(x, 2.0));
    }
    */

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glPushMatrix();
        glTranslatef(7, 6, 22);
        glScalef(0.7, 0.7, 0.7);

        // Frame
        glColor3f(0.5, 0.9, 0.4);
        glPushMatrix();
            glRotatef(-90, 1, 0, 0);
            glutSolidCone(1.2, 1.2, 40, 40);
        glPopMatrix();

        // Earth
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[9]);
        glPushMatrix();
            glTranslatef(0.0, 4.0, 0.0);
            glRotatef(20, 0, 1, 0);
            glRotatef(23.5, 0, 0, 1);           //tilt the earth
            glRotatef(earthTheta, 0, 1, 0);             //Rotation about polar axis of the Earth
            glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
            gluSphere ( qobj, 3.0, 36, 17 );
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Particles
void drawParticles()
{
    glPushMatrix();
        glTranslatef(50, 20, 2);
        glScalef(1.5, 2, 1.5);
        glRotatef(170, 1, 0, 0);
        // Creates Particles upto max
        for (int i=0;i<MAX;i++)
        {
            particle[i].life=.1;
            particle[i].fade=float(rand()%100)/1000;
            particle[i].xdir=float((rand()%50)-25.0);
            particle[i].ydir=float((rand()%80)-25.0);
            particle[i].zdir=float((rand()%50)-25.0);

        }

        // Moves
        for (int i = 0;i < MAX; i++){
            glColor3f(0.118, 0.57, 1);
            if (particle[i].life > 0)
            {
                glTranslatef(particle[i].x,particle[i].y,particle[i].z);
                // Particle Object
                gluSphere(qobj,0.1,4,10);
                glTranslatef(-particle[i].x,-particle[i].y,-particle[i].z);
                //Reduces Particles position to 1/5000th, increases time
                //between particle releases.
                particle[i].x += particle[i].xdir/300;
                particle[i].y += particle[i].ydir/300;
                particle[i].z += particle[i].zdir/300;
                particle[i].life -= particle[i].fade;
                if ( particle[i].life==0 || particle[i].y<0 || particle[i].y>10)
                {
                    particle[i].life=.1;
                    particle[i].fade=float(rand()%100)/1000.0;
                    particle[i].x=0.0;
                    particle[i].y=0.1;
                    particle[i].z=0.0;
                    particle[i].xdir=float((rand()%80)-31.0);
                    particle[i].ydir=float((rand()%60)-31.0);
                    particle[i].zdir=float((rand()%100)-32.0);
                }
            }
        }
    glPopMatrix();
}

// Skybox
void skybox(){
    glEnable(GL_TEXTURE_2D);

    ////////////////////// LEFT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);   glVertex3f(-1000,  -500, 1000);
      glTexCoord2f(1, 0);   glVertex3f(-1000, -500., -1000);
      glTexCoord2f(1, 1);   glVertex3f(-1000, 500., -1000);
      glTexCoord2f(0, 1);   glVertex3f(-1000, 500, 1000);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);   glVertex3f(-1000,  -500, -1000);
    glTexCoord2f(1, 0);   glVertex3f(1000, -500., -1000);
    glTexCoord2f(1, 1);   glVertex3f(1000, 500, -1000);
    glTexCoord2f(0, 1);   glVertex3f(-1000,  500, -1000);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);   glVertex3f(1000,  -500, -1000);
      glTexCoord2f(1, 0);   glVertex3f(1000, -500, 1000);
      glTexCoord2f(1, 1);   glVertex3f(1000, 500,  1000);
      glTexCoord2f(0, 1);   glVertex3f(1000,  500,  -1000);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);   glVertex3f( 1000, -500, 1000);
      glTexCoord2f(1, 0);   glVertex3f(-1000, -500,  1000);
      glTexCoord2f(1, 1);   glVertex3f(-1000, 500,  1000);
      glTexCoord2f(0, 1);   glVertex3f( 1000, 500, 1000);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1);   glVertex3f(-1000, 500, -1000);
        glTexCoord2f(0, 1);   glVertex3f(1000, 500,  -1000);
        glTexCoord2f(0, 0);   glVertex3f(1000, 500,  1000);
        glTexCoord2f(1, 0);   glVertex3f(-1000, 500, 1000);
    glEnd();

    /////////////////////// FLOOR //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(1, 0);   glVertex3f(-1000, -500., 1000);
        glTexCoord2f(0, 0);   glVertex3f(1000, -500.,  1000);
        glTexCoord2f(0, 1);   glVertex3f(1000, -500., -1000);
        glTexCoord2f(1, 1);   glVertex3f(-1000, -500., -1000);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//----------------------------------------------------------------------
//      LOAD FUNCTIONS
//----------------------------------------------------------------------

void loadGLTextures()           // Load bitmaps And Convert To Textures
{
    glGenTextures(20, texId);        // Create texture ids
    // *** left ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("images/front.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** front ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("images/left.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** right ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("images/back.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** back***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("images/right.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("images/top.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** down ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    loadTGA("images/down.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** cliff ***
    glBindTexture(GL_TEXTURE_2D, texId[6]);
    loadTGA("images/cliff.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // *** house wall ***
    glBindTexture(GL_TEXTURE_2D, texId[7]);
    loadTGA("images/wall2.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // *** ladder ***
    glBindTexture(GL_TEXTURE_2D, texId[8]);
    loadTGA("images/ladder.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // *** earth ***
    glBindTexture(GL_TEXTURE_2D, texId[9]);  //Use this texture name for the following OpenGL texture
    loadBMP("images/Earth.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // *** iron ***
    glBindTexture(GL_TEXTURE_2D, texId[10]);
    loadTGA("images/iron.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//----------------------------------------------------------------------
//      INITIALISE FUNCTION
//----------------------------------------------------------------------

//  Initialise
void initialise(void)
{
    float sun_pos[] = {-100, 100, 100, 1};

    loadGLTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    gluQuadricDrawStyle(qobj, GLU_FILL );
    gluQuadricNormals(qobj, GLU_SMOOTH );
    gluQuadricTexture (qobj, GL_TRUE);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, GREY);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, GREY);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, WHITE);
    glLightfv(GL_LIGHT1, GL_SPECULAR, WHITE);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.01);

    glEnable(GL_SMOOTH);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor (0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1.0, 1.0, 3000.0);
}

//----------------------------------------------------------------------
//      DISPLAY FUNCTION
//----------------------------------------------------------------------

//  Display
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (!persp) {
        gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z, 0, 1, 0);
    } else {
        gluLookAt(-0.5, humanY+13.5, 12.5,  look_x, 0, look_z, 0, 1, 0);
    }

    skybox();
    bowl();
    cliff();
    house();
    floorClock();
    hangingLight();
    globe();
    human();
    drawParticles();

    glutSwapBuffers();
}

//----------------------------------------------------------------------
//      EVENT FUNCTIONS
//----------------------------------------------------------------------

// Clock Timer
void clockTimer(int value)
{
    secondTheta += 3;
    glutPostRedisplay();
    glutTimerFunc(100, clockTimer, 0);
}

// Water Drop Timer
void waterDropTimer(int value)
{
    if (waterDropY < 0) {
        waterDropY = 20;
    }
    waterDropY -= 0.2;
    glutPostRedisplay();
    glutTimerFunc(20, waterDropTimer, 0);
}

// Light Timer
void lightTimer(int value)
{
    static bool dir = true;
    if (dir) {
        lightTheta += 0.1;
    } else {
        lightTheta -= 0.1;
    }
    if (lightTheta > 10) {
        dir = false;
    } else if (lightTheta < -10) {
        dir = true;
    }

    glutPostRedisplay();
    glutTimerFunc(20, lightTimer, 0);
}

// Earth Timer
void earthTimer(int value)
{
    earthTheta += 1;
    if (earthTheta > 360) {
        earthTheta = 0;
    }
    glutPostRedisplay();
    glutTimerFunc(20, earthTimer, 0);
}

// Human Timer
void humanTimer(int value)
{
    static bool dirTheta = true;
    if (dirTheta) {
        humanTheta += 1;
    } else {
        humanTheta -= 1;
    }
    if (humanTheta > 20) {
        dirTheta = false;
    } else if (humanTheta < -20) {
        dirTheta = true;
    }

    static bool dirY = true;
    if (dirY) {
        humanY += 0.2;
    } else {
        humanY -= 0.2;
    }
    if (humanY > -10) {
        dirY = false;
    } else if (humanY < -35) {
        dirY = true;
    }

    glutPostRedisplay();
    glutTimerFunc(20, humanTimer, 0);
}

//  Special Keypress
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle -= 0.04;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.04;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 0.4*sin(angle);
        eye_z += 0.4*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 0.4*sin(angle);
        eye_z -= 0.4*cos(angle);
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);

    // toggle perspective
    if (key == GLUT_KEY_F1) persp = !persp;

    glutPostRedisplay();
}

//----------------------------------------------------------------------
//      MAIN
//----------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE| GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Assignment");
   initialise();
   glutTimerFunc(20, clockTimer, 0);
   glutTimerFunc(20, waterDropTimer, 0);
   glutTimerFunc(20, lightTimer, 0);
   glutTimerFunc(20, earthTimer, 0);
   glutTimerFunc(20, humanTimer, 0);
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
