
#include <stdlib.h>
#include <GL/glut.h>

const double M = 1;
const double N = 1.5;

GLfloat vertices[][3] = {{0.0,M},{-1*M,0.0},
						{0.0,-2*M}, {2*M,0.0}};

GLfloat colors[][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},
					  {0.0,0.0,1.0}, {0.0,0.0,0.0}};

void polygon(int a, int b, int c , int d)
{

 	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		glVertex2fv(vertices[a]);
		glColor3fv(colors[b]);
		glVertex2fv(vertices[b]);
		glColor3fv(colors[c]);
		glVertex2fv(vertices[c]);
		glColor3fv(colors[d]);
		glVertex2fv(vertices[d]);
	glEnd();
																										}

void colorPoly(void)
{

	polygon(0,1,2,3);

}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

 	colorPoly();

 	glFlush();
	glutSwapBuffers();

}

/* 
void mouse(int btn, int state, int x, int y)
{

	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}
*/

void myReshape(int w, int h)
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
    	glOrtho(-2.0,2.0,-2.0*(GLfloat)h/(GLfloat)w,
            2.0*(GLfloat)h/(GLfloat)w,-10.0,10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
            2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);

}

int
main(int argc, char **argv)
{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(650, 650);
    glutCreateWindow("Coloured Poly");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	//glutMouseFunc(mouse);
	glEnable(GL_DEPTH_TEST);

    glutMainLoop();

}