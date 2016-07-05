#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>
#include <Poco/BasicEvent.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include <vector>
#include <string>


using std::vector;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Logger;
using Poco::BasicEvent;
using Poco::SharedPtr;
using Poco::NotificationQueue;

using namespace std;

class WebcamService;
class VideoStreamingServer;
class VideoStreamingRequestHandler : public HTTPRequestHandler
{
public:
	VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, VideoStreamingServer& server);
	~VideoStreamingRequestHandler();
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);
private:
	SharedPtr<WebcamService> webcamService;
	string boundary;
	VideoStreamingServer& server;
};
