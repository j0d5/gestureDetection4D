#ifndef KINECTWRAPPER_H_
#define KINECTWRAPPER_H_

#include <XnOS.h>
#include <XnCppWrapper.h>

#include <math.h>

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/highgui/highgui.hpp"

using namespace xn;
using namespace cv;
using namespace std;


#define DISPLAY_MODE_OVERLAY	1
#define DISPLAY_MODE_DEPTH		2
#define DISPLAY_MODE_IMAGE		3
#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_OVERLAY

class KinectWrapper {

public:
	bool init(string CalibFilePath);
	static KinectWrapper* getInstance();

    bool update();

    void getRGB(Mat *rgb);
    void getRawDepth(Mat *rawDepth);
    void getCombo(Mat *combo);
    void getDisplayDepth(Mat *displayDepth);

private:
	static KinectWrapper *_instance;

	KinectWrapper();
	virtual ~KinectWrapper();

	string CalibFilePath;
	bool started;

	Context g_context;

	ScriptNode g_scriptNode;
	DepthGenerator g_depth;
	ImageGenerator g_image;

	DepthMetaData g_depthMD;
	ImageMetaData g_imageMD;

	Mat *_rgbImage;
	Mat *_depthImage;
	Mat *_comboImage;
	Mat *_rawDepth;
};

#endif /* KinectWrapper_H_ */
