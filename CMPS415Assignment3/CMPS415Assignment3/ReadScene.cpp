/* CMPS 415/515, 2015, University of Louisiana at Lafayette
NOTE: No distribution beyond 415/515 classes.

This shows how to read records from our animation files.
"animdata.bin" must be placed in the proper directory.
This prints some partial record data to the console.

The Keyframe structure stores one scene configuration (record).

Finger joint angles appear in the following order in an array:
0  - thumb TM roll
1  - thumb MCP flexion
2  - thumb IP flexion
3  - thumb TM abduction
4  - index MCP flexion
5  - index 2nd knuckle flexion
6  - index MCP abduction
7  - middle MCP flexion
8  - middle 2nd knuckle flexion
9  - middle MCP abduction
10 - ring MCP flexion
11 - ring 2nd knuckle flexion
12 - ring MCP abduction
13 - pinky MCP flexion
14 - pinky 2nd knuckle flexion
15 - pinky MCP abduction
(third knuckle flexion is found from second knuckle value as in handout)

WARNING: This loader assumes a little-endian architecture with 32-bit floats and longs.
If you have problems and want to check the 32-bit requirement, you can print
the sizes of float and long. If they are not 4 bytes, replace with sized
types for your compiler, e.g, replace "unsigned long" with "unsigned __int32"
or uint32_t. Contact the grader if you need to convert to big endian form.
*/

#include <stdio.h>
#pragma warning (disable : 4996) // Windows ; consider instead replacing fopen with fopen_s

struct Keyframe
{
	unsigned long time; // Timestamp, milliseconds since first record. Assume nondecreasing order.
	float palm_p[3];    // palm position w.r.t. world (x, y, z)
	float palm_q[4];    // palm orientation w.r.t. world, quaternion (a, b, c, s) a.k.a. (x, y, z, w)
	float joint[16];    // finger joint angles (in radians). See list above.
	float ball_p[3];    // ball position
	float ball_q[4];    // ball orientation
};

int main(void)
{
	FILE *fp;
	struct Keyframe c;   // calling it "c" for "configuration"

	if (fp = fopen("animdata.bin", "rb")) {
		while (fread((void *)&c, sizeof(c), 1, fp) == 1) {
			printf("At time %ld ms, index flexion was %.2f rad, ball was at (%.2f,%.2f,%.2f)\n",
				c.time, c.joint[4], c.ball_p[0], c.ball_p[1], c.ball_p[2]);
		}
		fclose(fp);
	}
	else {
		printf("Could not open file.\n");
	}
	return 0;
}
