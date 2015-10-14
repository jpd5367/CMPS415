// CMPS 415 Assignment 2
// Author: Joseph DeHart
// CLID:   jpd5367
// Date:   10/6/2015
// Prof:   Christoph Borst

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <array>

#include <math.h>
#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>

#include "LoadShaders.h"

#pragma comment (lib, "glew32.lib")

using namespace std;

const float ROTATE_BY = 0.1745f; // Rotation Constant for "small" rotations
float elevation = 0.0f, azimuth = 0.0f; //elevation ans azimuth variables to rotate camera
int mouseX, mouseY; // used to track change in mouse position
float mouseTravel = 0.02f; // variable to adjust the sensitivity of camera rotations

// Vertex list (Vertices of a phalange):
GLfloat phalange_vertex_buffer_data[] = {
	0.00f,  0.025f,  0.025f,
	0.40f,  0.025f,  0.025f,
	0.00f, -0.025f,  0.025f,
	0.40f, -0.025f,  0.025f,
	0.00f,  0.025f, -0.025f,
	0.40f,  0.025f, -0.025f,
	0.00f, -0.025f, -0.025f,
	0.40f, -0.025f, -0.025f
};

// RGB color triples for every coordinate above.
GLfloat phalange_color_buffer_data[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

// Index list using triangle strips
GLushort phalange_indices_buffer_data[] =
{
	4, 6, 0, 2, 1, 3, 5, 7, // First strip
	0xFFFF, // restart primitive
	1, 0, 5, 4, 7, 6, 3, 2 // Second strip
};

// Vertex list (Vertices of a finger tip):
GLfloat tip_vertex_buffer_data[] = {
	0.00f,  0.025f ,  0.025f , 
	0.05f,  0.025f ,  0.025f ,
	0.06f,  0.0125f,  0.0125f,
	0.06f, -0.0125f,  0.0125f,
	0.00f, -0.025f ,  0.025f ,
	0.05f, -0.025f ,  0.025f ,
	0.00f,  0.025f , -0.025f ,
	0.05f,  0.025f , -0.025f ,
	0.06f,  0.0125f, -0.0125f,
	0.06f, -0.0125f, -0.0125f,
	0.00f, -0.025f , -0.025f ,
	0.05f, -0.025f , -0.025f
};

// RGB color triples for every coordinate above.
GLfloat tip_color_buffer_data[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

// Index list using triangle strips
GLushort tip_indices_buffer_data[] =
{
	0, 4, 1, 5, 2, 3, 8, 9, 7, 11, 6, 10,// First strip
	0xFFFF,// restart primitive
	9, 3, 11, 5, 10, 4, 6, 0, 7, 1, 8, 2// second strip
};

// Vertex list (Vertices of a palm):
GLfloat palm_vertex_buffer_data[] = {
	0.05f,  0.20f,  0.05f,
	0.40f,  0.20f,  0.05f,
	0.00f,  0.15f,  0.05f,
	0.05f,  0.15f,  0.05f,
	0.40f,  0.15f,  0.05f,
	0.00f, -0.15f,  0.05f,
	0.05f, -0.15f,  0.05f,
	0.40f, -0.15f,  0.05f,
	0.05f, -0.20f,  0.05f,
	0.40f, -0.20f,  0.05f,
	0.05f,  0.20f, -0.05f,
	0.40f,  0.20f, -0.05f,
	0.00f,  0.15f, -0.05f,
	0.00f, -0.15f, -0.05f,
	0.05f, -0.20f, -0.05f,
	0.40f, -0.20f, -0.05f
};

// RGB color triples for every coordinate above.
GLfloat palm_color_buffer_data[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

// Index list using triangle strips
GLushort palm_indices_buffer_data[] =
{
	1, 0, 4, 2, 7, 6, 9, 8, // First strip
	0xFFFF, // restart primitive
	0, 2, 3, 5, 6, 8, // Second strip
	0xFFFF, // restart primitive
	10, 11, 12, 15, 13, 14,
	0xFFFF,// restart primitive
	0, 10, 1, 11, 9, 15, 8, 14, 5, 13, 2, 12, 0, 10 // Third strip
};

// Vertex list (Vertices of a Axis Shape):
GLfloat coord_vertex_buffer_data[] = {
	// x axis vertices
	0.00f,  0.005f,  0.005f,
	0.50f,  0.005f,  0.005f,
	0.00f, -0.005f,  0.005f,
	0.50f, -0.005f,  0.005f,
	0.00f,  0.005f, -0.005f,
	0.50f,  0.005f, -0.005f,
	0.00f, -0.005f, -0.005f,
	0.50f, -0.005f, -0.005f,
	// y axis vertices
   -0.005f, 0.00f,  0.005f,
	0.005f, 0.00f,  0.005f,
   -0.005f, 0.50f,  0.005f,
	0.005f, 0.50f,  0.005f,
   -0.005f, 0.00f, -0.005f,
	0.005f, 0.00f, -0.005f,
   -0.005f, 0.50f, -0.005f,
	0.005f, 0.50f, -0.005f,
	// z axis vertices
   -0.005f, -0.005f,  0.50f,
	0.005f, -0.005f,  0.50f,
   -0.005f,  0.005f,  0.50f,
	0.005f,  0.005f,  0.50f,
   -0.005f, -0.005f,  0.00f,
	0.005f, -0.005f,  0.00f,
   -0.005f,  0.005f,  0.00f,
	0.005f,  0.005f,  0.00f
};

// RGB color triples for every coordinate above.
GLfloat coord_color_buffer_data[] = {
	// x axis colors
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	// y axis colors
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	// z axis colors
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

// Index list using triangle strips
GLushort coord_indices_buffer_data[] =
{
	// x indices
	4, 6, 0, 2, 1, 3, 5, 7, // First strip
	0xFFFF, // restart primitive
	1, 0, 5, 4, 7, 6, 3, 2, // Second strip
	// y indices
	0xFFFF,// restart primitive
	12, 14, 8, 10, 9, 11, 13, 15, // Third strip
	0xFFFF, // restart primitive
	9, 8, 13, 12, 15, 14, 11, 10, // Fourth strip
	// z indices
	0xFFFF,// restart primitive
	20, 22, 16, 18, 17, 19, 21, 23, // Fifth strip
	0xFFFF, // restart primitive
	17, 16, 21, 20, 23, 22, 19, 18 // Sixth strip
};

// precomputing size of data to be passed to multiuse functions
GLfloat  vertDataSize[4]  = { sizeof(phalange_vertex_buffer_data) , sizeof(tip_vertex_buffer_data) , sizeof(palm_vertex_buffer_data) , sizeof(coord_vertex_buffer_data) };
GLfloat  colorDataSize[4] = { sizeof(phalange_color_buffer_data)  , sizeof(tip_color_buffer_data)  , sizeof(palm_color_buffer_data)  , sizeof(coord_color_buffer_data)  };
GLushort indexDataSize[4] = { sizeof(phalange_indices_buffer_data), sizeof(tip_indices_buffer_data), sizeof(palm_indices_buffer_data), sizeof(coord_indices_buffer_data)};
int indexBufferSize[4] =    { 17, 25, 37, 53};

// precomputing data arrays to be passed to multiuse functions
GLfloat  *vertexBufferData[4] = {phalange_vertex_buffer_data , tip_vertex_buffer_data  , palm_vertex_buffer_data , coord_vertex_buffer_data};
GLfloat  *colorBufferData[4]  = {phalange_color_buffer_data  , tip_color_buffer_data   , palm_color_buffer_data  , coord_color_buffer_data};
GLushort *indexBufferData[4]  = {phalange_indices_buffer_data, tip_indices_buffer_data , palm_indices_buffer_data, coord_indices_buffer_data};

// Matrices to be used throughout the program
gmtl::Matrix44f id, // Identity matrix
				tempMatrix, // Matrix to to VM 
				phalangeMatrix, phalangeLocalAxisMatrix, // "M" Matrix for phalange model to store cumulative changes and cooresponing local axis shape
				tipMatrix     , tipLocalAxisMatrix,// "M" Matrix for tip model to store cumulative changes and cooresponing local axis shape
				palmMatrix    , palmLocalAxisMatrix,// "M" Matrix for palm model to store cumulative changes and cooresponing local axis shape
				C, // Transformation matrix for the camera
				V; // World to View Transformation Matrix


// Vertex array object
GLuint VertexArrayID[4];

// Vertex buffer objects
GLuint vertexbuffer [4];
GLuint colorbuffer  [4];
GLuint indexbuffer  [4];

// Parameter location for passing a matrix to vertex shader
GLuint Matrix_loc;

// Parameter locations for passing data to shaders
GLuint vertPosition_loc, vertColor_loc;

GLenum errCode;
const GLubyte *errString;

// Function Prototypes
void CameraInit();
void SetObjectBuffer(int objectID);
void DrawObject(int objectID, gmtl::Matrix44f &mat);
void TranslateLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void TranslateWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void ScaleLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void ScaleWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void RotateLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void RotateWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v);
void RotatePosYTipWorld();
void ScaleUpXTipXLocal();
void ScaleDownXTipXLocal();
void RotatePhalangePosXLocal();
void RotatePhalangeNegXLocal();
void RotatePhalangePosYLocal();
void RotatePhalangeNegYLocal();
void ScaleUpPhalangeLocal();
void ScaleDownPhalangeLocal();
void TranslatePosYPalmLocal();
void TranslateNegYPalmlocal();
void TranslatePosXPalmWorld();
void TranslateNegXPalmWorld();
void RotatePosXPalmLocal();
void RotatePosYPalmLocal();
void RotatePosZPalmLocal();
void RotatePosXPalmWorld();
void RotatePosYPalmWorld();
void RotatePosZPalmWorld();

void init(){
	id.set(1.0f, 0.0f, 0.0f, 0.0f, 
		   0.0f, 1.0f, 0.0f, 0.0f, 
		   0.0f, 0.0f, 1.0f, 0.0f, 
		   0.0f, 0.0f, 0.0f, 1.0f);
	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);
	CameraInit(); // Initialize camera matrix
	V = C * id; // Initialize View Matrix
	// OpenGL background color
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "Cube_Vertex_Shader.vert" },
	{ GL_FRAGMENT_SHADER, "Cube_Fragment_Shader.frag" },
	{ GL_NONE, NULL } };
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	//Get the shader parameter locations for passing data to shaders
	vertPosition_loc = glGetAttribLocation( program, "vertexPosition");
	vertColor_loc    = glGetAttribLocation( program, "vertexColor"   );
	Matrix_loc       = glGetUniformLocation(program, "Matrix"        );

	
	// Initialize Matrices
	gmtl::identity(tipMatrix);
	gmtl::identity(tipLocalAxisMatrix);
	gmtl::identity(palmMatrix);
	gmtl::identity(palmLocalAxisMatrix);
	phalangeMatrix.set(1.0f, 0.0f, 0.0f, -0.40f,
					   0.0f, 1.0f, 0.0f, -0.4f,
					   0.0f, 0.0f, 1.0f,  0.0f,
					   0.0f, 0.0f, 0.0f,  1.0f);
	phalangeLocalAxisMatrix.set(1.0f, 0.0f, 0.0f, -0.40f,
								0.0f, 1.0f, 0.0f, -0.40f,
								0.0f, 0.0f, 1.0f,  0.0f,
								0.0f, 0.0f, 0.0f,  1.0f);

	// will not recognize Matrix type Identifiers: commenting out
	//id.set(IDENTITY);
	//tempMatrix.set(AFFINE | NON_UNISCALE);
	//phalangeMatrix.set(AFFINE);
	//phalangeLocalAxisMatrix.set(AFFINE);
	//tipMatrix.set(AFFINE | NON_UNISCALE);
	//tipLocalAxisMatrix.set(AFFINE);
	//palmMatrix.set(AFFINE);
	//palmLocalAxisMatrix.set(AFFINE);
	//C.set(AFFINE);
	//V.set(AFFINE);

	/*** VERTEX ARRAY OBJECT SETUP***/
	// Create/Generate the Vertex Array Object
	glGenVertexArrays(4, &VertexArrayID[0]);
	
	// Create/Generate the Vertex Buffer Object for the vertices.
	glGenBuffers(4, &vertexbuffer[0]);
	glGenBuffers(4, &colorbuffer[0]);
	glGenBuffers(4, &indexbuffer[0]);

	//Bind and Buffer Data to Graphics system
	SetObjectBuffer(0);
	SetObjectBuffer(1);
	SetObjectBuffer(2);
	SetObjectBuffer(3);
}

void display(){
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Vector 3 to pass camera rotations to rotation function
	gmtl::Vec3f cameraAdjustments(-elevation, -azimuth, 0.00f);
	CameraInit(); // reset camera matrix 
	RotateLocal(C, cameraAdjustments);// rotate camera by -elevation and azimuth
	V = C * id; // View Matrix is the inverse of Camera Matrix

	//Draw all Objects to the scene
	DrawObject(0, phalangeMatrix);
	DrawObject(1, tipMatrix);
	DrawObject(2, palmMatrix);
	DrawObject(3, id);
	DrawObject(3, phalangeLocalAxisMatrix);
	DrawObject(3, tipLocalAxisMatrix);
	DrawObject(3, palmLocalAxisMatrix);
	
	//Check for any errors and print the error string.
	//NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		cout << "OpengGL Error: " << errString << endl;
	}
}

//keyboard handler, key presses call transformation functions
void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case 'q': case 'Q': case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	case 'a': case 'A':
		RotatePosYTipWorld();
		break;
	case 'b': case 'B':
		ScaleUpXTipXLocal();
		break;
	case 'c': case 'C':
		ScaleDownXTipXLocal();
		break;
	case 'd': case 'D':
		RotatePhalangePosXLocal();
		break;
	case 'e': case 'E':
		RotatePhalangeNegXLocal();
		break;
	case 'f': case 'F':
		RotatePhalangePosYLocal();
		break;
	case 'g': case 'G':
		RotatePhalangeNegYLocal();
		break;
	case 'h': case 'H':
		ScaleUpPhalangeLocal();
		break;
	case 'i': case 'I':
		ScaleDownPhalangeLocal();
		break;
	case 'j': case 'J':
		TranslatePosYPalmLocal();
		break;
	case 'k': case 'K':
		TranslateNegYPalmlocal();
		break;
	case 'l': case 'L':
		TranslatePosXPalmWorld();
		break;
	case 'm': case 'M':
		TranslateNegXPalmWorld();
		break;
	case 'n': case 'N':
		RotatePosXPalmLocal();
		break;
	case 'o': case 'O':
		RotatePosYPalmLocal();
		break;
	case 'p': case 'P':
		RotatePosZPalmLocal();
		break;
	case 'r': case 'R':
		RotatePosXPalmWorld();
		break;
	case 's': case 'S':
		RotatePosYPalmWorld();
		break;
	case 't': case 'T':
		RotatePosZPalmWorld();
		break;	
	}
	glutPostRedisplay(); // Display updated scene
}

// motion Function handler, when any mouse buttom is pressed down changes in x adjust Azimuth and changes in y adjust elevation
void mouse(int x, int y){
	azimuth   += (((x - mouseX) * mouseTravel));
	elevation += (((y - mouseY) * mouseTravel));
	mouseX = x; // used to check the change in x,y since last event
	mouseY = y;
	glutPostRedisplay();

}

int main(int argc, char** argv){
	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(1024, 1024);

	// Specify OpenGL version and core profile.
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("415 Assignment 2 jpd5367");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse);

	//Transfer the control to glut processing loop.
	glutMainLoop();

	return 0;
}
// Initialize camera position
void CameraInit(){
	C.set(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f);
}
// Binds and buffers data to the graphics system for an object
// Parameters: int representing array index of object
void SetObjectBuffer(int objectID){
	// Bind/Buffer Vertex Array
	glBindVertexArray(VertexArrayID[objectID]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[objectID]);
	glBufferData(GL_ARRAY_BUFFER, vertDataSize[objectID], vertexBufferData[objectID], GL_STATIC_DRAW);
	glVertexAttribPointer(vertPosition_loc, 3, GL_FLOAT, GL_FALSE, 0, ((void*)0));
	glEnableVertexAttribArray(vertPosition_loc);
	// Bind/Buffer Color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[objectID]);
	glBufferData(GL_ARRAY_BUFFER, colorDataSize[objectID], colorBufferData[objectID], GL_STATIC_DRAW);
	glVertexAttribPointer(vertColor_loc, 3, GL_FLOAT, GL_FALSE, 0, ((void*)0));
	glEnableVertexAttribArray(vertColor_loc);
	// Bind/Buffer index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer[objectID]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize[objectID], indexBufferData[objectID], GL_STATIC_DRAW);
}
// Draws updated Transformations
void DrawObject(int objectID, gmtl::Matrix44f &mat){
	// Bind Vertex Array
	glBindVertexArray(VertexArrayID[objectID]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[objectID]);
	// Bind Color Buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer[objectID]);
	// Bind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer[objectID]);
	// Send our transformation to the shader
	tempMatrix = V * mat;
	glUniformMatrix4fv(Matrix_loc, 1, GL_FALSE, &tempMatrix[0][0]);
	// Draw Object 
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, indexBufferSize[objectID], GL_UNSIGNED_SHORT, NULL);
	glutSwapBuffers();
}
// Translates Object in the local frame
// Parameters: 4x4 Matrix to be translated , Vector3 of float to specify the change in each axis
void TranslateLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];

	gmtl::Matrix44f temp;
	temp.set(1.00f, 0.00f, 0.00f,     x,
		     0.00f, 1.00f, 0.00f,     y,
		     0.00f, 0.00f, 1.00f,     z,
		     0.00f, 0.00f, 0.00f, 1.00f);
	mat = mat * temp;
}
// Translates Object in the world frame
// Parameters: 4x4 Matrix to be translated , Vector3 of float to specify the change in each axis
void TranslateWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];

	gmtl::Matrix44f temp;
	temp.set(1.00f, 0.00f, 0.00f,     x,
			 0.00f, 1.00f, 0.00f,     y,
			 0.00f, 0.00f, 1.00f,	  z,
			 0.00f, 0.00f, 0.00f, 1.00f);
	mat = temp * mat;
}
// Scales Object in the local frame
// Parameters: 4x4 Matrix to be scaled , Vector3 of float to specify the change in each axis
void ScaleLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];

	gmtl::Matrix44f temp;
	temp.set(    x, 0.00f, 0.00f, 0.00f,
		     0.00f,     y, 0.00f, 0.00f,
		     0.00f, 0.00f,     z, 0.00f,
		     0.00f, 0.00f, 0.00f, 1.00f);
	mat = mat * temp;
}
// Scales Object in the world frame
// Parameters: 4x4 Matrix to be scaled , Vector3 of float to specify the change in each axis
void ScaleWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];

	gmtl::Matrix44f temp;
	temp.set(    x, 0.00f, 0.00f, 0.00f,
		     0.00f,     y, 0.00f, 0.00f,
		     0.00f, 0.00f,     z, 0.00f,
		     0.00f, 0.00f, 0.00f, 1.00f);
	mat = temp * mat;
}
// Rotates Object in the local frame
// Parameters: 4x4 Matrix to be rotated , Vector3 of float to specify the change in each axis in radians
void RotateLocal(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];
	if (v[0] != 0.0f)
	{
		gmtl::Matrix44f tempX;
		tempX.set(1.00f,      0.00f,     0.00f, 0.00f,
			      0.00f,  cos(   x), sin(   x), 0.00f,
			      0.00f, -sin(   x), cos(   x), 0.00f,
			      0.00f,      0.00f,     0.00f, 1.00f);
		mat = mat * tempX;
	}
	if (v[1] != 0.0f)
	{
		gmtl::Matrix44f tempY;
		tempY.set(cos(   y), 0.00f, -sin(   y), 0.00f,
			          0.00f, 1.00f,      0.00f, 0.00f,
			      sin(   y), 0.00f,  cos(   y), 0.00f,
			          0.00f, 0.00f,      0.00f, 1.00f);
		mat = mat * tempY;
	}
	if (v[2] != 0.0f)
	{
		gmtl::Matrix44f tempZ;
		tempZ.set(cos(   z), sin(   z), 0.00f, 0.00f,
			     -sin(   z), cos(   z), 0.00f, 0.00f,
			          0.00f,     0.00f, 1.00f, 0.00f,
			          0.00f,     0.00f, 0.00f, 1.00f);
		mat = mat * tempZ;
	}
}
// Rotates Object in the world frame
// Parameters: 4x4 Matrix to be rotated , Vector3 of float to specify the change in each axis in radians
void RotateWorld(gmtl::Matrix44f &mat, gmtl::Vec3f v){
	float x, y, z;
	x = v.mData[0];
	y = v.mData[1];
	z = v.mData[2];
	if (v[0] != 0.0f)
	{
		gmtl::Matrix44f tempX;
		tempX.set(1.00f,      0.00f,     0.00f, 0.00f,
			      0.00f,  cos(   x), sin(   x), 0.00f,
			      0.00f, -sin(   x), cos(   x), 0.00f,
			      0.00f,      0.00f,     0.00f, 1.00f);
		mat = tempX * mat;
	}
	if (v[1] != 0.0f)
	{
		gmtl::Matrix44f tempY;
		tempY.set(cos(   y), 0.00f, -sin(   y), 0.00f,
			          0.00f, 1.00f,      0.00f, 0.00f,
			      sin(   y), 0.00f,  cos(   y), 0.00f,
			          0.00f, 0.00f,      0.00f, 1.00f);
		mat = tempY * mat;
	}
	if (v[2] != 0.0f)
	{
		gmtl::Matrix44f tempZ;
		tempZ.set(cos(   z), sin(   z), 0.00f, 0.00f,
			     -sin(   z), cos(   z), 0.00f, 0.00f,
			          0.00f,     0.00f, 1.00f, 0.00f,
			          0.00f,     0.00f, 0.00f, 1.00f);
		mat = tempZ * mat;
	}
}
// Rotates the "tip" model about the World Y axis in the positive direction by pressing the 'a' key on the keyboard
void RotatePosYTipWorld(){ // 'a'
	gmtl::Vec3f v{ 0.00f, ROTATE_BY, 0.0f };
	RotateWorld(tipMatrix, v);
	RotateWorld(tipLocalAxisMatrix, v);
}
// Scales up the "tip" model in the local x axis in the by pressing the 'b' key on the keyboard
void ScaleUpXTipXLocal(){ // 'b'
	gmtl::Vec3f v{ 1.1f, 1.00f, 1.0f };
	ScaleLocal(tipMatrix, v);
}
// Scales down the "tip" model in the local x axis in the by pressing the 'c' key on the keyboard
void ScaleDownXTipXLocal(){ // 'c'
	gmtl::Vec3f v{ 0.9f, 1.00f, 1.0f };
	ScaleLocal(tipMatrix, v);
}
// Rotates the "phalange" model about the local X axis in the positive direction by pressing the 'd' key on the keyboard
void RotatePhalangePosXLocal(){ // 'd'
	gmtl::Vec3f v{ ROTATE_BY, 0.00f, 0.0f };
	RotateLocal(phalangeMatrix, v);
}
// Rotates the "phalange" model about the local X axis in the negative direction by pressing the 'e' key on the keyboard
void RotatePhalangeNegXLocal(){ // 'e'
	gmtl::Vec3f v{ -ROTATE_BY, 0.00f, 0.0f };
	RotateLocal(phalangeMatrix, v);
}
// Rotates the "phalange" model about the local Y axis in the positive direction by pressing the 'f' key on the keyboard
void RotatePhalangePosYLocal(){ // 'f'
	gmtl::Vec3f v{ 0.00f, ROTATE_BY, 0.0f };
	RotateLocal(phalangeMatrix, v);
}
// Rotates the "phalange" model about the local Y axis in the negative direction by pressing the 'g' key on the keyboard
void RotatePhalangeNegYLocal(){ // 'g'
	gmtl::Vec3f v{ 0.00f, -ROTATE_BY, 0.0f };
	RotateLocal(phalangeMatrix, v);
}
// Scales up the "phalange" model in the local axis uniformally by pressing the 'h' key on the keyboard
void ScaleUpPhalangeLocal(){ // 'h'
	gmtl::Vec3f v{ 1.1f, 1.1f, 1.1f };
	ScaleLocal(phalangeMatrix, v);
}
// Scales down the "phalange" model in the local axis uniformally by pressing the 'i' key on the keyboard
void ScaleDownPhalangeLocal(){ // 'i'
	gmtl::Vec3f v{ 0.9f, 0.90f, 0.90f };
	ScaleLocal(phalangeMatrix, v);
}
// Translates the "palm" model along the local Y axis in the positive direction by pressing the 'j' key on the keyboard
void TranslatePosYPalmLocal(){ // 'j'
	gmtl::Vec3f v{ 0.0f, 0.05f, 0.0f };
	TranslateLocal(palmMatrix, v);
}
// Translates the "palm" model along the local Y axis in the negative direction by pressing the 'k' key on the keyboard
void TranslateNegYPalmlocal(){ // 'k'
	gmtl::Vec3f v{ 0.0f, -0.05f, 0.0f };
	TranslateLocal(palmMatrix, v);
}
// Translates the "palm" model along the world X axis in the positive direction by pressing the 'l' key on the keyboard
void TranslatePosXPalmWorld(){ // 'l'
	gmtl::Vec3f v{ 0.05f, 0.00f, 0.0f };
	TranslateWorld(palmMatrix, v);
	TranslateWorld(palmLocalAxisMatrix, v);
}
// Translates the "palm" model along the world X axis in the negative direction by pressing the 'm' key on the keyboard
void TranslateNegXPalmWorld(){ // 'm'
	gmtl::Vec3f v{ -0.05f, 0.00f, 0.0f };
	TranslateWorld(palmMatrix, v);
	TranslateWorld(palmLocalAxisMatrix, v);
}
// Rotates the "palm" model about the local X axis in the positive direction by pressing the 'n' key on the keyboard
void RotatePosXPalmLocal(){ // 'n'
	gmtl::Vec3f v{ ROTATE_BY, 0.00f, 0.0f };
	RotateLocal(palmMatrix, v);
}
// Rotates the "palm" model about the local Y axis in the positive direction by pressing the 'o' key on the keyboard
void RotatePosYPalmLocal(){ // 'o'
	gmtl::Vec3f v{ 0.00f, ROTATE_BY, 0.0f };
	RotateLocal(palmMatrix, v);
}
// Rotates the "palm" model about the local Z axis in the positive direction by pressing the 'p' key on the keyboard
void RotatePosZPalmLocal(){ // 'p'
	gmtl::Vec3f v{ 0.00f, 0.00f, ROTATE_BY };
	RotateLocal(palmMatrix, v);
}
// Rotates the "palm" model about the world X axis in the positive direction by pressing the 'r' key on the keyboard
void RotatePosXPalmWorld(){ // 'r'
	gmtl::Vec3f v{ ROTATE_BY, 0.00f, 0.0f };
	RotateWorld(palmMatrix, v);
	RotateWorld(palmLocalAxisMatrix, v);
}
// Rotates the "palm" model about the world Y axis in the positive direction by pressing the 's' key on the keyboard
void RotatePosYPalmWorld(){ // 's'
	gmtl::Vec3f v{ 0.00f, ROTATE_BY, 0.00f };
	RotateWorld(palmMatrix, v);
	RotateWorld(palmLocalAxisMatrix, v);
}
// Rotates the "palm" model about the world Z axis in the positive direction by pressing the 't' key on the keyboard
void RotatePosZPalmWorld(){ // 't'
	gmtl::Vec3f v{ 0.00f, 0.00f, ROTATE_BY };
	RotateWorld(palmMatrix, v);
	RotateWorld(palmLocalAxisMatrix, v);
}
