#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "ObjModel.h"
#include "Texture.h"
#include "Shader.h"
#include "Fbo.h"

#pragma comment(lib, "glew32.lib")

Shader* roomShader;
ObjModel* room;

std::vector<Shader*> shaders;
std::vector<Shader*> postProcessShaders;
std::vector<ObjModel*> models;
std::vector<float> distances;

Fbo* fbo;
int activeModel = 0;

int currentShader;
int currentPostShader;

glm::ivec2 screenSize;

float rotation;
int lastTime;

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

	room = new ObjModel("assets/models/room/room.obj");
	roomShader = new Shader("assets/shaders/texture");

	currentShader = 0;
	currentPostShader = 0;

	// original
	shaders.push_back(new Shader("assets/shaders/pre/simple"));
	shaders.push_back(new Shader("assets/shaders/pre/multitex"));
	shaders.push_back(new Shader("assets/shaders/pre/textureanim"));
	shaders.push_back(new Shader("assets/shaders/pre/texture"));
	shaders.push_back(new Shader("assets/shaders/pre/vertexanim"));

	// self added
	shaders.push_back(new Shader("assets/shaders/pre/greyScale"));
	shaders.push_back(new Shader("assets/shaders/pre/noiseFade"));
	shaders.push_back(new Shader("assets/shaders/pre/noiseSingleColor"));
	shaders.push_back(new Shader("assets/shaders/pre/noiseColor"));
	shaders.push_back(new Shader("assets/shaders/pre/noiseGrey"));
	shaders.push_back(new Shader("assets/shaders/pre/toon"));
	shaders.push_back(new Shader("assets/shaders/pre/explode"));

	// original
	postProcessShaders.push_back(new Shader("assets/shaders/post/postprocess"));

	// self added
	postProcessShaders.push_back(new Shader("assets/shaders/post/pixelate"));
	postProcessShaders.push_back(new Shader("assets/shaders/post/filmGrain"));
	postProcessShaders.push_back(new Shader("assets/shaders/post/water"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/scanline"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/median"));
	//postProcessShaders.push_back(new Shader("assets/shaders/post/sobelfilter"));
	postProcessShaders.push_back(new Shader("assets/shaders/post/postGrey"));

	models.push_back(new ObjModel("assets/models/ship/shipA_OBJ.obj"));
	distances.push_back(50);
	models.push_back(new ObjModel("assets/models/car/honda_jazz.obj"));
	distances.push_back(150);
	models.push_back(new ObjModel("assets/models/normalstuff/normaltest.obj"));
	distances.push_back(2);
	models.push_back(new ObjModel("assets/models/normalstuff/normaltest2.obj"));
	distances.push_back(2);

	if (glDebugMessageCallback)
	{

		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}

	rotation = 0;
	lastTime = glutGet(GLUT_ELAPSED_TIME);

	fbo = new Fbo(4096, 4096);
}

void display()
{

	fbo->bind();
	glViewport(0,0, fbo->width, fbo->height);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), screenSize.x / (float)screenSize.y, 0.01f, 2000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, distances[activeModel]), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -1));
	model = glm::rotate(model, rotation, glm::vec3(0, 0.25, 0));
	
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

	roomShader->use();
	roomShader->setUniform("modelMatrix", glm::scale(glm::mat4(1), distances[activeModel] * glm::vec3(1,1,1)));
	roomShader->setUniform("viewMatrix", view);
	roomShader->setUniform("projectionMatrix", projection);
	roomShader->setUniform("s_texture", 0);

	room->draw();

	Shader* shader = shaders[currentShader];
	shader->use();
	shader->setUniform("modelMatrix", model);
	shader->setUniform("viewMatrix", view);
	shader->setUniform("projectionMatrix", projection);

	glUniformMatrix3fv(shader->getUniform("normalMatrix"), 1, 0, glm::value_ptr(normalMatrix));								//en zet de matrix in opengl
	glUniform1f(shader->getUniform("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	glUniform1i(shader->getUniform("s_texture"), 0);

	models[activeModel]->draw();

	fbo->unbind();
	glViewport(0,0,screenSize.x, screenSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(-1, -1));
	verts.push_back(glm::vec2(1, -1));
	verts.push_back(glm::vec2(1, 1));
	verts.push_back(glm::vec2(-1, 1));

	postProcessShaders[currentPostShader]->use();
	glUniform1f(postProcessShaders[currentPostShader]->getUniform("time"), glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
	postProcessShaders[currentPostShader]->setUniform("s_texture", 0);
	postProcessShaders[currentPostShader]->setUniform("width", screenSize.x);
	postProcessShaders[currentPostShader]->setUniform("height", screenSize.y);

	fbo->use();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
	glDrawArrays(GL_QUADS, 0, verts.size());

	glutSwapBuffers();
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

	if (key == '[')
	{
	
		currentShader = (currentShader + shaders.size() - 1) % shaders.size();
		std::cout << "Shader " << currentShader << std::endl;
	}
	
	if (key == ']')
	{
	
		currentShader = (currentShader + 1) % shaders.size();
		std::cout << "Shader " << currentShader << std::endl;
	}

	if (key == '-')
	{
		currentPostShader = (currentPostShader + postProcessShaders.size() - 1) % postProcessShaders.size();
		std::cout << "Post shader " << currentPostShader << std::endl;
	}

	if (key == '=')
	{
		currentPostShader = (currentPostShader + 1) % postProcessShaders.size();
		std::cout << "Post shader " << currentPostShader << std::endl;
	}

	if (key == ',' || key == '.')
		activeModel = (activeModel + 1) % models.size();
}

void update()
{

	int time = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = time - lastTime;
	
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