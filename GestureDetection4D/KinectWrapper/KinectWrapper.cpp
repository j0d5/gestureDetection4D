#include "stdafx.h"
#include "KinectWrapper.h"

KinectWrapper* KinectWrapper::_instance;

KinectWrapper::KinectWrapper() {
	started = false;
}

KinectWrapper* KinectWrapper::getInstance() {
	if(!_instance) {
		_instance = new KinectWrapper();
	}
	return _instance;
}

KinectWrapper::~KinectWrapper() {
	started = false;
	_instance = 0;
}

bool KinectWrapper::init(string CalibFilePath) {
	this->CalibFilePath = CalibFilePath;

	XnStatus rc;

	EnumerationErrors errors;
	rc = g_context.InitFromXmlFile(CalibFilePath.c_str(), g_scriptNode, &errors);

	if (rc == XN_STATUS_NO_NODE_PRESENT) {
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		return started;
	} else if (rc != XN_STATUS_OK) {
		printf("Open failed: %s\n", xnGetStatusString(rc));
		return started;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_depth);
	if (rc != XN_STATUS_OK)	{
		printf("No depth node exists! Check your XML.");
		return started;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
	if (rc != XN_STATUS_OK)	{
		printf("No image node exists! Check your XML.");
		return started;
	}

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	// Hybrid mode isn't supported in this sample
	if (g_imageMD.FullXRes() != g_depthMD.FullXRes() || g_imageMD.FullYRes() != g_depthMD.FullYRes()) {
		printf ("The device depth and image resolution must be equal!\n");
		return started;
	}

	// RGB is the only image format supported.
	if (g_imageMD.PixelFormat() != XN_PIXEL_FORMAT_RGB24) {
		printf("The device image format must be RGB24\n");
		return started;
	}

	g_depth.GetAlternativeViewPointCap().SetViewPoint(g_image);

	_rgbImage = new Mat(480, 640, CV_8UC3, Scalar::all(0));
	_comboImage = new Mat(480, 640, CV_8UC3, Scalar::all(0));
	_depthImage = new Mat(480, 640, CV_8UC1, Scalar::all(0));
	_rawDepth = new Mat(480, 640, CV_16UC1, Scalar::all(0));

	started = true;
	return started;
}

bool KinectWrapper::update() {

	if (!started) return false;

	XnStatus rc = XN_STATUS_OK;

	const XnDepthPixel* pDepth;

	// Read a new frame
	rc = g_context.WaitAnyUpdateAll();
	if (rc != XN_STATUS_OK)	{
		printf("Read failed: %s\n", xnGetStatusString(rc));
		return  false;
	}

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	pDepth = g_depthMD.Data();

	const XnRGB24Pixel* pImageRow = g_imageMD.RGB24Data();
	const XnDepthPixel* pDepthRow = g_depthMD.Data();

	for (XnUInt y = 0; y < g_imageMD.YRes(); ++y) {

		const XnRGB24Pixel* pImage = pImageRow;
		const XnDepthPixel* pDepth = pDepthRow;

		for (XnUInt x = 0; x < g_imageMD.XRes(); ++x, ++pImage,++pDepth) {

			_rgbImage->at<Vec3b>(y,x) = Vec3b(pImage->nBlue, pImage->nGreen , pImage->nRed);

			if (*pDepth != 0)

				_rawDepth->at<short>(y,x) = *pDepth;
			else

				_rawDepth->at<short>(y,x) = 0 ;
		}

		pDepthRow += g_depthMD.XRes();
		pImageRow += g_imageMD.XRes();
	}

	double min, max;

	Mat aux(480,640,CV_8UC3);

	minMaxLoc(*_rawDepth, &min, &max,NULL,NULL);

	_rawDepth->convertTo(*_depthImage, CV_8UC1, 255.0/max);

	cvtColor(*_depthImage,aux,CV_GRAY2BGR);

	_rgbImage->copyTo(*_comboImage);
	aux.copyTo(*_comboImage, *_depthImage);

	return true;
}

void KinectWrapper::getRGB(Mat *rgb) {
	_rgbImage->copyTo(*rgb);
}

void KinectWrapper::getRawDepth(Mat *rawDepth) {
	_rawDepth->copyTo(*rawDepth);
}

void KinectWrapper::getCombo(Mat *combo) {
	_comboImage->copyTo(*combo);
}

void KinectWrapper::getDisplayDepth(Mat *displayDepth) {
	_depthImage->copyTo(*displayDepth);
}



