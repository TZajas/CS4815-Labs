#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif
#define THIN 5
#define MID 6
#define THICK 7
#define BLUE 8
#define GREEN 9
#define RED 10
#define QUIT 11


/*  Set initial size of the display window.  */
GLsizei winWidth = 1000, winHeight = 1000;  

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;
float red=0.0, green=0.0, blue=1.0;
int downX, downY;
int newX=0, newY=0;
bool leftButtonUp = false;
bool leftButton = false;
float zoomIn = 1.2;
float zoomOut = 0.8;
bool linedraw = false;
bool circledraw = false;
bool rectangledraw = false;
float thickness = 5.0;
int downActive = 0;
int clickCount = 0;
int recClickCount = 0;
int circleClickCount = 0;

class Points{
   public:
      int x, y;
};

Points linePoint [2];
Points recPoint [2];
Points cirPoint [2];

void drawCircle(){
	int x1 = cirPoint[0].x;
	int y1 = cirPoint[0].y;
	int x2 = cirPoint[1].x;
	int y2 = cirPoint[1].y;
        int r = sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);

		glColor3f (red, green, blue);
                glLineWidth(thickness);

       		glBegin(GL_LINE_LOOP);
   		for(int i = 0; i < 200; i++){
        		float theta = 2.0f * 3.1415926f * float(i) / float(50);//get the current angle

		        float x = r * cosf(theta);//calculate the x component
       			float y = r * sinf(theta);//calculate the y component

        		glVertex2f(x + x1, y + y1);//output vertex

       		}
       		glEnd(); 
		glFlush();
}

void drawLine(){
	glColor3f (red, green, blue);
  	glLineWidth(thickness);
  	glBegin(GL_LINES);  	
	  glVertex2f(linePoint[0].x, linePoint[0].y);
    	  glVertex2f(linePoint[1].x, linePoint[1].y);  	
  	glEnd();
	glFlush();
}

void drawRectangle(){
	glColor3f (red, green, blue);
        glLineWidth(thickness);
        glBegin(GL_LINES);      
          glVertex2f(recPoint[0].x, recPoint[0].y);
	  glVertex2f(recPoint[0].x, recPoint[1].y);

	  glVertex2f(recPoint[0].x, recPoint[1].y);
          glVertex2f(recPoint[1].x, recPoint[1].y);

          glVertex2f(recPoint[1].x, recPoint[1].y);
          glVertex2f(recPoint[1].x, recPoint[0].y);

          glVertex2f(recPoint[1].x, recPoint[0].y);
          glVertex2f(recPoint[0].x, recPoint[0].y);
        glEnd();
        glFlush();

}

int getDistance(int x1, int y1, int x2, int y2)
{
    // Calculating distance
    return sqrt(pow(x2 - x1, 2) +
                pow(y2 - y1, 2) * 1.0);
}

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}

/*void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
        glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}*/

/*  Compute binomial coefficients C for given value of n.  */

void displayFcn (void)
{
   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   /*  Set example number of control points and number of
    *  curve positions to be plotted along the Bezier curve.
    */
    /*if(clickCount == 1){
	drawLine();
    }*/
    if(clickCount>1){
    	drawLine();
    }
    if(recClickCount>1){
	drawRectangle();	
    }

    if(circleClickCount>1){
	drawCircle();
    }
    glLineWidth(1.0);
    glColor3f (0.0, 0.0, 0.1);
    glBegin (GL_LINES);
	glVertex2f(-1000, 0);
	glVertex2f(+1000, 0);
	glVertex2f(0, -1000);
	glVertex2f(0, 1000);
    glEnd ( );
    glFlush( );

    	
    //drawLine();



   glPointSize (4);
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


void processMenuEvents(int option) {

        switch (option) {
                case THIN : thickness = 3.0 ; break;
                case MID : thickness = 6.0 ; break;
                case THICK : thickness = 9.0 ; break;
		case RED : red = 1.0; green = 0.0; blue = 0.0; break;
                case GREEN : red = 0.0; green = 1.0; blue = 0.0; break;
                case BLUE : red = 0.0; green = 0.0; blue = 1.0; break;
		case QUIT : exit(0); break;
        }
}

void createGLUTMenus() {

        int menu,submenu1,submenu2;
	
	submenu2 = glutCreateMenu(processMenuEvents); 
	glutAddMenuEntry("Red",RED);
        glutAddMenuEntry("Blue",BLUE);
        glutAddMenuEntry("Green",GREEN);

        submenu1 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Thin Line",THIN);
        glutAddMenuEntry("Medium Line",MID);
        glutAddMenuEntry("Thick Line",THICK); 

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Line Thickeness",submenu1);
        glutAddSubMenu("Line Colour",submenu2);
	glutAddMenuEntry("Quit",QUIT);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
	
}
void MouseCallback(int button, int state, int x, int y)
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

	downX = x; downY = y;
  	leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
  	glutPostRedisplay();
	
	if (state == GLUT_DOWN && circledraw == true && button==GLUT_LEFT_BUTTON){
    		downActive = 1;
  	}

        leftButtonUp = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP));

	
	if(leftButtonUp){
	     if(linedraw){

		if(clickCount>1){
			clickCount=0;	
		}
		linePoint[clickCount].x =  worldX;
		linePoint[clickCount].y =  worldY;

		clickCount++;
              }else if(rectangledraw){
                  if(recClickCount>1){
                        recClickCount=0;   
                   }
		recPoint[recClickCount].x =  worldX;
                recPoint[recClickCount].y =  worldY;
	        recClickCount++;
	      }else if(circledraw){
		if(circleClickCount>1){
			circleClickCount =0;
		}
		cirPoint[circleClickCount].x = worldX;
		cirPoint[circleClickCount].y = worldY;
		circleClickCount++;
	      }
	}

	if((button == GLUT_WHEEL_UP) && (glutGetModifiers()&&GLUT_ACTIVE_CTRL)){
		glScalef(zoomIn,zoomIn,0.0);
		glutPostRedisplay();
	}
	
	if((button == GLUT_WHEEL_DOWN) && (glutGetModifiers()&&GLUT_ACTIVE_CTRL)){
                glScalef(zoomOut,zoomOut,0.0);
                glutPostRedisplay();
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

 
void MotionCallback(int x, int y)
{
  newX +=  x-downX;
  newY -= y-downY;
 /* if (leftButton){
	glViewport (newX, newY, winWidth, winHeight);//drag
  }
   // zoom
  downX = x;   downY = y;*/ 
  glutPostRedisplay();
}


int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (0, 0);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("yfig");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(MouseCallback);
   glutMotionFunc(MotionCallback);
   createGLUTMenus();
   glutMainLoop ( );
}
