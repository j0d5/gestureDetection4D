#ifndef DEVICE_H_
#define DEVICE_H_

#include <XnCppWrapper.h>
#include <XnTypes.h>
#include "CyclicBuffer.h"

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
		XnChar strError[1024];				\
		errors.ToString(strError, 1024);	\
		printf("%s\n", strError);			\
		return (rc);						\
	}

using namespace xn;

// global cyclic buffer
CyclicBuffer<XnPoint3D> pointBuffer(200);

// OpenNI objects
Context g_Context;
ScriptNode g_ScriptNode;
DepthGenerator g_DepthGenerator;
HandsGenerator g_HandsGenerator;
GestureGenerator g_GestureGenerator;
Player g_Player;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 640
#define GL_WIN_SIZE_Y 480

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

SessionState g_SessionState = NOT_IN_SESSION;

XnStatus openDeviceFile(const char* csFile)
{
	XnStatus rc;
	rc = g_Context.Init();
	CHECK_RC(rc, "Context.Init()");
	rc = g_Context.OpenFileRecording(csFile);
	CHECK_RC(rc, "OpenOpenFileRecording");
	
	// g_Player = new Player(g_Context, "oni");
	// openCommon();

	return XN_STATUS_OK;
}

#endif