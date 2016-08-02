#include "WebCamController.h"
#include "VideoStreamingController.h"
#include "WebcamService.h"

#include <iostream>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/Observer.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <Poco/URI.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/OptionCallback.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/SharedPtr.h>
#include <Poco/Thread.h>

using namespace cv;
using namespace std;
using Poco::Logger;
using Poco::SharedPtr;
using Poco::AutoPtr;
using Poco::URI;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Timestamp;
using Poco::ThreadPool;
using Poco::Thread;
using Poco::Observer;
using Poco::Net::HTTPStreamFactory;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using Poco::Util::LayeredConfiguration;
using Poco::Util::ServerApplication;

void displayHelp() 
{
	cout << "Copyright: H3D Inc. 2016, Yuefeng Zhu" << endl;
	cout << "This is camera webserver app" << endl;
	cout << "OpenCV and Poco are required to run this app" << endl;
	cout << endl;

	cout << "USAGE:" << endl;
	cout << endl;

	cout << "caserver2 [-m] <1080p|5m> [-r] <0|2|4> [-g] <int> [-h] " << endl;
	cout << endl;

	cout << "Where:" << endl;
	cout << endl;

	cout << "-m,--mode" << endl;
	cout << "   Camera resolution, either 1080p or 5m" << endl;
	cout << endl;

	cout << "-r,--resize" << endl;
	cout << "   Shrink the image by none or 2 or 4 times" << endl;
	cout << endl;

	cout << "-g,--grab" << endl;
	cout << "   Number of grabbing the buffer each capture" << endl;
	cout << endl;

	cout << "-h,--help" << endl;
	cout << "   Display this information" << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{
	Logger& logger = Logger::get("main");
	logger.information("Threads were used");

	int mode = 0;
	int nGrab = 4;
	int nResize = 0;

	if (argc > 1)
	{
		for (int i=1; i<argc; i++)
		{
			char * ptr1;
			char * ptr2;

			ptr1 = strstr(argv[i], "--help");
			ptr2 = strstr(argv[i], "-h");
			if (ptr1 != NULL || ptr2 != NULL)
			{
				displayHelp();
				return 0;
			}
		}
	}


	if (argc > 1)
	{
		for (int i=1; i<argc; i+=2)
		{
			char * ptr1;
			char * ptr2;

			ptr1 = strstr(argv[i], "--mode");
			ptr2 = strstr(argv[i], "-m");
			if (ptr1 != NULL || ptr2 != NULL) 
			{
				if (argc > i+1)
				{
					if (strstr(argv[i+1], "5m") == NULL) { mode = 0;} else { mode = 1; } 
					cout << "change mode to " << mode << endl;
					continue;
				} 
				else
				{
					displayHelp();
					return 0;
				}
				
			}

			ptr1 = strstr(argv[i], "--resize");
			ptr2 = strstr(argv[i], "-r");
			if (ptr1 != NULL || ptr2 != NULL) 
			{
				if (argc > i+1)
				{
					nResize = atoi(argv[i+1]);
					cout << "change resize to " << nResize << endl;
					continue;
				}
				else
				{
					displayHelp();
					return 0;
				}
			}

			ptr1 = strstr(argv[i], "--grab");
			ptr2 = strstr(argv[i], "-g");
			if (ptr1 != NULL || ptr2 != NULL) 
			{
				if (argc > i+1)
				{
					nGrab = atoi(argv[i+1]);
					if (nGrab <= 0) nGrab = 1; 
					if (nGrab > 20) nGrab = 20;
					cout << "change grab to " << nGrab << endl;
					continue;
				}
				else
				{
					displayHelp();
					return 0;
				}
			}
		}

		cout << "mode = " << mode << " resize = " << nResize << " grab = " << nGrab << endl;
	}
	/*if (argc > 1) {
		int m = atoi(argv[1]);
		if (m>0) mode = 1;
	}

	if (argc > 2) {
		int m = atoi(argv[2]);
		if (m > 0) bResize = true;
	}

	if (argc > 3) {
		int m = atoi(argv[3]); if (m<=0) m = 1; if (m>=100) m = 100;
		nGrab = m;
	}*/

	SharedPtr<WebcamService> webcamService(new WebcamService(mode, nResize, nGrab));

	WebCamController webCamCtrl(webcamService);
	VideoStreamingController vidStreamCtrl(webcamService);
	

	webCamCtrl.StartWebCam();
	vidStreamCtrl.StartStreamingServer();

	std::cout << "test test" << std::endl;

	char key;
	while (1) {
		key = cvWaitKey(1000);
		//std::cout << key << std::endl;

		if (char(key) == 'q') {
			
			break; //If you hit ESC key loop will break.
		}
	}

	webCamCtrl.StopWebCam();
	vidStreamCtrl.StopStreamingServer();

	return 0;
}

