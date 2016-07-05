#include "WebcamService.h"
#include <iostream>
#include <iomanip>

#include <Poco/Clock.h>
#include <Poco/Stopwatch.h>

using Poco::Stopwatch;
using Poco::Clock;
using std::cout;

#ifndef USE_OV_CAPTURE
WebcamService::WebcamService() : capture(VideoCapture()) {
#else
WebcamService::WebcamService() : capture(jafp::OvVideoCapture::OV_MODE_1920_1080_15) {
#endif

	recordingThread = new Thread("WebCamRecording");
	recordingAdapter = new RunnableAdapter<WebcamService>(*this, &WebcamService::RecordingCore);
	isRecording = false;
	fps = 15;
	delay = 1000 / fps; //in ms
}

WebcamService::~WebcamService() {
	if (recordingThread->isRunning()) {
		StopRecording();
	}

	if (capture.isOpened()) {
		capture.release();
	}
}

int WebcamService::GetDelay() {
	return delay;
}

int WebcamService::GetFPS() {
	return fps;
}

void WebcamService::SetModifiedImage(Mat& image) {
	//Stopwatch sw;
	//sw.start();

	Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
	// encode mat to jpg and copy it to content
	cv::imencode(".bmp", image, modifiedImage);	
	
	//sw.stop();
	//printf("modified image: %f  ms/n", sw.elapsed() * 0.001);
}

vector<uchar>* WebcamService::GetModifiedImage() {
	Poco::Mutex::ScopedLock lock(modifiedImgMutex); //will be released after leaving scop
	vector<uchar> *tempImg = new vector<uchar>(modifiedImage.begin(), modifiedImage.end());
	return tempImg;
}

Mat& WebcamService::GetLastImage() {
	Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
	return lastImage;
}

bool WebcamService::StartRecording() {
	Logger& logger = Logger::get("WebcamService");

#ifndef USE_OV_CAPTURE
	capture.open(0);
#else
	capture.open();
#endif

	if (!capture.isOpened()){
		logger.error("No camera available!");
		std::cout << "Open Camera Error" << std::endl;
		return false;
	}

	std::cout << "Open Camera successfully" << std::endl;

	logger.information("starting recording...");

	//camera settings
	//capture.set(CV_CAP_PROP_FPS, fps);
	//Possible resolutions : 1280x720, 640x480; 440x330
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	//logger.information("Camera settings: ");
	//logger.information("FPS: " + std::to_string(capture.get(CV_CAP_PROP_FPS)));
	//logger.information("Resolution: " + std::to_string(capture.get(CV_CAP_PROP_FRAME_WIDTH)) + "x" + std::to_string(capture.get(CV_CAP_PROP_FRAME_HEIGHT)));
	//logger.information("Codec: " + std::to_string(capture.get(CV_CAP_PROP_FOURCC)));
	//logger.information("Format: " + std::to_string(capture.get(CV_CAP_PROP_FORMAT)));

	isRecording = true;
	recordingThread->start(*recordingAdapter);

	logger.information("started recording");

	return true;
}

bool WebcamService::StopRecording() {
	Logger& logger = Logger::get("WebcamService");

	logger.information("stopping recording...");

	if (recordingThread->isRunning()) {
		isRecording = false;
		logger.information("recording activity stop requested");
		recordingThread->join();
		logger.information("recording activity stopped successfully");
	}
	else {
		logger.error("recording activity is already stopped!");
	}

	logger.information("stopped recording");

	return true;
}

bool WebcamService::IsRecording() {
	return capture.isOpened() && recordingThread->isRunning();
}

void WebcamService::RecordingCore() {
	Logger& logger = Logger::get("WebcamService");
	Mat frame;

	//Stopwatch sw;
	Clock clock;
	int newDelay = 0;

	while (isRecording) {
		if (!capture.isOpened()) {
			logger.error("Lost connection to webcam!");
			std::cout << "Recording Error in the middle" << std::endl;
			break;
		}

		//sw.restart();

		//Create image frames from capture
		capture >> frame;

		cv::imwrite("testweb.jpg", frame);
		//std::cout << "Captured Frame" << std::endl;

		clock.update();
		if (!frame.empty()) 
		{
			
			Poco::Mutex::ScopedLock lock(lastImgMutex); //will be released after leaving scop
			lastImage = frame; //Clone image
			SetModifiedImage(lastImage);
				//logger.information("new image");
			//std::cout << "Captured good Frame" << std::endl;

			Notify();
		}
		else {
			logger.warning("Captured empty webcam frame!");

			std::cout << "Captured empty Frame" << std::endl;
		}

		newDelay = delay - clock.elapsed() * 0.001;

		if (newDelay > 0) {
			//webcam can only be queried after some time again
			//according to the FPS rate
			Thread::sleep(newDelay);
		}
	}

	isRecording = false;
}
