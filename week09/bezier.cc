#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
#endif

//  Set initial size of the display window.
GLsizei winWidth = 600, winHeight = 600;  

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin, xwcMax;
GLfloat ywcMin, ywcMax;

float downX, downY, newX, newY;
bool leftButton = false;
int point;
bool inRadius = true;
float zoomIn = 1.2;
float zoomOut = 0.8;

class wcPt3D {
   public:
      GLfloat x, y, z;
};

wcPt3D ctrlPts [6] ={ {-40, -40, 0}, {-10, 100, 0},
                      {10, -100, 0}, {100, 20, 0},
                      {100, 50, 0}, {150, 60, 0}
                      };


GLint nCtrlPts = 6;

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}

void MouseCallback(int button, int state, int x, int y)
{
  downX = x; downY = y;
  leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));


  float distance;
  

  for(int i=0;i<6;i++)
  {
	distance = sqrt(pow(downX - ctrlPts[i].x, 2) +
                   pow(downY - ctrlPts[i].y, 2) * 1.0);
        if(distance<0.3){	
		point = i;
		inRadius = true;
	}
  }

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
  //newX +=  x-downX;
  //newY -= y-downY;
   if(leftButton){
	if(inRadius){
		newX +=  x-downX;
                newY -= y-downY;
		ctrlPts[point].x=newX;
                ctrlPts[point].y=winHeight-newY;
		glutPostRedisplay();
	}else{
		newX +=  x-downX;
  		newY -= y-downY;
   	 	glutPostRedisplay();
    	}	
   }
  //glutPostRedisplay();
}
void setClippingWindow(void){
	
	xwcMin = ctrlPts[0].x;
	xwcMax = ctrlPts[0].x;
	ywcMin = ctrlPts[0].y;
	ywcMax = ctrlPts[0].y;
	for(int i=0;i<nCtrlPts;i++){
		if(xwcMin>ctrlPts[i].x){
			xwcMin = ctrlPts[i].x;
		}

		if(xwcMax<ctrlPts[i].x){
                        xwcMax = ctrlPts[i].x;
                }

		if(ywcMin>ctrlPts[i].y){
                        ywcMin = ctrlPts[i].y;
                }
		
		if(ywcMax<ctrlPts[i].y){
                        ywcMin = ctrlPts[i].y;
                }

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
   GLint nBezCurvePts = 1000;
    glColor3f (0.0, 0.0, 1.0);
    glBegin (GL_LINES);
        glVertex2f (ctrlPts[0].x,ctrlPts[0].y);
	for(int i = 1;i<nCtrlPts-1;i++){
		glVertex2f (ctrlPts[i].x,ctrlPts[i].y);
		glVertex2f (ctrlPts[i].x,ctrlPts[i].y);
	}
        glVertex2f (ctrlPts[nCtrlPts-1].x,ctrlPts[nCtrlPts-1].y);
    glEnd ( );
    glFlush( );

   glPointSize (6);
   glColor3f (1.0, 0.0, 0.0);      //  Set point color to red.

   bezier (ctrlPts, nCtrlPts, nBezCurvePts);
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

int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");
   setClippingWindow( );
   glutMouseFunc(MouseCallback);
   glutMotionFunc(MotionCallback);
   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);

   glutMainLoop ( );
}
