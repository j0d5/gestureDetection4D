#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "XnVNite.h"
#include <XnCppWrapper.h>
#include <XnTypes.h>
// local header
#include "Detection.h"
#include <list>

using namespace std;

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	printf("No Hand!\n");
	g_pointBuffer.flush();

	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}
void XN_CALLBACK_TYPE TouchingCallback(xn::HandTouchingFOVEdgeCapability& generator, XnUserID id, const XnPoint3D* pPosition, XnFloat fTime, XnDirection eDir, void* pCookie)
{
	g_pDrawer->SetTouchingFOVEdge(id);
}
/*
void XN_CALLBACK_TYPE MyGestureInProgress(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
printf("Gesture %s in progress\n", strGesture);
}
void XN_CALLBACK_TYPE MyGestureReady(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
printf("Gesture %s ready for next stage\n", strGesture);
}
*/

void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE HandCreate(HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie) {
	printf("Hand detected!\n");
	g_pointBuffer.flush();
	frequencyCounter = FEATURE_VECTOR_FREQUENCY;
}

void XN_CALLBACK_TYPE HandUpdate(HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie) {
	// printf("Position X: %.2f Y: %.2f Z: %.2f\n", pPosition->X, pPosition->Y, pPosition->Z);
	

	if(!g_IsTrainMode) {
		g_pointBuffer.push(*pPosition);
	} else {
		//if list is full: resize list to double the size
		if(g_pointList4Training.size() == g_pointList4Training.max_size()) {
			g_pointList4Training.resize(g_pointList4Training.max_size() * 2);
		}

		g_pointList4Training.push_back(*pPosition);
	}

	if (!g_IsTrainMode && g_pointBuffer.isFull() && !frequencyCounter--) {
		doQuery();

		frequencyCounter = FEATURE_VECTOR_FREQUENCY;
	}
}

void XN_CALLBACK_TYPE HandDestroy(HandsGenerator &generator, XnUserID user, XnFloat fTime, void *pCookie) {
	printf("Hand destroyed!\n");
	g_pointBuffer.flush();
	frequencyCounter = FEATURE_VECTOR_FREQUENCY;
}

XnStatus initializeNiteKomponents () {
	XnStatus rc;
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
	return XN_STATUS_OK;
}


#endif
