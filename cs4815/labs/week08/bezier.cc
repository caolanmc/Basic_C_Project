#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin, xwcMax;
GLfloat ywcMin, ywcMax;

bool leftButton = false;
int point = 10;
float downX, downY,downXnew, downYnew;

class wcPt3D {
   public:
      GLfloat x, y, z;
};

wcPt3D ctrlPts[6]={ 
                    {-50.0,-50.0,0.0},{0.0,100.0,0.0},
                    {20.0,-100.0,0.0},{50.0,50.0,0.0},
                    {50.0,70.0,0.0},{40.0,60.0,0.0}
                  };

GLint nCtrlPts = 6, nBezCurvePts = 1000;

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}

void clippingwindow(void)
{
  float xval [6];
  float yval [6];
  int i;

  for(i = 0;i<6;i++)
  {
    xval[i] = ctrlPts[i].x;
  }

  for(i = 0;i<6;i++)
  {
    yval[i] = ctrlPts[i].y;
  }

  xwcMin = xval[0];
  xwcMax = xval[0];
  ywcMin = yval[0];
  ywcMax = yval[0];

  for(i = 1;i<6;i++)
  {
    if(xwcMin > xval[i])
      xwcMin = xval[i];
    if(xwcMax < xval[i])
      xwcMax = xval[i];
    if(ywcMin > yval[i])
      ywcMin = yval[i];
    if(ywcMax < yval[i])
      ywcMax = yval[i];
  }
}

void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
        glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs (GLint n, GLint * C)
{
   GLint k, j;

   for (k = 0;  k <= n;  k++) {
      /*  Compute n!/(k!(n - k)!).  */
      C [k] = 1;
      for (j = n;  j >= k + 1;  j--)
        C [k] *= j;
      for (j = n - k;  j >= 2;  j--)
        C [k] /= j;
   }
}

void computeBezPt (GLfloat t, wcPt3D * bezPt, GLint nCtrlPts,
                    wcPt3D * ctrlPts, GLint * C)
{
   GLint k, n = nCtrlPts - 1;
   GLfloat bezBlendFcn;

   bezPt->x = bezPt->y = bezPt->z = 0.0;

   /*  Compute blending functions and blend control points. */
   for (k = 0; k < nCtrlPts; k++) {
      bezBlendFcn = C [k] * pow (t, k) * pow (1 - t, n - k);
      bezPt->x += ctrlPts [k].x * bezBlendFcn;
      bezPt->y += ctrlPts [k].y * bezBlendFcn;
      bezPt->z += ctrlPts [k].z * bezBlendFcn;
   }
}

void bezier (wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
   wcPt3D bezCurvePt;
   GLfloat t;
   GLint *C;

   /*  Allocate space for binomial coefficients  */
   C = new GLint [nCtrlPts];

   binomialCoeffs (nCtrlPts - 1, C);
   for (int i = 0;  i <= nBezCurvePts;  i++) {
      t = GLfloat (i) / GLfloat (nBezCurvePts);
      computeBezPt (t, &bezCurvePt, nCtrlPts, ctrlPts, C);
      plotPoint (bezCurvePt);
   }
   delete [ ] C;
}

void displayFcn (void)
{
   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   /*  Set example number of control points and number of
    *  curve positions to be plotted along the Bezier curve.
    */
  glColor3f (0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex2f(ctrlPts[0].x, ctrlPts[0].y);
    glVertex2f(ctrlPts[1].x, ctrlPts[1].y);
    glVertex2f(ctrlPts[1].x, ctrlPts[1].y);
    glVertex2f(ctrlPts[2].x, ctrlPts[2].y);
    glVertex2f(ctrlPts[2].x, ctrlPts[2].y);
    glVertex2f(ctrlPts[3].x, ctrlPts[3].y);
    glVertex2f(ctrlPts[3].x, ctrlPts[3].y);
    glVertex2f(ctrlPts[4].x, ctrlPts[4].y);
    glVertex2f(ctrlPts[4].x, ctrlPts[4].y);
    glVertex2f(ctrlPts[5].x, ctrlPts[5].y);
  glEnd();
  glFlush();

   glPointSize (6);
   glColor3f (0.5, 0.0, 0.5);      //  Set point color to purple.

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   gluOrtho2D(xwcMin,xwcMax,ywcMin,ywcMax);
   glutPostRedisplay();

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ( );

   glTranslatef(downXnew,downYnew,0.0);

   bezier (ctrlPts, nCtrlPts, nBezCurvePts);

   glutSwapBuffers();
   glutPostRedisplay();
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

void keyboard(unsigned char key,int x, int y)
{
  if(key=='z')
  {
    xwcMin+=5;
    ywcMin+=5;
  }
  if(key=='Z')
  {
    xwcMin-=5;
    ywcMin-=5;
  }
}

void MouseCallback(int button, int state, int x, int y)
{
  downX=x; downY=y;
  leftButton=((button==GLUT_LEFT_BUTTON)&&(state==GLUT_DOWN));

  int i=0;
  point=10;
  float tempX=0,tempY=0;
  float tempX2=0,tempY2=0;

  tempX=(float)x;
  tempY=(float)winHeight-y;
  float distX,distY;

  for(;i<nCtrlPts;i++)
  {
    tempX2=(float)winWidth-ctrlPts[i].x;
    tempY2=(float)winHeight-ctrlPts[i].y;
    distX=(float)tempX2/tempX;
    distY=(float)tempY2/tempY;

      if(distX<=1.3&&distY<=1.3)
        point=i;
  }

  glutPostRedisplay();

  if(glutGetModifiers() && GLUT_ACTIVE_CTRL)
  {

    if(button == GLUT_WHEEL_UP)
    {
      xwcMin+=5;
      ywcMin+=5;
      glutPostRedisplay();
    }
    if(button == GLUT_WHEEL_DOWN)
    {
      xwcMin-=5;
      ywcMin-=5;
      glutPostRedisplay();
    }
  }
}

void MotionCallback(int x, int y)
{
  if(leftButton)
  {
    if(point==10)
    {
      downXnew=x-downX;
      downYnew=downY-y;
    }
    else
    {
      ctrlPts[point].x=(float)x;
      ctrlPts[point].y=(float)winHeight-y;
      glutPostRedisplay();
    }
  }
}

int main (int argc, char** argv)
{
   clippingwindow();
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");

   glutMouseFunc(MouseCallback);
   glutKeyboardFunc(keyboard);
   glutMotionFunc(MotionCallback);

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);

   glutMainLoop ( );
}
