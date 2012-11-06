/**
* 
* @author Marcel Vielsack, Johannes Wetzel, Johannes Steudle
*/

#include <boost/filesystem.hpp>

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
namespace fs = boost::filesystem;

int main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	EnumerationErrors errors;

	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	if (argc > 1) {
		if (argc > 2 && !strcmp(argv[1], "-t") && fs::exists(argv[2])) {
			std::cout << "Starting Trainingsmode with folder: " << argv[2] << std::endl;

			fs::directory_iterator end_iter;
			for ( fs::directory_iterator dir_itr( argv[2] ); dir_itr != end_iter; ++dir_itr )
			{
				try
				{
					if ( fs::is_regular_file( dir_itr->status() ) )
					{
						std::cout << "Loading file: " << dir_itr->path().string() << "\n";
						rc = openDeviceFile(dir_itr->path().string().c_str());
						CHECK_RC(rc, "OpenDeviceFile");
						printf("File loaded.\n");
						g_HandsGenerator.Create(g_Context);
						g_GestureGenerator.Create(g_Context);
						initializeNiteKomponents();
					}
				}
				catch ( const std::exception & ex )
				{
					std::cout << dir_itr->path() << " " << ex.what() << std::endl;
				}
			}
			exit(0);
		} else if (!strcmp(argv[1], "-d")) {
			std::cout << "Starting Detectionmode" << std::endl;
			exit(0);
		} else if (fs::exists(argv[1])) {
			rc = openDeviceFile(argv[1]);
			CHECK_RC(rc, "OpenDeviceFile");
			printf("File loaded.\n");
			g_HandsGenerator.Create(g_Context);
			g_GestureGenerator.Create(g_Context);
		} else {
			std::cout << "Something went wrong with the parameters..." << std::endl;
			exit(-1);
		}
	}

	//
	initializeNiteKomponents();

	// Mainloop
	glInit(&argc, argv);
	glutMainLoop();

	getchar();
	return 0;
}
