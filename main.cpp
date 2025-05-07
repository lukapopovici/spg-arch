#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PI glm::pi<float>()

glm::mat4 projectionMatrix, viewMatrix, modelMatrix;

std::string textFileRead(char *fn) {
	std::ifstream ifile(fn);
	std::string filetext;
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	return filetext;
}


float points[] = {
	0.0f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f
};

GLuint shader_programme, vao;
int height, width;
glm::mat4 projection, view, model, mvp;

float vertices[] = {
	 // positions         // colors           // texture coords
	 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 2.0f,
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f 
};

unsigned int texture;


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);

	// Model matrix :
	model = glm::rotate(glm::radians(10.0f), glm::vec3(0, 0, 1));
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = projection * view * model;

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint matrixID = glGetUniformLocation(shader_programme, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void init()
{
	glClearColor(1, 1, 1, 0);
	//glEnable(GL_DEPTH_TEST);
}


void reshape(int w, int h)
{
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f); // In world coordinates

	// Camera matrix
	view = glm::lookAt(
		glm::vec3(0, 0, 1), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(512, 512);
	glutCreateWindow("SPG");

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	GLint texture_units;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);

	printf("Max number of textures =  %d\n", texture_units);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	glewInit();

	init();

	//texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	// load and generate the texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("wallg.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//end texture

	GLuint vbo = 123;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	std::string vstext = textFileRead("vertex.vert");
	std::string fstext = textFileRead("fragment.frag");
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);


	GLint status = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//pot exista și warning-uri, nu doar erori de compilare
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		//preluăm și afișăm informațiile logate
		glGetShaderInfoLog(vs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);


	status = GL_FALSE;
	InfoLogLength;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//pot exista și warning-uri, nu doar erori de compilare
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		//preluăm și afișăm informațiile logate
		glGetShaderInfoLog(fs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}