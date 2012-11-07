/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Point Viewer Sample                                  *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>
#include <cmath>
#include "CyclicBuffer.cpp"
// Header for NITE
#include "XnVNite.h"
// local header
#include "PointDrawer.h"



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

#ifdef USE_GLUT
	#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
		#include <GLUT/glut.h>
	#else
#include <glut.h>
	#endif
#elif defined(USE_GLES)
	#include "opengles.h"
	#include "kbhit.h"
#endif
#include "signal_catch.h"

#ifdef USE_GLES
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;
#endif

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::HandsGenerator g_HandsGenerator;
xn::GestureGenerator g_GestureGenerator;
xn::ImageGenerator g_imageGenerator;
XnMapOutputMode QVGAMode = { 320, 240, 30 };
XnMapOutputMode VGAMode = { 640, 480, 30 };
CyclicBuffer* cyclicBuffer;
bool recording = false;
bool dump = false;
bool valid=true;
char* gesture=NULL;
int pointCounter=0;
XnPoint3D* start=NULL;
	// To count missed frames
	XnUInt64 nLastDepthTime = 0;
	XnUInt64 nLastImageTime = 0;
	XnUInt32 nMissedDepthFrames = 0;
	XnUInt32 nMissedImageFrames = 0;
	XnUInt32 nDepthFrames = 0;
	XnUInt32 nImageFrames = 0;
	
// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

SessionState g_SessionState = NOT_IN_SESSION;

void CleanupExit()
{
	g_ScriptNode.Release();
	g_DepthGenerator.Release();
	g_HandsGenerator.Release();
	g_GestureGenerator.Release();
	g_Context.Release();

	exit (1);
}

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
//	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	//printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
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
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}
void XN_CALLBACK_TYPE Hand_Update(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, 
								  XnFloat fTime, void* pCookie) 
{

	pointCounter++;
	/*if(pointCounter==9){
		valid=true;
		start = new XnPoint3D();
		start->X=pPosition->X;
		start->Y=pPosition->Y;
		start->Z=pPosition->Z;
	}
	if(pointCounter>10){
		double diff;
		diff=abs(start->X - pPosition->X);
	
		if(diff>30){
			valid=false;
		}

		diff=abs(start->Y - pPosition->Y);
	
		if(diff>30){
			valid=false;
		}
		printf("valid %d\n",valid);
	}
	*/
	
	//printf("Punkte: %i \n",pointCounter);
}

void XN_CALLBACK_TYPE Hand_Create(xn::HandsGenerator& generator, XnUserID nId, const XnPoint3D* pPosition, 
								  XnFloat fTime, void* pCookie) 
{ 
	printf("XN_CALLBACK_TYPE Hand_Create \n");
	pointCounter=0;
	printf("pPosition = %f %f %f\n",pPosition->X,pPosition->Y,pPosition->Z);
	start = new XnPoint3D();
	start->X=pPosition->X;
	start->Y=pPosition->Y;
	start->Z=pPosition->Z;
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

// this function is called each frame
void glutDisplay (void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	XnMapOutputMode mode;
	g_DepthGenerator.GetMapOutputMode(mode);
#ifdef USE_GLUT
	glOrtho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);
#elif defined(USE_GLES)
	glOrthof(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);
#endif

	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
	
		if(dump)
		{
			cyclicBuffer->Dump();

			if(valid&&(gesture)){
				//Write oni file to db !!!
				cyclicBuffer->writeDumpToDB(start,gesture);
			}
			
			dump = false;
		}
		// Get next data
		g_Context.WaitAndUpdateAll();
		
		if(recording)
		{
			// Save data
			cyclicBuffer->Update(g_DepthGenerator,g_imageGenerator);	
		}
		// Update NITE tree
		g_pSessionManager->Update(&g_Context);
#ifdef USE_GLUT
		PrintSessionState(g_SessionState);
		if(gesture!=NULL)
		PrintGesture(gesture);
#endif
	}

#ifdef USE_GLUT
	glutSwapBuffers();
#endif
}

#ifdef USE_GLUT
void glutIdle (void)
{
	if (g_bQuit) {
		CleanupExit();
	}

	// Display the frame
	glutPostRedisplay();
}

void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// Exit
		CleanupExit();
	case'1':
		if(!recording)
		gesture="LetterL";
		break;
	case'2':
		if(!recording)
		gesture="LetterO";
		break;
	case'3':
		if(!recording)
		gesture="Push";
		break;
	case'4':
		if(!recording)
		gesture="Wave";
		break;
	case'5':
		if(!recording)
		gesture="5";
		break;
	case'6':
		if(!recording)
		gesture="6";
		break;
	case'7':
		if(!recording)
		gesture="7";
		break;
	case'8':
		if(!recording)
		gesture="8";
		break;
	case'9':
		if(!recording)
		gesture="9";
		break;
	case'0':
		if(!recording)
		gesture="10";
		break;
	// Key "o" for Swipe Gesture
	case'o':
		if(!recording)
		gesture="11";
		break;
	//Key "p" for Push Gesture
	case'p':
		if(!recording)
		gesture="12";
		break;
	case 'd':
		// Toggle drawing of the depth map
		g_bDrawDepthMap = !g_bDrawDepthMap;
		g_pDrawer->SetDepthMap(g_bDrawDepthMap);
		break;
	case 'f':
		g_bPrintFrameID = !g_bPrintFrameID;
		g_pDrawer->SetFrameID(g_bPrintFrameID);
		break;
	case 's':
		// Toggle smoothing
		if (g_fSmoothing == 0)
			g_fSmoothing = 0.1;
		else 
			g_fSmoothing = 0;
		g_HandsGenerator.SetSmoothing(g_fSmoothing);
		break;
	case 'e':
		// end current session
		g_pSessionManager->EndSession();
		break;
	case 'r':
		// record
		recording = true;
		break;
	case 'x':
		recording=false;
		dump=true;
		break;
	}
}

void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("Gesture Recorder");
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
#endif

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
XnStatus ConfigureGenerators(const RecConfiguration& config, xn::Context& context, xn::DepthGenerator& depthGenerator, xn::ImageGenerator& imageGenerator)
{
	XnStatus nRetVal = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Configure the depth, if needed
	if (config.bRecordDepth)
	{
		nRetVal = context.CreateAnyProductionTree(XN_NODE_TYPE_DEPTH, NULL, depthGenerator, &errors);
		CHECK_RC_ERR(nRetVal, "Create Depth", errors);
		nRetVal = depthGenerator.SetMapOutputMode(*config.pDepthMode);
		CHECK_RC(nRetVal, "Set Mode");
		if (config.bMirrorIndicated && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_MIRROR))
		{
			depthGenerator.GetMirrorCap().SetMirror(config.bMirror);
		}

		// Set Hole Filter
		depthGenerator.SetIntProperty("HoleFilter", TRUE);
	}
	// Configure the image, if needed
	if (config.bRecordImage)
	{
		nRetVal = context.CreateAnyProductionTree(XN_NODE_TYPE_IMAGE, NULL, imageGenerator, &errors);
		CHECK_RC_ERR(nRetVal, "Create Image", errors);
		nRetVal = imageGenerator.SetMapOutputMode(*config.pImageMode);
		CHECK_RC(nRetVal, "Set Mode");

		if (config.bMirrorIndicated && imageGenerator.IsCapabilitySupported(XN_CAPABILITY_MIRROR))
		{
			imageGenerator.GetMirrorCap().SetMirror(config.bMirror);
		}
	}

	// Configuration for when there are both streams
	if (config.bRecordDepth && config.bRecordImage)
	{
		// Registration
		if (config.bRegister && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
		{
			nRetVal = depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
			CHECK_RC(nRetVal, "Registration");
		}
		// Frame Sync
		if (config.bFrameSync && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_FRAME_SYNC))
		{
			if (depthGenerator.GetFrameSyncCap().CanFrameSyncWith(imageGenerator))
			{
				nRetVal = depthGenerator.GetFrameSyncCap().FrameSyncWith(imageGenerator);
				CHECK_RC(nRetVal, "Frame sync");
			}
		}
	}

	return XN_STATUS_OK;
}
// Parse the command line arguments
XnBool ParseArgs(int argc, char** argv, RecConfiguration& config)
{
	XnBool bError = FALSE;

	for (int i = 1; i < argc; ++i)
	{
		if (xnOSStrCaseCmp(argv[i], "time") == 0)
		{
			// Set the time of each recording
			if (argc > i+1)
			{
				config.nDumpTime = atoi(argv[i+1]);
				if (config.nDumpTime == 0)
					bError = TRUE;
				i++;
			}
			else
			{
				bError = TRUE;
			}
		}
		else if (xnOSStrCaseCmp(argv[i], "depth") == 0)
		{
			// Set depth resolution (default is QVGA)
			if (argc > i+1)
			{
				if (xnOSStrCaseCmp(argv[i+1], "vga") == 0)
				{
					config.pDepthMode = &VGAMode;
					++i;
				}
				else if (xnOSStrCaseCmp(argv[i+1], "qvga") ==0)
				{
					config.pDepthMode = &QVGAMode;
					++i;
				}
			}
			config.bRecordDepth = TRUE;
		}
		else if (xnOSStrCaseCmp(argv[i], "image") == 0)
		{
			// Set image resolution (default is QVGA)
			if (argc > i+1)
			{
				if (xnOSStrCaseCmp(argv[i+1], "vga") == 0)
				{
					config.pImageMode = &VGAMode;
					++i;
				}
				else if (xnOSStrCaseCmp(argv[i+1], "qvga") ==0)
				{
					config.pImageMode = &QVGAMode;
					++i;
				}
			}
			config.bRecordImage = TRUE;
		}
		else if (xnOSStrCaseCmp(argv[i], "verbose") == 0)
		{
			// Control the log
			config.bVerbose = TRUE;
		}
		else if (xnOSStrCaseCmp(argv[i], "mirror") == 0)
		{
			// Set mirror mode (does nothing if missing)
			config.bMirrorIndicated = TRUE;
			if (argc > i+1)
			{
				if (xnOSStrCaseCmp(argv[i+1], "on") == 0)
				{
					config.bMirror = TRUE;
					++i;
				}
				else if (xnOSStrCaseCmp(argv[i+1], "off") ==0)
				{
					config.bMirror = FALSE;
					++i;
				}
			}
		}
		else if (xnOSStrCaseCmp(argv[i], "registration") == 0)
		{
			// Set registration of depth to image
			config.bRegister = TRUE;
		}
		else if (xnOSStrCaseCmp(argv[i], "framesync") == 0)
		{
			// Sync the image and the depth
			config.bFrameSync = TRUE;
		}
		else if (xnOSStrCaseCmp(argv[i], "outdir") == 0)
		{
			// Set the directory in which the files will be created
			if (argc > i+1)
			{
				xnOSStrCopy(config.strDirName, argv[i+1], XN_FILE_MAX_PATH);
				++i;
			}
		}
		//Setting the specific gesture index
		/*else if (xnOSStrCaseCmp(argv[i], "gesture") == 0)
		{
			
			if (argc > i+1)
			{
				if (xnOSStrCaseCmp(argv[i+1], "null") == 0)
				{
					classIdx = 0;
					++i;
				}
				else if (xnOSStrCaseCmp(argv[i+1], "point") == 0)
				{
					classIdx = 1;
					++i;
				}
				else if (xnOSStrCaseCmp(argv[i+1], "wave") == 0)
				{
					classIdx = 2;
					++i;
				}
			}
		} into featureExtraction*/
		else
		{
			printf("Unknown option %s\n", argv[i]);
		}
	}

	// Must record something!
	if (!config.bRecordDepth && !config.bRecordImage)
	{
		printf("Recording nothing!\n");
		//printUsage(argv[0]);
		return FALSE;
	}
	// No default time
	if (config.nDumpTime == 0)
	{
		printf("Missing time\n");
		return FALSE;
	}

	return !bError;
}


// xml to initialize OpenNI
#define SAMPLE_XML_PATH "../Sample-Tracking.xml"

int main(int argc, char ** argv)
{
	RecConfiguration config;
	// To count missed frames
	XnUInt64 nLastDepthTime = 0;
	XnUInt64 nLastImageTime = 0;
	XnUInt32 nMissedDepthFrames = 0;
	XnUInt32 nMissedImageFrames = 0;
	XnUInt32 nDepthFrames = 0;
	XnUInt32 nImageFrames = 0;
	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Initialize OpenNI
	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");
	// Create and initialize the cyclic buffer
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
	CHECK_RC(rc, "Find hands generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
	CHECK_RC(rc, "Find gesture generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_imageGenerator);
	CHECK_RC(rc, "Find image generator");
	if (!ParseArgs(argc, argv, config))
	{
		printf("Parse error\n");
		return 1;
	}
	ConfigureGenerators(config, g_Context, g_DepthGenerator, g_imageGenerator);
	cyclicBuffer= new CyclicBuffer(g_Context, g_DepthGenerator, g_imageGenerator, config);
	cyclicBuffer->Initialize(config.strDirName, 4);
	XnCallbackHandle h;
	XnCallbackHandle h2;
	if (g_HandsGenerator.IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE))
	{
		g_HandsGenerator.GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(TouchingCallback, NULL, h);
	}
	g_HandsGenerator.RegisterHandCallbacks(Hand_Create,Hand_Update,0,0,h2);
	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);


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
#ifdef USE_GLUT
	glInit(&argc, argv);
	glutMainLoop();

#elif defined(USE_GLES)
	if (!opengles_init(GL_WIN_SIZE_X, GL_WIN_SIZE_Y, &display, &surface, &context))
	{
		printf("Error initializing opengles\n");
		CleanupExit();
	}
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	while ((!_kbhit()) && (!g_bQuit))
	{
		glutDisplay();
		eglSwapBuffers(display, surface);
	}
	opengles_shutdown(display, surface, context);

	CleanupExit();
#endif
}
