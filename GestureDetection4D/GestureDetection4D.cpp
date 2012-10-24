#include "stdafx.h"
#include <math.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include "KinectWrapper/KinectWrapper.h"

using namespace cv;

int posX, posY;

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------

void mouseHandler (int event, int x, int y, int flags, void *param){

	switch(event) {
		/* mouse move */
	case CV_EVENT_MOUSEMOVE:
		posX = x;
		posY = y;

		break;
	}
}

int main(int argc, char* argv[]){

	CvFont font;

	Mat *rgbImage = new Mat(480, 640, CV_8UC3, Scalar::all(0));
	Mat *depthImage = new Mat(480, 640, CV_8UC1, Scalar::all(0));
	Mat *comboImage = new Mat(480, 640, CV_8UC3, Scalar::all(0));
	Mat *rawDepth = new Mat(480, 640, CV_16UC1, Scalar::all(0));

	KinectWrapper *wrapper = KinectWrapper::getInstance();

	if(!wrapper->init("../../SamplesConfig.xml")) exit(0);

	cvInitFont(&font, CV_FONT_VECTOR0, 1.0, 1.0, 0, 1, CV_AA);

	namedWindow( "RGB" );
	namedWindow( "Depth" );
	namedWindow( "Combo" );

	cvSetMouseCallback( "Depth", mouseHandler, NULL );

	while (waitKey(25) != 27) {
		if( wrapper->update()) {

			wrapper->getRGB(rgbImage);
			wrapper->getDisplayDepth(depthImage);
			wrapper->getCombo(comboImage);
			wrapper->getRawDepth(rawDepth);
			imshow("RGB", *rgbImage);

			imshow("Combo", *comboImage);

			char textDepth[20];
			sprintf(textDepth, "(%d, %d): %d \n", posX, posY, rawDepth->at<short>(posY, posX));
			putText(*depthImage, textDepth, cvPoint(10, 30), FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255,0));
			imshow("Depth", *depthImage);
		}
	}
	exit(0);
	return 0;
}