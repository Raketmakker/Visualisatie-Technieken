#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#pragma comment(lib, "glew32.lib")

#include "Shader.h"

Shader* simple;
Shader* grey;
Shader* toy;

glm::ivec2 screenSize;

float rotation;
int lastTime;

void drawSquare();

class Vertex
{

	public:
		glm::vec3 position;
		glm::vec4 color;
		
		Vertex(const glm::vec3 &position, const glm::vec4 &color) : position(position), color(color) {}
};

#ifdef WIN32
	void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
	void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}

void init()
{

	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glClearColor(1, 0.7f, 0.3f, 1.0f);

	simple = new Shader("simple");
	grey = new Shader("grey");
	toy = new Shader("toy");

	glEnableVertexAttribArray(0);							// we gebruiken vertex attribute 0
	glEnableVertexAttribArray(1);							// en vertex attribute 1

	if (glDebugMessageCallback)
	{

		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}

	rotation = 0;
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mvp = glm::perspective(80.0f, screenSize.x / (float)screenSize.y, 0.01f, 100.0f);		//begin met een perspective matrix
	
	simple->use();
	mvp *= glm::lookAt(glm::vec3(0, -2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	mvp = glm::translate(mvp, glm::vec3(0, 0, -1));													//verplaats de camera gewoon naar achter
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 1, 0));											//roteer het object een beetje
	
	simple->setUniform("modelViewProjectionMatrix", mvp);
	simple->setUniform("time", (int)(lastTime / 1000.0f));
	
	drawSquare();

	grey->use();
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, -1, 0));
	mvp = glm::translate(mvp, glm::vec3(5, 0, 0));
	mvp = glm::rotate(mvp, rotation, glm::vec3(1, 0, 0));
	
	grey->setUniform("modelViewProjectionMatrix", mvp);
	grey->setUniform("time", (int)(lastTime / 1000.0f));															//en tekenen :)

	drawSquare();

	toy->use();
	mvp = glm::rotate(mvp, rotation, glm::vec3(-1, 0, 0));
	mvp = glm::translate(mvp, glm::vec3(-10, 0, 0));
	mvp = glm::rotate(mvp, rotation, glm::vec3(0, 0, 1));
	
	toy->setUniform("modelViewProjectionMatrix", mvp);
	toy->setUniform("time", (int)(lastTime / 1000.0f));

	drawSquare();

	glutSwapBuffers();
}

void drawSquare()
{

	Vertex vertices[] = {
		Vertex(glm::vec3(-1, -1,  1), glm::vec4(1, 1, 0, 1)),
		Vertex(glm::vec3( 1, -1,  1), glm::vec4(1, 1, 0, 1)),
		Vertex(glm::vec3( 1,  1,  1), glm::vec4(1, 1, 0, 1)),
		Vertex(glm::vec3(-1,  1,  1), glm::vec4(1, 1, 0, 1)),
		
		Vertex(glm::vec3( 1, -1,  1), glm::vec4(0, 1, 1, 1)),
		Vertex(glm::vec3( 1, -1, -1), glm::vec4(0, 1, 1, 1)),
		Vertex(glm::vec3( 1,  1, -1), glm::vec4(0, 1, 1, 1)),
		Vertex(glm::vec3( 1,  1,  1), glm::vec4(0, 1, 1, 1)),
		
		Vertex(glm::vec3(-1,  1,  1), glm::vec4(1, 0, 1, 1)),
		Vertex(glm::vec3( 1,  1,  1), glm::vec4(1, 0, 1, 1)),
		Vertex(glm::vec3( 1,  1, -1), glm::vec4(1, 0, 1, 1)),
		Vertex(glm::vec3(-1,  1, -1), glm::vec4(1, 0, 1, 1)),
		
		Vertex(glm::vec3(-1, -1, -1), glm::vec4(0, 0, 1, 1)),
		Vertex(glm::vec3( 1, -1, -1), glm::vec4(0, 0, 1, 1)),
		Vertex(glm::vec3( 1,  1, -1), glm::vec4(0, 0, 1, 1)),
		Vertex(glm::vec3(-1,  1, -1), glm::vec4(0, 0, 1, 1)),
		
		Vertex(glm::vec3(-1, -1,  1), glm::vec4(1, 0, 0, 1)),
		Vertex(glm::vec3(-1, -1, -1), glm::vec4(1, 0, 0, 1)),
		Vertex(glm::vec3(-1,  1, -1), glm::vec4(1, 0, 0, 1)),
		Vertex(glm::vec3(-1,  1,  1), glm::vec4(1, 0, 0, 1)),
		
		Vertex(glm::vec3(-1, -1, -1), glm::vec4(0, 1, 0, 1)),
		Vertex(glm::vec3( 1, -1, -1), glm::vec4(0, 1, 0, 1)),
		Vertex(glm::vec3( 1, -1,  1), glm::vec4(0, 1, 0, 1)),
		Vertex(glm::vec3(-1, -1,  1), glm::vec4(0, 1, 0, 1))
	};

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), vertices);									//geef aan dat de posities op deze locatie zitten
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &vertices[0].color);					//geef aan dat de kleuren op deze locatie zitten
	glDrawArrays(GL_QUADS, 0, sizeof(vertices) / sizeof(Vertex));
}

void reshape(int newWidth, int newHeight)
{

	screenSize.x = newWidth;
	screenSize.y = newHeight;
	
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	if (key == VK_ESCAPE)
		glutLeaveMainLoop();
}

void update()
{

	int time = glutGet(GLUT_ELAPSED_TIME);
	int elapsed = time - lastTime;
		
	rotation += elapsed / 1000.0f;

	glutPostRedisplay();

	lastTime = time;
}

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitWindowSize(1900, 1000);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Visualisatietechnieken");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);

	init();
	
	glutMainLoop();
}