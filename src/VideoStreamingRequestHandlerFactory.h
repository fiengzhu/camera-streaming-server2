#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/SharedPtr.h>
#include <Poco/NotificationQueue.h>

#include <string>

using std::string;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::SharedPtr;
using Poco::NotificationQueue;

class VideoStreamingServer;
class WebcamService;
class VideoStreamingRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	VideoStreamingRequestHandlerFactory(SharedPtr<WebcamService> webcamService, 
		VideoStreamingServer& server, const string& uri);
	~VideoStreamingRequestHandlerFactory();
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request);
private:
	SharedPtr<WebcamService> webcamService;
	VideoStreamingServer& server;
	string uri;
};
