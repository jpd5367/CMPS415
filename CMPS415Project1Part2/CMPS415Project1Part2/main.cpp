// CMPS 415/ CSCE515, University of Louisiana at Lafayette
//
//
// NOTE: No permission is given for distribution beyond
//       the 415/515 class, of this file or any derivative works.
//

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

#define W_WIDTH  600
#define W_HEIGHT 600

bool HasGLSLSupport(void);
bool InitOpenGLExtensions(void);
bool HasOpenGL2Support(void);
bool HasGeometryShaderSupport(void);
bool HasShaderModel4(void);

bool useGLSL = false;
bool extensions_init = false;
bool bGeometryShader = false;
bool bGPUShader4 = false;

bool InitOpenGLExtensions(void){
	if (extensions_init)
		return true;

	extensions_init = true;

	GLenum err = glewInit();

	if (GLEW_OK != err)	{
		cout << "Error:" << glewGetErrorString(err) << endl;
		extensions_init = false;
		return false;
	}

	cout << "OpenGL Vendor: " << (char*)glGetString(GL_VENDOR) << "\n";
	cout << "OpenGL Renderer: " << (char*)glGetString(GL_RENDERER) << "\n";
	cout << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << "\n\n";

	HasGLSLSupport();

	//cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n\n";

	return true;
}

bool HasGLSLSupport(void){
	bGeometryShader = HasGeometryShaderSupport();
	bGPUShader4 = HasShaderModel4();

	if (useGLSL)
		return true;  // already initialized and GLSL is available

	useGLSL = true;

	if (!extensions_init)
		InitOpenGLExtensions();  // extensions were not yet initialized!!

	if (GLEW_VERSION_2_0){
		cout << "OpenGL 2.0 (or higher) is available!" << endl;
	}
	else if (GLEW_VERSION_1_5){
		cout << "OpenGL 1.5 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_4){
		cout << "OpenGL 1.4 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_3){
		cout << "OpenGL 1.3 core functions are available" << endl;
	}
	else if (GLEW_VERSION_1_2){
		cout << "OpenGL 1.2 core functions are available" << endl;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader")){
		cout << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader")){
		cout << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
		useGLSL = false;
	}

	if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects")){
		cout << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
		useGLSL = false;
	}

	if (useGLSL){
		cout << "[OK] OpenGL Shading Language version " << glGetString(GL_SHADING_LANGUAGE_VERSION) <<
			" is available" << endl;
	}
	else{
		cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
	}

	return useGLSL;
}

bool HasOpenGL2Support(void){
	if (!extensions_init)
		InitOpenGLExtensions();

	return (GLEW_VERSION_2_0 == GL_TRUE);
}

bool HasGeometryShaderSupport(void){
	if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
		return false;

	return true;
}

bool HasShaderModel4(void){
	if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
		return false;

	return true;
}

void display(void){
	glFlush();
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);

	glutCreateWindow("CMPS 415/515");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	GLenum err = glewInit();
	if (GLEW_OK != err)
		perror((char *)glewGetErrorString(err));
	if (InitOpenGLExtensions()){
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
			GLUT_ACTION_GLUTMAINLOOP_RETURNS);
		glutMainLoop();
	}
}