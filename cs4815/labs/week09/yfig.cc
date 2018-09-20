#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif
#define THICKER 1
#define THINNER 2
#define RED 	3
#define GREEN 	4
#define BLUE 	5
#define BLACK 	6
#define QUIT 	7
enum
{
  MOUSE_LEFT_BUTTON = 0,
  MOUSE_MIDDLE_BUTTON = 1,
  MOUSE_RIGHT_BUTTON = 2,
  MOUSE_SCROLL_UP = 3,
  MOUSE_SCROLL_DOWN = 4
};

int numLines;
int numRect;
bool leftButton = false;
bool linedraw = false;
bool circledraw = false;
bool rectangledraw = false;
typedef enum state
{
waitOnClick,
clickedonce,
}state;

typedef struct point
{
  int x;
  int y;
}point;

point lines[256][2] ;
point rect[256][2] ;
int gState=waitOnClick;
bool lineIsValid=false;
int gHeight;
float gColor[3]={0,1,0};
GLdouble movex,movey,downX,downY,downXnew, downYnew;
float thickness = 3.0;
GLfloat xwcMin = -600.0, xwcMax = 600.0;
GLfloat ywcMin = -600.0, ywcMax = 600.0;
GLsizei winWidth = 600, winHeight = 600; 
bool MiddleButton = false;
float rotate =0.0;
float start = 0.0;
float zDepth = 0.0;
int pointSize = 0;
int pointArray[100][2];
float radiusArray[50];
int mX = 0;
int mY = 0;
int rubberBandOn = 0;
int downActive = 0;

void drawCircle(float radius, float x1, float y1)
{   
  glColor3fv(gColor);
  glLineWidth(thickness);
  float angle = 0;
  float x2,y2,cx,cy,fx,fy;
  int cache = 0;
  glBegin(GL_LINES);
  for (angle = 0; angle < 360; angle+=1.0)
  {
    float rad_angle = angle * 3.14 / 180;
    x2 = x1+radius * sin((double)rad_angle);
    y2 = y1+radius * cos((double)rad_angle);
    if (cache)
    {
      glVertex2f(cx,cy);
      glVertex2f(x2,y2);
    }
    else
    {
      fx = x2;
      fy = y2;
    }
      cache = 1;
      cx = x2;
      cy = y2;
  }
  glVertex2f(x2,y2);
  glVertex2f(fx,fy);

  glEnd();
}

float calculate_radius(int x1,int y1)
{
  int x_diff = mX - x1;
  int y_diff = mY - y1;
    if(x_diff<0)
    {
      x_diff *= -1;
    }
    if(y_diff<0)
    {
      y_diff *= -1;
    }
    float mag = x_diff*x_diff + y_diff*y_diff;
    float rad = sqrt(mag);
    return rad;
}

void drawRectangle()
{
  glColor3fv(gColor);
  glLineWidth(thickness);
  glBegin(GL_LINES);

  for(int i=0; i<=numRect; i++)
  {
    glVertex2i(rect[i][0].x,rect[i][0].y);
    glVertex2i(rect[i][1].x,rect[i][0].y);
    glVertex2i(rect[i][0].x,rect[i][0].y);
    glVertex2i(rect[i][0].x,rect[i][1].y);
    glVertex2i(rect[i][0].x,rect[i][1].y);
    glVertex2i(rect[i][1].x,rect[i][1].y);
    glVertex2i(rect[i][1].x,rect[i][0].y);
    glVertex2i(rect[i][1].x,rect[i][1].y);
  }

  glEnd();
}

void drawlines()
{
  glColor3fv(gColor);
  glLineWidth(thickness);
  glBegin(GL_LINES);

  for(int i=0; i<=numLines; i++)
  {
    glVertex2i(lines[i][0].x,lines[i][0].y);
    glVertex2i(lines[i][1].x,lines[i][1].y);
  }

  glEnd();
}

void displayFcn (void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
   	glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   /*  Set example number of control points and number of
    *  curve positions to be plotted along the Bezier curve.
    */
    drawlines();
    drawRectangle();
    for(int i=0; i<pointSize; i+=2)
    {
      int rad_index = (int)(i/2);
      float rad = radiusArray[rad_index];
      if (rad>1)
      {
        drawCircle(rad,pointArray[i][0],pointArray[i][1]);
      }
    }

    glutSwapBuffers();
  	glLineWidth(1.0);

   	glBegin(GL_LINES);
      glColor3f (0.0, 0.0, 1.0);
	    glVertex2i(0, 0);
	    glVertex2i((-winWidth* 3), 0);
	    glVertex2i(0, 0);
	    glVertex2i((winWidth * 3), 0);
	    glVertex2i(0, 0);
	    glVertex2i(0, (-winHeight * 3));
	    glVertex2i(0, 0);
	    glVertex2i(0, (winHeight * 3));
  	glEnd();
  	glFlush();

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );
   gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax); 
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ( );
   movex = movex / 5;
   movey = movey / 5;
   glTranslatef(movex,movey,0.0);
   glutSwapBuffers();
   //glutPostRedisplay();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   /*  Maintain an aspect ratio of 1.0.  */
  glViewport (0, 0, newHeight, newHeight);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ( );

  gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

  glutPostRedisplay();
}

void
processMenuEvents(int option)
{
    switch(option)
    {
    	case QUIT : exit(0); break;
    	case THICKER : thickness +=1 ; break;
    	case THINNER : thickness -=1 ; break;
      case RED 	   : gColor[0]=1,gColor[1]=0,gColor[2]=0; break;
      case GREEN 	 : gColor[0]=0,gColor[1]=1,gColor[2]=0; break;
      case BLUE 	 : gColor[0]=0,gColor[1]=0,gColor[2]=1; break;
      case BLACK 	 : gColor[0]=0,gColor[1]=0,gColor[2]=0; break;
    }
}

void
createGLUTMenus()
{
	int menu,submenuSize,submenuColor;

	submenuColor=glutCreateMenu(processMenuEvents);
	  glutAddMenuEntry("Red",RED);
	  glutAddMenuEntry("Green",GREEN);
	  glutAddMenuEntry("Blue",BLUE);
	  glutAddMenuEntry("Black",BLACK);

	submenuSize=glutCreateMenu(processMenuEvents);
	  glutAddMenuEntry("Thicken",THICKER);
	  glutAddMenuEntry("Thin",THINNER);

	menu=glutCreateMenu(processMenuEvents);
	  glutAddSubMenu("Colour",submenuColor);
	  glutAddSubMenu("Size",submenuSize);
	  glutAddMenuEntry("Quit",QUIT);
	  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init()
{
  glClearColor(0,0,0,1);
  numLines=-1;
  numRect=-1;
  createGLUTMenus();
}

void mouseclick(int button, int state,int x, int y)
{
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble worldX, worldY, worldZ;
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );
  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  winZ = 0;
  gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

  if (state == GLUT_DOWN && circledraw == true && button==GLUT_LEFT_BUTTON)
  {
    downActive = 1;
  }

  if (state == GLUT_UP && circledraw == true)
  {
    if(downActive)
    {
      pointArray[pointSize][0]=worldX;
      pointArray[pointSize][1]=worldY;
      pointSize++;
      downActive = 0;
      rubberBandOn = !rubberBandOn;
    }
    glutIdleFunc(NULL);
  }

  if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
  {
    if(linedraw == true)
    {
      switch(gState)
      {
        case waitOnClick:
          ++numLines;
          lines[numLines][0].x=worldX;
          lines[numLines][0].y=worldY; 
          lines[numLines][1].x=worldX;
          lines[numLines][1].y=worldY;
          gState++;
        break;

        case clickedonce:
          lines[numLines][1].x=worldX;
          lines[numLines][1].y=worldY;
          gState=waitOnClick;
        break;
      }
    }

    else if(rectangledraw == true)
    {
      switch(gState)
      {
      case waitOnClick:
        ++numRect;
        rect[numRect][0].x=worldX;
        rect[numRect][0].y=worldY; 
        rect[numRect][1].x=worldX;
        rect[numRect][1].y=worldY;
        gState++;
      break;

      case clickedonce:
        rect[numRect][1].x=worldX;
        rect[numRect][1].y=worldY;
        gState=waitOnClick;
      break;
      }
    }
  }

  else
  {
    MiddleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));
    downX = worldX; 
    downY = worldY;
    int mod = glutGetModifiers(); 
    if (mod == GLUT_ACTIVE_CTRL)
    {
      if (button == MOUSE_SCROLL_UP)
      {
        xwcMin += 5;
        ywcMin += 5;  
      }
      else if(button == MOUSE_SCROLL_DOWN)
      {
        xwcMin -= 5;
        ywcMin -= 5;
      }
    }
  }
  glutPostRedisplay();
}

void MotionCallback(int x, int y)
{
  if (MiddleButton)
  {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble worldX, worldY, worldZ;
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    winZ = 0;
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    movex =worldX - x;
    movey= worldY - y;
    //glutPostRedisplay(); causes buggy visuals when middlemouse dragging
  }
}

void mouseDrag(int x, int y)
{
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16]; 
  GLfloat winX, winY, winZ; 
  GLdouble worldX, worldY, worldZ;
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); 
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );
  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  winZ = 0;
  gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
  if(circledraw == true)
  {
    mX = worldX;
    mY = worldY;
    if (rubberBandOn)
    {
      float rd = calculate_radius(pointArray[pointSize-1][0],pointArray[pointSize-1][1]);
      radiusArray[pointSize/2] = rd;
    }
    displayFcn();
  }

  if(linedraw == true)
  {
    if(gState==clickedonce)
    {
      lines[numLines][1].x=worldX;
      lines[numLines][1].y=worldY;
      glutPostRedisplay();
    }
  }

  else if(rectangledraw == true)
  {
    if(gState==clickedonce)
    {
      rect[numRect][1].x=worldX;
      rect[numRect][1].y=worldY;
      glutPostRedisplay();
    }
  }
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 'l' || key == 'L') 
  {
    linedraw = true;
    circledraw = false;
    rectangledraw = false;
  }
  if (key == 'c' || key == 'C') 
  {
    linedraw = false;
    circledraw = true;
    rectangledraw = false;
  }
  if (key == 'r' || key == 'R') 
  {
    linedraw = false;
    circledraw = false;
    rectangledraw = true;
  }
}

int main(int argc, char ** argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
glutInitWindowPosition(100,100);
glutInitWindowSize(500,500);
glutCreateWindow("My Drawing App");
glutReshapeFunc(winReshapeFcn);
glutDisplayFunc(displayFcn);
glutKeyboardFunc(keyboard);
glutMouseFunc(mouseclick);
glutPassiveMotionFunc(mouseDrag);
glutMotionFunc(MotionCallback);
init();

glutMainLoop();
return 0;
} 