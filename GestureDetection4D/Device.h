#ifndef DEVICE_H_
#define DEVICE_H_


// Headers for OpenNI
#include "XnVNite.h"
#include <XnCppWrapper.h>
#include <XnTypes.h>

#include "Callbacks.h"

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
XnStatus initializeNiteKomponents();

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


XnStatus playFileSilent(char* file) {
	XnStatus rc = XN_STATUS_OK;
	std::string filename = file;

	// opens the oni file
	rc = openDeviceFile(file);
	CHECK_RC(rc, "OpenDeviceFile");
	std::cout << "File loaded..." << std::endl;
	std::cout << "Start learning..." << std::endl;
	g_HandsGenerator.Create(g_Context);
	g_GestureGenerator.Create(g_Context);
	initializeNiteKomponents();

	Player p;
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_PLAYER, p);
	CHECK_RC(rc, "Get Player");

	// play file and generate feature vectors, train svm
	while(!p.IsEOF()) {
		XnMapOutputMode mode;
		g_DepthGenerator.GetMapOutputMode(mode);

		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		// Update NITE tree
		g_pSessionManager->Update(&g_Context);
		PrintSessionState(g_SessionState);
	}
	return rc;
}



#endif
