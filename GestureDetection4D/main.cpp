/**
* 
* @author Marcel Vielsack, Johannes Wetzel, Johannes Steudle
*/

#include <boost/filesystem.hpp>

#include <iostream>
#include <stdio.h>
#include <string>

// local header
#include "PointDrawer.h"
#include "Callbacks.h"
#include "GlutRoutines.h"

#include "OniFileDataSet.h"
#include "Detection.h"



// xml to initialize OpenNI
#define SAMPLE_XML_PATH "../../Sample-Tracking.xml"
namespace fs = boost::filesystem;



int main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	EnumerationErrors errors;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	//training mode
	if (argc > 1) {
		if (!strcmp(argv[1], "-t")) {
			std::cout << "Starting Trainingmode...\n" << std::endl;
			doTraining();
			exit(0);
		}
		//detection mode 
		else if (argc > 1 && !strcmp(argv[1], "-d")) {

			//live stream
			std::cout << "Starting Detectionmode" << std::endl;
			g_gestureSVM.loadModel(SVM_MODEL_FILE);
			g_PreGestureSVM.loadModel(SVM_PRE_MODEL_FILE);

			gestureNames = Datasource().getGestureNames();

			// 
			if (argc > 2 && fs::exists(argv[2])) {
				std::cout << "...with file: " << argv[2] << std::endl;
				rc = openDeviceFile(argv[2]);
				CHECK_RC(rc, "OpenDeviceFile");
				std::cout << "File loaded." << std::endl;
			}

			rc = initializeNiteKomponents();
			CHECK_RC(rc, "initializeNiteKomponents");

			// Mainloop
			glInit(&argc, argv);
			glutMainLoop();

			exit(0);
		} else if (fs::exists(argv[1])) {
			rc = openDeviceFile(argv[1]);
			CHECK_RC(rc, "OpenDeviceFile");
			std::cout << "File loaded." << std::endl;
			g_HandsGenerator.Create(g_Context);
			g_GestureGenerator.Create(g_Context);
		} 
		else if(argc > 1 && !strcmp(argv[1], "-dt"))
		{
			std::cout << "Starting Detectionmode" << std::endl;
			g_gestureSVM.loadModel(SVM_MODEL_FILE);
			g_PreGestureSVM.loadModel(SVM_PRE_MODEL_FILE);
			queryWithTrainingData();
		}
		else {
			std::cout << "\n\n---- Something went wrong with the parameters..." << std::endl;
			exit(-1);
		}
	} else {
		std::cout << "\n\n---- Too less parameters, should be like:" << std::endl;
		std::cout << "---- GestureDetection4D -t\n" << std::endl;
		std::cout << "---- GestureDetection4D -d\n" << std::endl;
		exit(-1);
	}
	return 0;
}
