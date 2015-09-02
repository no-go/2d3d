#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "tools3d.cpp"
using namespace std;
using namespace cv;

int block = 5;
float xyDelta[1000][1000][50];

float fitt(
	int lX, int lY,
	int rX, int rY
) {
	float out=0;
	for(int i=0; i<block; i++) {
		for(int j=0; j<block; j++) {
			for(int c=0; c<3; c++) {
				out += abs(
					eLeft.at<Vec3b>(lY+i, lX+j)[c]
					- eRight.at<Vec3b>(rY+i, rX+j)[c]
				);
			}
		}
	}
	
	return out;
}

int mini(int x, int y) {
	int out = 1;
	for(int dx=0; dx < (2*moveMax); dx++) {
		if(xyDelta[x][y][out] > xyDelta[x][y][dx]) out = dx;
	}
	return out;
}

int main(int argc, char *argv[]) {
	if(!(argc == 5 || argc == 3)) {
		cout<<"usage: "<<argv[0]<<" von bis linkesBild rechtesBild"<<endl;
		cout<<"usage: "<<argv[0]<<" 3Dmap Texture"<<endl;
		return 1;
	}
	
	Mat mixed;
	
	if(argc == 5) {
		int von = atoi(argv[1]);
		int bis = atoi(argv[2]);
		moveMax = bis-von;
		
		Vec3b left,right;
		
		eLeft = imread(argv[3], CV_LOAD_IMAGE_COLOR);
		eRight = imread(argv[4], CV_LOAD_IMAGE_COLOR);
		width = eLeft.cols;
		height = eLeft.rows;
		
		for(int dx=0; dx < (2*moveMax); dx++) {
			mixed = Mat::zeros(height, width-2*moveMax, CV_32F);
			for(int y=0; y < height-block; y++) {
				for(int x=0; x < (width-2*moveMax)-block; x++) {
					xyDelta[x][y][dx] = fitt(x+dx, y, x+von, y);
					mixed.at<float>(y, x) = xyDelta[x][y][dx];
				}
			}
			show("Process", mixed, 10);
			cout<<".";
			cout.flush();
		}
		cout<<endl;
		
		mixed = Mat::zeros(height, width-2*moveMax, CV_8U);
		for(int y=0; y < height-block; y++) {
			for(int x=0; x < (width-2*moveMax)-block; x++) {
				mixed.at<uchar>(y, x) = mini(x, y);
			}
		}
		string name3dmap = string(argv[3]) + string("MAP.png");
		imwrite(name3dmap.c_str(), mixed);
		show("Process", mixed, 2000);
		destroyAllWindows();
	} else {
		mixed = imread(argv[1]);
		eLeft = imread(argv[2], CV_LOAD_IMAGE_COLOR);
		width = eLeft.cols;
		height = eLeft.rows;
		moveMax = (width - mixed.cols)/2;
	}
	
	float fact = 1;
	int fo=5;
	
	init3d(argc, argv);
	glNewList(thelist, GL_COMPILE);
	glPushMatrix();
	
	for(int y=height-block; y > fo; y-=1) {
		if(y%fo) continue;
		for(int x=fo; x < (width-2*moveMax)-block -fo; x+=1) {
			if(x%fo) continue;
			float z1 = mixed.at<uchar>(y, x);
			Vec3b colori1 = eLeft.at<Vec3b>(y, x);
			
			float z2 = mixed.at<uchar>(y, x+fo);
			Vec3b colori2 = eLeft.at<Vec3b>(y, x+fo);
			
			float z3 = mixed.at<uchar>(y+fo, x+fo);
			Vec3b colori3 = eLeft.at<Vec3b>(y+fo, x+fo);
			
			float z4 = mixed.at<uchar>(y+fo, x);
			Vec3b colori4 = eLeft.at<Vec3b>(y+fo, x);

			glBegin(GL_QUADS);
			
			glColor3ub(colori4[2], colori4[1], colori4[0]);
			glVertex3f(fact*x, fact*(height-y-fo), z4);
			
			glColor3ub(colori3[2], colori3[1], colori3[0]);
			glVertex3f(fact*(x+fo), fact*(height-y-fo), z3);

			glColor3ub(colori2[2], colori2[1], colori2[0]);
			glVertex3f(fact*(x+fo), fact*(height-y), z2);

			glColor3ub(colori1[2], colori1[1], colori1[0]);
			glVertex3f(fact*x, fact*(height-y), z1);

			glEnd();
			
		}
	}

	glPopMatrix();
	glEnd();
	glEndList();
	glutMainLoop();

	return 0;
}
