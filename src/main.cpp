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

int main(int argc, char *argv[])
{
	Logger& logger = Logger::get("main");
	logger.information("Threads were used");

	int mode = 0;
	int nGrab = 4;
	bool bResize = false;


	if (argc > 1) {
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
	}

	SharedPtr<WebcamService> webcamService(new WebcamService(mode, bResize, nGrab));

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