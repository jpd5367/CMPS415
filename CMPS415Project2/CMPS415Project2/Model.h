#ifndef __MODEL_H_INCLUDED__
#define __MODEL_H_INCLUDED__

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>
#include <array>

#pragma comment (lib, "glew32.lib")

using namespace std;

class Model
{
public:
	gmtl::Matrix44f M;
	GLfloat* vertexBufferData;
	GLfloat* colorBufferData;
	GLfloat* indexBufferData;
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint indexBuffer;

	Model::Model();

	void SetVertexBufferData(GLfloat []);

	void SetColorBufferData(GLfloat*);

	void SetIndexBufferData(GLfloat*);

	void SetVertexBuffer(GLfloat);

	void SetColorBuffer(GLfloat);

	void SetIndexBuffer(GLfloat);

	void SetMatrix(gmtl::Matrix44f);

	GLuint GetVertexBuffer();

	GLuint GetColorBuffer();

	GLuint GetIndexBuffer();

	gmtl::Matrix44f GetMatrix();

};

#endif // __Model_H_INCLUDED__