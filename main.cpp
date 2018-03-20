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

using namespace std;

float angle=0, look_x=0, look_y=20, look_z=0, eye_x=0, eye_y=10, eye_z=200;  //Camera parameters
GLuint texId[7];

struct vertex {
    float x;
    float y;
    float z;
};

//----------------------------------------------------------------------
//      DRAW FUNCTIONS
//----------------------------------------------------------------------

//  Cliff
void cliff()
{
    float black[4]  = {0, 0, 0, 0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    float lb[3] = {-1000, -500, 0};
    float rb[3] = {1000, -500, 0};
    float rt[3] = {1000, 50, 0};
    float lt[3] = {-1000, 50, 0};

    glColor3f(0, 0, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);    // turn off specular highlights

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[6]);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);         glVertex3f(lb[0], lb[1], lb[2]);
        glTexCoord2f(5.079, 0);     glVertex3f(rb[0], rb[1], rb[2]);
        glTexCoord2f(5.079, 1);     glVertex3f(rt[0], rt[1], rt[2]);
        glTexCoord2f(0, 1);         glVertex3f(lt[0], lt[1], lt[2]);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
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

//  Ground Plane
void floor()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.7, 0.7,  0.7);          //Floor colour

    for (int i = -200; i <= 200; i +=5)
    {
        glBegin(GL_LINES);          //A set of grid lines on the xz-plane
            glVertex3f(-200, -40, i);
            glVertex3f(200, -40, i);
            glVertex3f(i, -40, -200);
            glVertex3f(i, -40, 200);
        glEnd();
    }
    glEnable(GL_LIGHTING);
}

//----------------------------------------------------------------------
//      LOAD FUNCTIONS
//----------------------------------------------------------------------

void loadGLTextures()           // Load bitmaps And Convert To Textures
{
    glGenTextures(7, texId);        // Create texture ids
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

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

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
//      INITIALISE FUNCTION
//----------------------------------------------------------------------

//  Initialise
void initialise(void)
{
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    loadGLTextures();
    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glEnable(GL_SMOOTH);
    * */

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor (1.0, 1.0, 1.0, 0.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(70.0, 1.0, 1.0, 5000.0);
}

//----------------------------------------------------------------------
//      DISPLAY FUNCTION
//----------------------------------------------------------------------

//  Display
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 1.0, 1.0, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye_x, eye_y, eye_z,  look_x, look_y, look_z,   0, 1, 0);
    //gluLookAt (0, 500, 0, 0, 500, 50, 0, 1, 0);

    skybox();
    glColor3f(.7, .2, 1);
    cliff();

    glutSolidTeapot(1);

    glutSwapBuffers();
}

//----------------------------------------------------------------------
//      EVENT FUNCTIONS
//----------------------------------------------------------------------

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

    look_x = eye_x + 200*sin(angle);
    look_z = eye_z - 200*cos(angle);
    glutPostRedisplay();
}

//----------------------------------------------------------------------
//      MAIN
//----------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE| GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Assignment");
   initialise();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
