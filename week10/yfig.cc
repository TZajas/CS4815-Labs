#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;  

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;
int downX, downY;
int newX=0, newY=0;
bool leftButton = false;
float zoomIn = 1.2;
float zoomOut = 0.8;

class wcPt3D {
   public:
      GLfloat x, y, z;
};

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}


void displayFcn (void)
{
   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   //glPointSize (4);
   glBegin(GL_LINES){






   } 
   glColor3f (1.0, 0.0, 0.0); //  Set point color to red.

   glutSwapBuffers();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   /*  Maintain an aspect ratio of 1.0.  */
   if(newWidth>newHeight){
        	glViewport (0, 0, newHeight, newHeight);
   }else{
		glViewport (0, 0, newWidth, newWidth);
   }

   winWidth = newWidth;
   winHeight = newHeight;

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );

   gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

   glutPostRedisplay();
}

void MouseCallback(int button, int state, int x, int y)
{
  downX = x; downY = y;
  leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
  glutPostRedisplay();
	
	if((button == GLUT_WHEEL_UP) && (glutGetModifiers()&&GLUT_ACTIVE_CTRL)){
		glScalef(zoomIn,zoomIn,0.0);
		glutPostRedisplay();
	}
	
	if((button == GLUT_WHEEL_DOWN) && (glutGetModifiers()&&GLUT_ACTIVE_CTRL)){
                glScalef(zoomOut,zoomOut,0.0);
                glutPostRedisplay();
        }

}
 
void MotionCallback(int x, int y)
{
  newX +=  x-downX;
  newY -= y-downY;
  if (leftButton){
	glViewport (newX, newY, winWidth, winHeight);//drag
  }
   // zoom
  downX = x;   downY = y; 
  glutPostRedisplay();
}

int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   glutMouseFunc(MouseCallback);
   glutMotionFunc(MotionCallback);
   glutMainLoop ( );
}
