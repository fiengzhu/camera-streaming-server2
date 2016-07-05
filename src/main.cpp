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

int main()
{
	Logger& logger = Logger::get("main");
	logger.information("Threads were used");

	SharedPtr<WebcamService> webcamService(new WebcamService());

	WebCamController webCamCtrl(webcamService);
	VideoStreamingController vidStreamCtrl(webcamService);
	

	webCamCtrl.StartWebCam();
	vidStreamCtrl.StartStreamingServer();

	std::cout << "test test" << std::endl;

	char key;
	while (1) {
		key = cvWaitKey(10);

		if (char(key) == 27) {
			break; //If you hit ESC key loop will break.
		}
	}

	webCamCtrl.StopWebCam();
	vidStreamCtrl.StopStreamingServer();

	return 0;
}