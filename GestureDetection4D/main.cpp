/**
 * 
 * @author Marcel Vielsack, Johannes Wetzel, Johannes Steudle
*/

#include <iostream>
// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
// Header for NITE
#include "XnVNite.h"

// local header
#include "PointDrawer.h"
#include "Device.h"
#include "Callbacks.h"
#include "GlutRoutines.h"
#include "CyclicBuffer.h"
#include "../FeatureExtractor4D/IFeatureExtractor4D.h"

// #include "signal_catch.h"

// xml to initialize OpenNI
#define SAMPLE_XML_PATH "../../Sample-Tracking.xml"

int main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	EnumerationErrors errors;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	if (argc > 1) {
		if (!strcmp(argv[1], "-t")) {
			std::cout << "Starting Trainingsmode" << std::endl;
			exit(0);
		} else if (!strcmp(argv[1], "-d")) {
			std::cout << "Starting Detectionmode" << std::endl;
			exit(0);
		} else {
			rc = openDeviceFile(argv[1]);
			CHECK_RC(rc, "OpenDeviceFile");
			printf("File loaded.\n");
			g_HandsGenerator.Create(g_Context);
			g_GestureGenerator.Create(g_Context);
		}
	}

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
	CHECK_RC(rc, "Find hands generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
	CHECK_RC(rc, "Find gesture generator");

	XnCallbackHandle h;
	if (g_HandsGenerator.IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE))
	{
		g_HandsGenerator.GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(TouchingCallback, NULL, h);
	}

	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);

	// register handupdate callback for getting the point
	g_HandsGenerator.RegisterHandCallbacks(HandCreate, HandUpdate, HandDestroy, NULL, h);

	// Create NITE objects
	g_pSessionManager = new XnVSessionManager;
	rc = g_pSessionManager->Initialize(&g_Context, "RaiseHand", "RaiseHand");
	CHECK_RC(rc, "SessionManager::Initialize");
	g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	g_pDrawer = new XnVPointDrawer(20, g_DepthGenerator); 
	g_pFlowRouter = new XnVFlowRouter;
	g_pFlowRouter->SetActive(g_pDrawer);

	g_pSessionManager->AddListener(g_pFlowRouter);

	g_pDrawer->RegisterNoPoints(NULL, NoHands);
	g_pDrawer->SetDepthMap(g_bDrawDepthMap);

	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	// Mainloop
	glInit(&argc, argv);
	glutMainLoop();

	getchar();
	return 0;
}
