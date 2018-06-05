/* viewpoints.c

   Laura Toma

   What it does:

   Draws a set of points in the default 2D projection.

   Includes a tentative function for printing and drawing a list of-
   points (assumed to be a convex hull). These functions were not
   debugged so use them at your own risk.

   Feel free to change this code as you need.

*/

#include "geom.h"
#include "rtimer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

//to compile on both apple and unix platform
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
using namespace std;




//pre-defined colors for convenience
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};
/* from https://www.opengl.org/discussion_boards/showthread.php/132502-Color-tables  */
GLfloat brown[3] = { 0.647059, 0.164706, 0.164706};
GLfloat DarkBrown[3] = { 0.36, 0.25, 0.20};
GLfloat DarkTan[3] = { 0.59, 0.41, 0.31};
GLfloat Maroon[3]= { 0.556863, 0.137255, 0.419608};
GLfloat DarkWood[3] = { 0.52, 0.37, 0.26};
GLfloat  Copper[3] = { 0.72,  0.45,  0.20};
GLfloat green1[3] = {.5, 1, 0.5};
GLfloat green2[3] = {0.0, .8, 0.0};
GLfloat green3[3] = {0.0, .5, 0.0};
GLfloat ForestGreen[3] = { 0.137255, 0.556863, 0.137255};
GLfloat MediumForestGreen[3] = { 0.419608 , 0.556863 , 0.137255};
GLfloat LimeGreen[3] ={ 0.196078,  0.8 , 0.196078};
GLfloat Orange[3] = { 1, .5, 0};
GLfloat Silver[3] = { 0.90, 0.91, 0.98};
GLfloat Wheat[3] = { 0.847059 , 0.847059, 0.74902};





/* global variables */
const int WINDOWSIZE = 500;

//the array of n points
//needs to be global in order to be rendered
vector<point2D>  points;

int n;  //desired number of points

//the convex hull, stored as a list.
//needs to be global in order to be rendered
vector<point2D>  hull;



//currently there are 4 different ways to initialize points.
//The user can cycle through them by pressing 'i'
int NB_INIT_CHOICES = 7;
int  POINT_INIT_MODE = 0;





/********************************************************************/
/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);

/* render the array of points stored in global variable points.
   Each point is drawn as a small square.  */
void draw_points();

/* Render the hull; the points on the hull are expected to be in
   boundary order (either ccw or cw). To render the hull we'll draw
   lines between consecutive points */
  void draw_hull();

/* print the array of n points stored in global variable points[]*/
void print_points(vector<point2D> points);

//print the list of points in global variable  hull
void print_hull (vector<point2D> hull);

/** initializer function */
void initialize_points_circle();
void initialize_points_horizontal_line();
void initialize_points_random() ;
void initialize_points_star() ;
//you'll add more


/********************************************************************/





//assumes global array points  is allocated to hold n points.
//this functions initializes points[] with  n points on a circle.
//The points are in the range (0,0) to (WINSIZE,WINSIZE).
void initialize_points_circle() {

  printf("initialize points circle\n");
  //clear the vector just to be safe
  points.clear();

  double  step = 2* M_PI/n;
  int rad = 100;

  int i;
  point2D p;
  for (i=0; i<n; i++) {
    p.x = WINDOWSIZE/2+ rad*cos(i*step);
    p.y = WINDOWSIZE/2+ rad*sin(i*step);
    points.push_back(p);
  }
}

void initialize_points_heart() {

  printf("initialize points heart\n");
  //clear the vector just to be safe
  points.clear();

  double  step = 2* M_PI/n;
  int rad = 100;

  int i;
  point2D p;
  for (i=0; i<n; i++) {
    double t = i*step;
    double r = 2 - 2*sin(t) + sin(t)*(sqrt(abs(cos(t)))/(sin(t)+1.4));
    r = r*rad;
    p.x = WINDOWSIZE/2 + r*cos(t);
    p.y = WINDOWSIZE/1.2 + r*sin(t);
    points.push_back(p);
  }
}

void initialize_points_doubleCircle(){

  printf("initialize points double circle\n");
  //clear the vector just to be safe
  points.clear();
  double  step = 2* M_PI/n;
  int rad = 100;


  int i;
  point2D p;
  for (i=0; i<n; i++) {
    if( i%2 == 0){
      p.x = WINDOWSIZE/4+ rad*cos(i*step);
      p.y = WINDOWSIZE/2+ rad*sin(i*step);
      points.push_back(p);
    }
    if(i%2 == 1) {
      p.x = WINDOWSIZE/2 + rad*cos(i*step);
      p.y = WINDOWSIZE/2 + rad*sin(i*step);
      points.push_back(p);
    }
  }
}

//assumes global array points is allocated to hold n points.
//this functions initializes points[] with  n points on a line
//The points are in the range (0,0) to (WINSIZE,WINSIZE)
void initialize_points_horizontal_line() {

  printf("initialize points line\n");
  //clear the vector just to be safe
  points.clear();

  int i;
  point2D p;
  for (i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    p.y =  WINDOWSIZE/2;
    points.push_back(p);
  }
}



//assumes global array points is allocated to hold n points.
//this functions initializes points[] with  n random points
//The points are in the range (0,0) to (WINSIZE,WINSIZE)
void initialize_points_random() {

   printf("initialize points random\n");
  //clear the vector just to be safe
  points.clear();

  int i;
  point2D p;
  for (i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    points.push_back(p);
  }
}


//assumes global array points is allocated to hold n points.
//this functions initializes points[] with  n points  that look like a star
//The points are in the range (0,0) to (WINSIZE,WINSIZE)
void initialize_points_star() {

  printf("initialize points star\n");
  //clear the vector just to be safe
  points.clear();

  int i;
  point2D p;
  for (i=0; i<n; i++) {
    if (i%2 == 0) {

      p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
      p.y =  random() % ((int)(.7*WINDOWSIZE))  / 5;
      p.y += (int)((1-.7/5)*WINDOWSIZE/2);
    };
    if (i%2 == 1)  {

      p.x = random() % ((int)(.7*WINDOWSIZE)) / 5;
      p.x +=  (int)((1-.7/5)*WINDOWSIZE/2);
      p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    }

    points.push_back(p);

  }//for i

}


void initialize_points_butterfly(){
    printf("initialize points butterfly\n");

    points.clear();

    for(int i = 0; i < n; i++){
        float t = (float) random();
        point2D p;
        p.x = sin(t)*(exp(cos(t)) - 2 * cos(4*t) - sin(pow(t/12, 5))) * 55;
        p.y = cos(t)*(exp(cos(t)) - 2 * cos(4*t) - sin(pow(t/12, 5))) * 55;

        p.x += WINDOWSIZE/2;
        p.y += WINDOWSIZE/2;

        points.push_back(p);
    }
}

void initialize_points_square() {
    printf("initialize points square\n");
    //clear the vector just to be safe
    points.clear();
    int i;
    point2D p;

    p.x = (int)((.3*WINDOWSIZE)/2);
    p.y = (int)((.3*WINDOWSIZE)/2);
    points.push_back(p);

    p.x = (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
    p.y = (int)((.3*WINDOWSIZE)/2);
    points.push_back(p);

    p.x = (int)((.3*WINDOWSIZE)/2);
    p.y = (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
    points.push_back(p);

    p.x = (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
    p.y = (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
    points.push_back(p);


    for (i=4; i<n; i++) {
        if (i%4 == 0) { // bottom
            p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
            p.y =  (int)((.3*WINDOWSIZE)/2);
            points.push_back(p);
        } else if (i%4 == 1) { // right
            p.x = (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
            p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
            points.push_back(p);
        } else if (i%4 == 2) { // top
            p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
            p.y =  (int)((.3*WINDOWSIZE)/2) + ((int)(.7*WINDOWSIZE));
            points.push_back(p);
        } else { // left
            p.x = (int)((.3*WINDOWSIZE)/2);
            p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
            points.push_back(p);
        }

    }
}



/* ****************************** */
/* print the array of points */
void print_points(vector<point2D> points) {

  int i;
  printf("points: ");
  for (i=0; i< points.size(); i++) {
    printf("[%3d,%3d] ", points[i].x, points[i].y);
  }
  printf("\n");
  fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}

/* ****************************** */
//print the  hull;
void print_hull (vector<point2D> hull) {

  printf("hull: ");
  //...
  printf("\n");
 }



/* ****************************** */
int main(int argc, char** argv) {

  //read number of points from user
  if (argc!=2) {
    printf("usage: viewPoints <nbPoints>\n");
    exit(1);
  }
  n = atoi(argv[1]);
  printf("you entered n=%d\n", n);
  assert(n >0);

  //initialize the points
  initialize_points_star();
  //print_points(points);

  //compute the convex hull and store it in global variable "hull"
  Rtimer rt1;
  rt_start(rt1);
  hull = graham_scan(points);
  rt_stop(rt1);
  print_hull(hull);
  //print the timing
  char buf [1024];
  rt_sprint(buf,rt1);
  printf("hull time:  %s\n\n", buf);
  fflush(stdout);


  //start the rendering
  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display);
  glutKeyboardFunc(keypress);

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);

  /* give control to event handler */
  glutMainLoop();
  return 0;
}




/* ****************************** */
/* This is the main render function. We registered this function to be
   called by GL to render the window.
 */
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  //clear all modeling transformations
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  /* The default GL window is [-1,1]x[-1,1]x[-1,1] with the origin in
     the center. The camera is at (0,0,0) looking down negative
     z-axis.

     The points are in the range (0,0) to (WINSIZE,WINSIZE), so they
     need to be mapped to [-1,1]x [-1,1] */
  //then scale the points to [0,2]x[0,2]
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);
  //first translate the points to [-WINDOWSIZE/2, WINDOWSIZE/2]
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0);

  draw_points();
  draw_hull();

  /* execute the drawing commands */
  glFlush();
}




/* ****************************** */
/* draw the array of points stored in global variable points[]
   each point is drawn as a small square
*/
void draw_points(){

  const int R= 1;
  //draw polygon filled or line
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set drawing color
  glColor3fv(yellow);

  int i;
  for (i=0; i<n; i++) {
    //draw a small square centered at (points[i].x, points[i].y)
    glBegin(GL_POLYGON);
    glVertex2f(points[i].x -R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y+R);
    glVertex2f(points[i].x -R,points[i].y+R);
    glEnd();
  }
} //draw_points





/* ****************************** */
/* Render the hull; the points on the hull are expected to be in
   boundary order (either ccw or cw). To render the hull we'll draw
   lines between consecutive points */
void draw_hull(){

  //set color
  glColor3fv(red);

  if (hull.size() >0) {
    int i;
    for (i=0; i< hull.size()-1; i++) {

      //draw a line fromcrt to next
      glBegin(GL_LINES);
      glVertex2f(hull[i].x, hull[i].y);
      glVertex2f(hull[i+1].x, hull[i+1].y);
      glEnd();
    }

    //draw a line from the last point to the first point
    //fill in code here


  }//if (hull not empty)
}



/* ****************************** */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
    exit(0);
    break;

  case 'i':
    //change points initializer
    POINT_INIT_MODE = (POINT_INIT_MODE+1) % NB_INIT_CHOICES;
    switch (POINT_INIT_MODE) {
    case 0:
      initialize_points_circle();
      break;
    case 1:
      initialize_points_star();
      break;
    case 2:
      initialize_points_square();
      break;
    case 3:
      initialize_points_horizontal_line();
      break;
    case 4:
      initialize_points_random();
      break;
    case 5:
      initialize_points_heart();
      break;
    case 6:
      initialize_points_square();
      break;
    } //switch

    //note: we change global array points, so we must recompute the hull
     hull = graham_scan(points);

    //redraw
    glutPostRedisplay();

  } //switch (key)

}//keypress



/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height);
}
