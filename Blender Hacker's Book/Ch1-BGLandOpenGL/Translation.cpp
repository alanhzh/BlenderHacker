#include<gl/glut.h>
#include<stdlib.h>

void Draw_triangle(void)
{
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	//glBegin(GL_TRIANGLES);
	glVertex3i(1,0,0);
	glVertex3i(0,1,0);
	glVertex3i(-1,0,0);
	glVertex3i(1,0,0);
	glEnd();
	glFlush();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);//
	Draw_triangle();//Original

	glColor3f(1.0,0.0,0.0);//Red
	glTranslatef(-0.2,-0.2,-6.0);//X,Y,Z translating!
	Draw_triangle();//Moved

	glColor3f(0.0,1.0,0.0);//Green
	glTranslatef(-0.2,-0.2,-1.5);//X,Y,Z translating!
	glScalef(2.0,2.0,2.0);//X,Y,Z translated and scaling!
	Draw_triangle();//Scaled

	glColor3f(0.0,0.0,1.0);//Blue
	glTranslatef(-0.2,-0.2,-0.5);//X,Y,Z translating!
	glRotatef(30,0.0,1.0,0.0);//X,Y,Z Rotating!
	Draw_triangle();//Rotated

	glutSwapBuffers();
}

void init(int w, int h)
{ 
	//Basic View==change to modelview matrix to prepare for draw this scene
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(-2.0,2.0,-2.0,2.0,-2.0,2.0);
	
	//Projecton View==set projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0,1.0,-1.0,1.0,1.0,10.0);
	gluPerspective(45.0,(GLfloat)w/(GLfloat)h,1.0,150.0);
	
	//Viewport View
	glViewport(0,0, (GLsizei)w, (GLsizei)h);//Windows' w and h
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Translation");
	init(500,500);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}






