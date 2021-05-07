#include "OpenGLRenderer.h"
#include "IRenderer.h"

#include <GL/glew.h>
#include <GL/glut.h>

OpenGLRenderer::OpenGLRenderer()
{	
	
}

OpenGLRenderer::~OpenGLRenderer()
{
}

bool OpenGLRenderer::Init()
{
	//Use depth buffering for hidden surface elimination
	glEnable(GL_DEPTH_TEST);
	//Setup the view
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, //Field of view in degree
		1.0, //Aspect ratio
		1.0, //Z near
		10.0); //Z far
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, 5.0, //Eye is at (0,0,5)
		0.0, 0.0, 0.0,   //Center is at (0,0,0) 
		0.0, 1.0, 0.);   //Up is in positive Y direction

	//Adjust cube position
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	glRotatef(-20, 0.0, 0.0, 1.0);
	return 0;
}

int OpenGLRenderer::Run()
{
	glutInit(&argc, argv);//Initialize GLUT and process user parameters
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//Request double buffered true color window with Z-buffer
	glutCreateWindow("OpenGL Cube");//Create Presentation Window
	glEnable(GL_DEPTH_TEST);//Enable Z-buffer depth test
	Init();
	Display();
	glutMainLoop();
	return 0;
}

void OpenGLRenderer::Init_Graphics()
{
	//Cube Front 
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 1.0);		glVertex3f(0.5, -0.5, 0.5);		//Purple
	glColor3f(0.0, 1.0, 0.0);		glVertex3f(0.5, 0.5, -0.5);		//Green
	glColor3f(0.0, 0.0, 1.0);		glVertex3f(-0.5, 0.5, -0.5);	//Blue
	glColor3f(0.0, 1.0, 1.0);		glVertex3f(-0.5, -0.5, -0.5);	//Cyan
	glEnd();

	//Cube Back
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 1.0);		glVertex3f(0.5, -0.5, 0.5);
	glColor3f(0.0, 1.0, 0.0);		glVertex3f(0.5, 0.5, 0.5);
	glColor3f(0.0, 0.0, 1.0);		glVertex3f(-0.5, 0.5, 0.5);
	glColor3f(0.0, 1.0, 1.0);		glVertex3f(-0.5, -0.5, 0.5);
	glEnd();

	//De3ia Roz
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 1.0);		glVertex3f(0.5, -0.5, -0.5);
									glVertex3f(0.5, 0.5, -0.5);
									glVertex3f(0.5, 0.5, 0.5);
									glVertex3f(0.5, -0.5, 0.5);
	glEnd();

	//Aristera Prasino
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);		glVertex3f(-0.5, -0.5, 0.5);
									glVertex3f(-0.5, 0.5, 0.5);
									glVertex3f(-0.5, 0.5, -0.5);
									glVertex3f(-0.5, -0.5, -0.5);
	glEnd();

	//Panw Mple
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);		glVertex3f(0.5, 0.5, 0.5);
									glVertex3f(0.5, 0.5, -0.5);
									glVertex3f(-0.5, 0.5, -0.5);
									glVertex3f(-0.5, 0.5, 0.5);
	glEnd();

	//Katw Roz
	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 1.0);		glVertex3f(0.5, -0.5, -0.5);
									glVertex3f(0.5, -0.5, 0.5);
									glVertex3f(-0.5, -0.5, 0.5);
									glVertex3f(-0.5, -0.5, -0.5);
	glEnd();
}

void  OpenGLRenderer::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Init_Graphics();
	glutSwapBuffers();
}


