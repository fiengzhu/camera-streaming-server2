#include <Poco/SharedPtr.h>
#include <memory>

using Poco::SharedPtr;

class WebcamService;
class WebCamController
{
public:
	WebCamController(SharedPtr<WebcamService> webcamService);
	~WebCamController();
	void StartWebCam();
	void StopWebCam();
private:
	SharedPtr<WebcamService> webcamService;
};
