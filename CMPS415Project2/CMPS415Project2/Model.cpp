

#include "Model.h"

	Model::Model(){
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer);
		glGenBuffers(1, &indexBuffer);
	}

	void Model::SetVertexBufferData(GLfloat l[]){
		vertexBufferData = new GLfloat[l.size()];
		for (int i = 0; i < l.size(); i++)
		{
			vertexBufferData = l;
		}
	}

	void Model::SetColorBufferData(GLfloat* l){
		colorBufferData = l;
	}

	void Model::SetIndexBufferData(GLfloat* l){
		indexBufferData = l;
	}
	/*
	void Model::SetVertexBuffer(GLuint v){
		vertexBuffer = v;
	}

	void Model::SetColorBuffer(c){
		colorBuffer = c;
	}

	void Model::SetIndexBuffer(GLfloat i){
		indexBuffer = i;
	}
    */
	void Model::SetMatrix(gmtl::Matrix44f mat){
		M = mat;
	}

	GLuint Model::GetVertexBuffer(){
		return vertexBuffer;
	}

	GLuint Model::GetColorBuffer(){
		return colorBuffer;
	}

	GLuint Model::GetIndexBuffer(){
		return indexBuffer;
	}

	gmtl::Matrix44f Model::GetMatrix()
	{
		return M;
	}

