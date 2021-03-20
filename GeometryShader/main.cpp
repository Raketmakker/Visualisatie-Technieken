#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/color_space.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#pragma comment(lib, "glew32.lib")

glm::ivec2 screenSize;
GLuint programId;
GLuint modelViewUniform;
GLuint timeUniform;
GLuint vertexShader;
GLuint fragmentShader;
float rotation;
int lastTime;

class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 color;
	Vertex(const glm::vec3 &position, const glm::vec3 &color) : position(position), color(color) {}
};


void checkShaderErrors(GLuint shaderId)
{
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);					//kijk of het compileren is gelukt
	if (status == GL_FALSE)
	{
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);				//haal de lengte van de foutmelding op
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);		//en haal de foutmelding zelf op
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}
}

void printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		if (infoLog[0] != '\0')
		{
			printf("%s\n", infoLog);
			//	getchar();
		}
		free(infoLog);
	}
}


#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}


std::vector<Vertex> vertices;


float random()
{
	return (float)rand() / RAND_MAX;
}

void init()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glClearColor(1, 0.7f, 0.3f, 1.0f);
	std::string name = "defaultGeometry";

	std::ifstream geomShaderFile(name+".gs");
	std::string geomShaderData((std::istreambuf_iterator<char>(geomShaderFile)), std::istreambuf_iterator<char>());
	const char* cgeomShaderData = geomShaderData.c_str();

	std::ifstream vertexShaderFile(name+".vs");
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	const char* cvertexShaderData = vertexShaderData.c_str();

	std::ifstream fragShaderFile(name+".fs");
	std::string fragShaderData((std::istreambuf_iterator<char>(fragShaderFile)), std::istreambuf_iterator<char>());
	const char* cfragShaderData = fragShaderData.c_str();

	programId = glCreateProgram();							// maak een shaderprogramma aan

	GLuint geomId = glCreateShader(GL_GEOMETRY_SHADER);		// maak vertex shader aan
	glShaderSource(geomId, 1, &cgeomShaderData, NULL);		// laat opengl de shader uit de variabele 'vertexShader' halen
	glCompileShader(geomId);								// compileer de shader
	checkShaderErrors(geomId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, geomId);					// hang de shader aan het shaderprogramma

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);		// maak vertex shader aan
	glShaderSource(vertexId, 1, &cvertexShaderData, NULL);		// laat opengl de shader uit de variabele 'vertexShader' halen
	glCompileShader(vertexId);								// compileer de shader
	checkShaderErrors(vertexId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, vertexId);					// hang de shader aan het shaderprogramma


	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);	// maak fragment shader aan
	glShaderSource(fragmentId, 1, &cfragShaderData, NULL);	// laat opengl de shader uit de variabele 'fragmentShader' halen
	glCompileShader(fragmentId);							// compileer de shader
	checkShaderErrors(fragmentId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, fragmentId);					// hang de shader aan het shaderprogramma

	glLinkProgram(programId);								// link het programma, zorg dat alle attributes en varying gelinked zijn
	glUseProgram(programId);								// Zet dit als actieve programma

	printProgramInfoLog(programId);

	modelViewUniform = glGetUniformLocation(programId, "modelViewProjectionMatrix");	//haal de uniform van modelViewMatrix op
	timeUniform = glGetUniformLocation(programId, "time");	//haal de uniform van modelViewMatrix op

	glEnableVertexAttribArray(0);							// we gebruiken vertex attribute 0
	glEnableVertexAttribArray(1);							// we gebruiken vertex attribute 1

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}

	/*for (int i = 0; i < 9999; i++)
	{
		vertices.push_back(Vertex(glm::vec3(random() * 10 - 5, random() * 10 - 5, random() * 10 - 5), 
			glm::rgbColor(glm::vec3(random() * 360, random() * .5 + .5, 1))));
	}*/
	vertices.push_back(Vertex(glm::vec3(0.0, 0.0, 0.0),
		glm::rgbColor(glm::vec3(random() * 360, random() * .5 + .5, 1))));
	vertices.push_back(Vertex(glm::vec3(0.0, 1.0, 1.0),
		glm::rgbColor(glm::vec3(random() * 360, random() * .5 + .5, 1))));
	vertices.push_back(Vertex(glm::vec3(1.0, 1.0, 0.0),
		glm::rgbColor(glm::vec3(random() * 360, random() * .5 + .5, 1))));


	rotation = 0;
	lastTime = glutGet(GLUT_ELAPSED_TIME);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 mvp = glm::perspective(70.0f, screenSize.x / (float)screenSize.y, 0.9f, 20.0f);		//begin met een perspective matrix
	mvp *= glm::lookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));					//vermenigvuldig met een lookat
	mvp = glm::translate(mvp, glm::vec3(0, 0, -1));													//of verplaats de camera gewoon naar achter
	mvp = glm::rotate(mvp, rotation*.25f, glm::vec3(0, 1, 0));											//roteer het object een beetje
	//mvp = glm::rotate(mvp, 0.0f, glm::vec3(0, 1, 0));											//roteer het object een beetje
	glUniformMatrix4fv(modelViewUniform, 1, 0, glm::value_ptr(mvp));								//en zet de matrix in opengl
	glUniform1f(timeUniform, lastTime / 1000.0f);


	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), vertices.data());									//geef aan dat de posities op deze locatie zitten
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), &vertices[0].color);									//geef aan dat de posities op deze locatie zitten
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());																			//en tekenen :)

	glutSwapBuffers();
}

void reshape(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
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