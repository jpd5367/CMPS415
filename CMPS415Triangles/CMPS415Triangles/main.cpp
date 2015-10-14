//Triangle Scan Conversion
//Author: Joseph DeHart
//CLID:   jpd5367
//professor: Dr. Christoph Borst
//course: CMPS415
//semester: Fall2015
// Project 1
//Submitted: September 17, 2015

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define WIDTH 800		// width of window (also frame buffer's width)
#define HEIGHT 600		// height of window (also frame buffer's height)
static GLubyte frame_buffer[HEIGHT][WIDTH][3];

struct click{
	int x;
	int y;
};

int clickCount = 0;
click clicks[3] = { 0, 0, 0 };

/*
frame_buffer simulates a frame buffer with 3 bytes per pixel (24 bit RGB).

The first index of "frame_buffer" refers to the y-coordinate (0 to HEIGHT-1).
The second index of "frame_buffer" refers to the x-coordinate (0 to WIDTH-1).
The third index of "frame_buffer" selects the R, G, or B byte.
Each element has value 0 to 255, with 0 being minimum intensity and 255 being max.

Example [setting the pixel at (100,10) to red]:
frame_buffer[10][100][0] = 255;
frame_buffer[10][100][1] = 0;
frame_buffer[10][100][2] = 0;

General Coding and Submission Requirements can be found on Moodle.

*/
// Calculates the slope of the edge of a triangle 
float LineSlope(int v1,int v2)
{
	return (((float)clicks[v2].y - (float)clicks[v1].y) / ((float)clicks[v2].x - (float)clicks[v1].x));
}

// Calculates the increment of a color along a given edge
float ColorBoundaryIncrement(int v1, int v2)
{
	if ((((float)clicks[v2].x - (float)clicks[v1].x)) != 0)
		return ((float)256.0 / ((float)clicks[v2].x - (float)clicks[v1].x));
	else
		return 0.0;
}

// main triangle scan conversion and color interpolation is processed here
void ProcessTriangle()
{
	// sort array
	click temp;
	temp.x = 0;
	temp.y = 0;

	for (int j = 0; j < 2; j++)// sort points based on x values
	{
		for (int i = 0; i < 2; i++)
		{
			if (clicks[i].x > clicks[i + 1].x)
			{
				temp.x = clicks[i].x;
				temp.y = clicks[i].y;
				clicks[i].x = clicks[i + 1].x;
				clicks[i].y = clicks[i + 1].y;
				clicks[i + 1].x = temp.x;
				clicks[i + 1].y = temp.y;
			}
		}
	}

	// floating point calculations outside of the loop
	float redToGreenIncrement = ColorBoundaryIncrement(0, 1);
	float redToBlueIncrement = ColorBoundaryIncrement(0, 2);
	float greenToBlueIncrement = ColorBoundaryIncrement(1, 2);

	float v0V1Slope = LineSlope(0, 1);
	float v0V2Slope = LineSlope(0, 2);
	float v1V2Slope = LineSlope(1, 2);

	bool isUpwardTriangle = false;
	if (v1V2Slope < 0.0) isUpwardTriangle = true;

	//set first pixel red if no a vertical edge
	if (clicks[0].x != clicks[1].x)
	{
		frame_buffer[clicks[0].y][clicks[0].x][0] = 255;
		frame_buffer[clicks[0].y][clicks[0].x][1] = 0;
		frame_buffer[clicks[0].y][clicks[0].x][2] = 0;
	}

	int bottomBoundaryX = clicks[0].x;
	float bottomBoundaryY = (float)clicks[0].y;
	int bottomPixelY = clicks[0].y;
	float topBoundaryY = (float)clicks[0].y;
	int topPixelY = clicks[0].y;
	float bottomRedValue = 255;
	float topRedValue = 255;
	float bottomGreenValue = 0;
	float topGreenValue = 0;
	float bottomBlueValue = 0;
	float topBlueValue = 0;

	//loop to iteratively set pixel colors
	//first loop V0x < x <= V1x
	int V0xV1xLength = clicks[1].x - clicks[0].x;
	for (int i = 0; i < V0xV1xLength; i++)// for each column
	{
		if (isUpwardTriangle)
		{
			//initialize upward triangle increments
			bottomRedValue -= redToBlueIncrement;
			topRedValue -= redToGreenIncrement;
			bottomGreenValue = 0;
			topGreenValue += redToGreenIncrement;
			bottomBlueValue += redToBlueIncrement;
			topBlueValue = 0;
			bottomBoundaryY += v0V2Slope;
			topBoundaryY += v0V1Slope;
		}
		else // if downward pointing triangle
		{
			//initialize downward triangle increments
			bottomRedValue -= redToGreenIncrement ;
			topRedValue -= redToBlueIncrement;
			bottomGreenValue += redToGreenIncrement;
			topGreenValue = 0;
			bottomBlueValue = 0;
			topBlueValue += redToBlueIncrement;
			bottomBoundaryY += v0V1Slope;
			topBoundaryY += v0V2Slope;
		}

		bottomBoundaryX += 1;
		bottomPixelY = (int)ceil(bottomBoundaryY);
		topPixelY = (int)floor(topBoundaryY);

		float redColumnIncrement = 0;
		float greenColumnIncrement = 0;
		float blueColumnIncrement = 0;
		if ((topPixelY - bottomPixelY) != 0)
		{
			redColumnIncrement = (topRedValue - bottomRedValue) / (topPixelY - bottomPixelY);
			greenColumnIncrement = (topGreenValue - bottomGreenValue) / (topPixelY - bottomPixelY);
			blueColumnIncrement = (topBlueValue - bottomBlueValue) / (topPixelY - bottomPixelY);
		}

		float redCurrentValue = bottomRedValue + (bottomPixelY - bottomBoundaryY) * redColumnIncrement;
		float greenCurrentValue = bottomGreenValue + (bottomPixelY - bottomBoundaryY) * greenColumnIncrement;
		float blueCurrentValue = bottomBlueValue + (bottomPixelY - bottomBoundaryY) * blueColumnIncrement;

		// inner for loop for filling interior of triangle
		int columnHeight = topPixelY - bottomPixelY;
		for (int j = 0; j < columnHeight; j++)
		{
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][0] = (GLubyte)ceil(redCurrentValue - .5);
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][1] = (GLubyte)ceil(greenCurrentValue - .5);
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][2] = (GLubyte)ceil(blueCurrentValue - .5);
			redCurrentValue += redColumnIncrement;
			greenCurrentValue += greenColumnIncrement;
			blueCurrentValue += blueColumnIncrement;
		}

	}

	// second loop V1x < x <= V2x
	int V1xV2xLength = clicks[2].x - clicks[1].x;
	for (int i = 0; i < V1xV2xLength; i++)// for each column
	{
		if (isUpwardTriangle)
		{
			//increment upward triangle increments
			bottomRedValue -= redToBlueIncrement;
			topRedValue = 0;
			bottomGreenValue = 0;
			topGreenValue -= greenToBlueIncrement;
			bottomBlueValue += redToBlueIncrement;
			topBlueValue += greenToBlueIncrement;
			bottomBoundaryY += v0V2Slope;
			topBoundaryY += v1V2Slope;
		}
		else // if downward pointing triangle
		{
			//initialize downward triangle increments
			bottomRedValue = 0;
			topRedValue -= redToBlueIncrement;
			bottomGreenValue -= greenToBlueIncrement;
			topGreenValue = 0;
			bottomBlueValue += greenToBlueIncrement;
			topBlueValue += redToBlueIncrement;
			bottomBoundaryY += v1V2Slope;
			topBoundaryY += v0V2Slope;
		}
	
		bottomBoundaryX += 1;
		bottomPixelY = (int)ceil(bottomBoundaryY);
		topPixelY = (int)floor(topBoundaryY);

		float redColumnIncrement = 0;
		float greenColumnIncrement = 0;
		float blueColumnIncrement = 0;
		if ((topPixelY - bottomPixelY) != 0)
		{
			redColumnIncrement = (topRedValue - bottomRedValue) / (topPixelY - bottomPixelY);
			greenColumnIncrement = (topGreenValue - bottomGreenValue) / (topPixelY - bottomPixelY);
			blueColumnIncrement = (topBlueValue - bottomBlueValue) / (topPixelY - bottomPixelY);
		}

		float redCurrentValue = bottomRedValue;
		float greenCurrentValue = bottomGreenValue;
		float blueCurrentValue = bottomBlueValue;

		// inner for loop for filling interior of triangle
		int columnHeight = topPixelY - bottomPixelY;
		for (int j = 0; j < columnHeight; j++)
		{
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][0] = (GLubyte)ceil(redCurrentValue - .5);
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][1] = (GLubyte)ceil(greenCurrentValue - .5);
			frame_buffer[(bottomPixelY + j)][bottomBoundaryX][2] = (GLubyte)ceil(blueCurrentValue - .5);
			redCurrentValue += redColumnIncrement;
			greenCurrentValue += greenColumnIncrement;
			blueCurrentValue += blueColumnIncrement;
		}

	}
	clickCount = 0;//reset clickCount

	for (int i = 0; i < 2; i++)//reinitialize clicks
	{
		clicks[i].x = 0;
		clicks[i].y = 0;
	}
}


void processClick(int x, int y)
{
	clicks[clickCount].x = x;//store pos in click index
	clicks[clickCount].y = HEIGHT - y;//store pos in click index

	if (clickCount == 2)// if 3rd click has been gathered proceed to process the triangle
	{
		ProcessTriangle();
	}
	else clickCount++;
}

void mousebuttonhandler(int button, int state, int x, int y)
{
	// when left mouse button is pressed down:
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		processClick(x, y);
	}
	// cause a display event to occur for GLUT:
	glutPostRedisplay();
}


/* Called when a display event occurs in GLUT: */
void display(void) {
	/*
	Set the raster position to the lower-left corner to avoid a problem
	(with glDrawPixels) when the window is resized to smaller dimensions.
	*/
	glRasterPos2i(-1, -1);

	// Write the information stored in "frame_buffer" to the color buffer
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer);
	glFlush();
}


int main(int argc, char **argv) {
	// GLUT initialization:
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frame Buffer Example");

	glutDisplayFunc(display);

	// Start GLUT event processing loop:
	glutMouseFunc(mousebuttonhandler);
	glutMainLoop();

	return 1;
}