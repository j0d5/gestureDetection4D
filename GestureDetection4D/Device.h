#ifndef DEVICE_H_
#define DEVICE_H_


// Headers for OpenNI
#include <XnCppWrapper.h>
// Header for NITE
#include "XnVNite.h"


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



// OpenNI objects
Context g_Context;
ScriptNode g_ScriptNode;
DepthGenerator g_DepthGenerator;
HandsGenerator g_HandsGenerator;
GestureGenerator g_GestureGenerator;

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

/// opens an onifile
XnStatus openDeviceFile(const char* csFile)
{
	XnStatus rc;
	rc = g_Context.Init();
	CHECK_RC(rc, "Context.Init()");
	rc = g_Context.OpenFileRecording(csFile);
	CHECK_RC(rc, "OpenOpenFileRecording");
	Player g_Player;
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_PLAYER, g_Player);
	CHECK_RC(rc, "Find Playernode");
	g_Player.SetRepeat(false);

	return XN_STATUS_OK;
}

#endif
