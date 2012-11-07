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
#include "Device.h"
#include "Callbacks.h"
#include "GlutRoutines.h"
#include "Datasource.h"
#include "OniFileDataSet.h"

// #include "signal_catch.h"

// xml to initialize OpenNI
#define SAMPLE_XML_PATH "../../Sample-Tracking.xml"
namespace fs = boost::filesystem;

XnStatus loadFiles (int argc, char ** argv) {
	XnStatus rc = XN_STATUS_OK;
	// iterates through the given directory, extracts the file and class and loads the files for training
	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_itr( argv[2] ); dir_itr != end_iter; ++dir_itr ) {
		try {
			if (fs::is_regular_file(dir_itr->status()) 
				&& !strcmp(dir_itr->path().extension().string().c_str(), ".oni"))
			{
				std::cout << "Loading file: " << dir_itr->path() << std::endl;
				std::string filename = dir_itr->path().filename().string();

				trainingClass = atoi(&filename.at(filename.length() - 5));
				std::cout << "Training class: " << trainingClass << std::endl;

				// opens the oni file
				rc = openDeviceFile(dir_itr->path().string().c_str());
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
			}
		} catch (const std::exception & ex)	{
			std::cout << dir_itr->path() << " " << ex.what() << std::endl;
		}
	}
}

XnStatus loadFileFromDB(char* file, int className) {
	XnStatus rc = XN_STATUS_OK;
	std::string filename = file;
	trainingClass = className;

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

int main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	EnumerationErrors errors;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	if (argc > 1) {
		if (argc > 1 && !strcmp(argv[1], "-t")) {
			std::cout << "Starting Trainingmode..." << std::endl;

			Datasource d;
			std::vector<OniFileDataSet*> oniFiles = d.getOniFileDatasets();

			std::vector<OniFileDataSet*>::iterator iter;

			for(iter = oniFiles.begin(); iter != oniFiles.end(); iter++)	{
				std::cout << "DB-Filename: " << (*iter)->getFilepath() << std::endl;
				std::cout << "GestureID: " << (*iter)->getGestureId() << std::endl;
				std::cout << "GestureName: " << (*iter)->getGestureName() << std::endl;

				loadFileFromDB((*iter)->getFilepath(), (*iter)->getGestureId());
			}			
			
			gestureSVM.generateModel(); // this has to be done after collecting feature vectors
			gestureSVM.saveModel(SVM_MODEL_FILE);
			exit(0);

		} else if (!strcmp(argv[1], "-d")) {
			std::cout << "Starting Detectionmode" << std::endl;
			gestureSVM.loadModel(SVM_MODEL_FILE);

			rc = initializeNiteKomponents();
			CHECK_RC(rc, "initializeNiteKomponents");

			// Mainloop
			glInit(&argc, argv);
			glutMainLoop();

			exit(0);
		} else if (fs::exists(argv[1])) {
			rc = openDeviceFile(argv[1]);
			CHECK_RC(rc, "OpenDeviceFile");
			printf("File loaded.\n");
			g_HandsGenerator.Create(g_Context);
			g_GestureGenerator.Create(g_Context);
		} else {
			std::cout << "\n\n---- Something went wrong with the parameters..." << std::endl;
			exit(-1);
		}
	} else {
		std::cout << "\n\n---- Too less parameters, should be like:" << std::endl;
		std::cout << "---- GestureDetection4D -t ..\\..\\OniFiles\n\n" << std::endl;
		exit(-1);
	}
	return 0;
}
