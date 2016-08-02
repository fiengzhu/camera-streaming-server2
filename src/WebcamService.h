#include "Observable.h"
#include "ov_video_capture.h"

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <Poco/Thread.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Logger.h>
#include <Poco/RWLock.h>
#include <Poco/Mutex.h>

#include <memory>
#include <vector>

using cv::VideoCapture;
using cv::Mat;
using std::vector;
using Poco::Thread;
using Poco::RunnableAdapter;
using Poco::Logger;
using Poco::Mutex;

class WebcamService : public Observable < WebcamService > {
public:
	WebcamService(int mode, int nResize = 0, int nGrab = 4);
	~WebcamService();

	bool StartRecording();
	bool StopRecording();
	Mat& GetLastImage();
	vector<uchar>* GetModifiedImage();
	void SetModifiedImage(Mat& image);
	bool IsRecording();
	int GetFPS();
	int GetDelay();
private:
	bool isRecording;
	bool isModifiedAvailable;
	int fps;
	int delay;
	int m_nResize;

	jafp::OvVideoCapture * capture;

	Mat lastImage;
	vector<uchar> modifiedImage;
	Thread* recordingThread;
	RunnableAdapter<WebcamService>* recordingAdapter;
	Poco::Mutex lastImgMutex;
	Poco::Mutex modifiedImgMutex;
	vector<int> params;

	void RecordingCore();
};
