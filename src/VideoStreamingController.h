#include <Poco/SharedPtr.h>
#include <Poco/AutoPtr.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Activity.h>
#include <Poco/NotificationQueue.h>

using Poco::NotificationQueue;
using Poco::SharedPtr;
using Poco::AutoPtr;
using Poco::Net::SocketAddress;
using Poco::Activity;

class VideoStreamingServer;
class WebcamService;
class VideoStreamingController
{
public:
	VideoStreamingController(SharedPtr<WebcamService> webcamService);
	~VideoStreamingController();
	void StartStreamingServer();
	bool StopStreamingServer();
private:
	VideoStreamingServer* streamingServer;

	void HandleLostConnections(const void* pSender, const SocketAddress& arg);
};
