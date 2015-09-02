#include <GL/glut.h>
#include <cmath>
using namespace std;

GLuint thelist;
int width,height,moveMax;
cv::Mat eLeft,eRight;


float angle = 0.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f, z=5.0f;
// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;


void show(string wName, cv::Mat &img, int ms){
	double mini, maxi, scale, shift;
	cv::minMaxLoc(img, &mini, &maxi);

	scale = 255.0/(maxi-mini);
	shift = -mini*scale;

	cv::Mat imgGray;
	cv::convertScaleAbs(img, imgGray, scale, shift);

	cv::imshow(wName, imgGray);
	cv::waitKey(ms);
}

void show3D(void) {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glTranslatef(width/-2, height/-2, moveMax);
	glCallList(thelist);
	glTranslatef(width/2, height/2, -1 * moveMax);
	glutSwapBuffers();
}

// mit den Tasten 1 bis 6 kann man um die x,y oder z-Achse drehen
void tastendruck(unsigned char key, int x, int y){
	if(key == '1') {
		glTranslatef(0.0, 0.0, -5.0);
	} else if(key == '2') {
		glTranslatef(0.0, 0.0, 5.0);
	}else if(key == '3') {
		glRotatef(2, 0.0, 1.0, 0.0);
	} else if(key == '4') {
		glRotatef(-2, 0.0, 1.0, 0.0);
	} else if(key == '5') {
		glRotatef(2, 0.0, 0.0, 1.0);
	} else if(key == '6') {
		glRotatef(-2, 0.0, 0.0, 1.0);
	} else {
		return;
	}
	glutPostRedisplay();
}

void mouseRotate(int x, int y) {
	static int lastx = 0;
	static int lasty = 0;
	if(x-lastx > 0) glRotatef(-1, 0, 1, 0);
	if(x-lastx < 0) glRotatef( 1, 0, 1, 0);
	if(y-lasty > 0) glRotatef(-1, 1, 0, 0);
	if(y-lasty < 0) glRotatef( 1, 1, 0, 0);
	glutPostRedisplay();
	lastx = x; lasty = y;
}

void init3d(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	int biggest = max(width, height);
	glutInitWindowSize(biggest, biggest);
	glutCreateWindow("2 Augen 2D -> 3D");

	glOrtho(
		biggest/-2,biggest/2,
		biggest/-2, biggest/2,
		-4*biggest, 4*biggest
	);

	thelist = glGenLists(1);
		
	//GL_POINTS
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.2, 0.2, 0.2, 1);
	glEnable(GL_DEPTH_TEST);

	// register callbacks
	glutDisplayFunc(show3D);
	glutMotionFunc(mouseRotate);
	glutKeyboardFunc(tastendruck);
	
}
