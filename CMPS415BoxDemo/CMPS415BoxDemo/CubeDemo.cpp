// CMPS 415 : Cubes example. Draws two instances of a buffered rectangular cuboid.
// Also demonstrates GMTL matrix and passing to shaders.
// You must have associated shader programs in the correct directory.
// Not for redistribution beyond the class.

// Uses LoadShaders and vgl.h from Red Book code archive
// Some parts are based on Example 3.7 of OpenGL red book 8th Edition.

//#define USE_PRIMITIVE_RESTART

//#include <stdio.h>
//#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>

#include "LoadShaders.h"

#pragma comment (lib, "glew32.lib")

using namespace std;

// Vertex list (corners of a cuboid):
static const GLfloat g_vertex_buffer_data[] = {
	-0.20f,  0.20f,  0.20f, // front upper left
	 0.20f,  0.20f,  0.20f, // front upper right
	-0.20f, -0.20f,  0.20f, // front lower left
	 0.20f, -0.20f,  0.20f, // front lower right
	 0.00f,  0.40f, -0.20f, // rear  upper left
	 0.40f,  0.40f, -0.20f, // rear  upper right	
	 0.00f,  0.00f, -0.20f, // rear  lower left
	 0.40f,  0.00f, -0.20f, // rear  lower right
};

// RGB color triples for every coordinate above.
static const GLfloat g_color_buffer_data[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

// Index list. Using triangle strips as in OpenGL book example 3.7.
static const GLushort g_cube_indices_buffer_data[] =
{
	4, 6, 0, 2, 1, 3, 5, 7, // First strip
	0xFFFF, // restart primitive
	1, 0, 5, 4, 7, 6, 3, 2 // Second strip
};

gmtl::Matrix44f translateMatrix, Matrix;

// Vertex array object
GLuint VertexArrayID[2];

// Vertex buffer objects
GLuint vertexbuffer;
GLuint colorbuffer;
GLuint indexbuffer;

// Parameter location for passing a matrix to vertex shader
GLuint Matrix_loc;
// Parameter locations for passing data to shaders
GLuint vertposition_loc, vertcolor_loc;

GLenum errCode;
const GLubyte *errString;

void init(){
	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);

	// OpenGL background color
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "Cube_Vertex_Shader.vert" },
	{ GL_FRAGMENT_SHADER, "Cube_Fragment_Shader.frag" },
	{ GL_NONE, NULL } };

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	//Get the shader parameter locations for passing data to shaders
	vertposition_loc = glGetAttribLocation(program, "vertexPosition");
	vertcolor_loc = glGetAttribLocation(program, "vertexColor");
	Matrix_loc = glGetUniformLocation(program, "Matrix");

	// initialize two GMTL matrices ; one identity, one translation
	gmtl::identity(Matrix);

	translateMatrix.set(1.0f, 0.0f, 0.0f, -0.65f,
						0.0f, 1.0f, 0.0f, -0.65f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

//	translateMatrix.set(1.0f, 0.0f, 0.0f, 0.00f,
//						0.0f, 1.0f, 0.0f, 0.00f,
//						0.0f, 0.0f, 1.0f, 0.0f,
//						0.0f, 0.0f, 0.0f, 1.0f);

	/*** VERTEX ARRAY OBJECT SETUP***/
	// Create/Generate the Vertex Array Object
	glGenVertexArrays(1, &VertexArrayID[0]);
	// Bind the Vertex Array Object
	glBindVertexArray(VertexArrayID[0]);
	// Create/Generate the Vertex Buffer Object for the vertices.
	glGenBuffers(1, &vertexbuffer);
	// Bind the Vertex Buffer Object.
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Transfer data in to graphics system
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	// Specify data location and organization
	glVertexAttribPointer(vertposition_loc, // This number must match the layout in the shader
		3, // Size
		GL_FLOAT, // Type
		GL_FALSE, // Is normalized
		0, ((void*)0));
	// Enable the use of this array
	glEnableVertexAttribArray(vertposition_loc);

	// Similarly, set up the color buffer.
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(vertcolor_loc, 3, GL_FLOAT, GL_FALSE, 0, ((void*)0));
	glEnableVertexAttribArray(vertcolor_loc);

	// Set up the element (index) array buffer and copy in data
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	// Transfer data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(g_cube_indices_buffer_data),
		g_cube_indices_buffer_data, GL_STATIC_DRAW);
}

void display(){
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Send our transformation to the shader
	glUniformMatrix4fv(Matrix_loc, 1, GL_FALSE, &Matrix[0][0]);

	// When using multiple VAOs for multiple objects, buffers have to be switched.
	// In the current code, since we are rendering only one object, the buffer binding is defined in init().
	//
	// The main idea would be:
	// for object1
	// glBindVertexArray();
	// glUniformMatrix4fv(); - Send a transformation matrix of the object1 to the shader
	// glDrawElements();
	// for object2 -- switch the Vertex Array object
	// glBindVertexArray();
	// glUniformMatrix4fv(); - Send a transformation matrix of the object2 to the shader
	// glDrawElements();

	glBindVertexArray(VertexArrayID[0]);
	//Draw the cuboid
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);

	// Send a different transformation matrix to the shader
	glUniformMatrix4fv(Matrix_loc, 1, GL_FALSE, &translateMatrix[0][0]);

	// Draw the transformed cuboid
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
		
	//Ask GL to execute the commands from the buffer
	glFlush();	// *** if you are using GLUT_DOUBLE, use glutSwapBuffers() instead 
	
	//Check for any errors and print the error string.
	//NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		cout << "OpengGL Error: " << errString << endl;
	}
}

//User-defined keyboard handler
void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'q': case 'Q': case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	}
}

int main(int argc, char** argv){
	// For more details about the glut calls, 
	// refer to the OpenGL/freeglut Introduction handout.

	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(1024, 1024);

	// Specify OpenGL version and core profile.
	// We use 3.3 in this class, not supported by very old cards
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("415/515 CUBOID DEMO");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//Transfer the control to glut processing loop.
	glutMainLoop();

	return 0;
}
