#include "VideoStreamingRequestHandlerFactory.h"
#include "VideoStreamingRequestHandler.h"
#include "VideoStreamingServer.h"
#include "WebcamService.h"
//#include "InvalidUriRequestHandler.h"

VideoStreamingRequestHandlerFactory::VideoStreamingRequestHandlerFactory(SharedPtr<WebcamService> webcamService, VideoStreamingServer& server, const string& uri) 
	: webcamService(webcamService), server(server), uri(uri) { 
}

VideoStreamingRequestHandlerFactory::~VideoStreamingRequestHandlerFactory() {
	//do not delete, since it is a shared pointer
	webcamService = nullptr; 
}

HTTPRequestHandler* VideoStreamingRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& request) {
	if (request.getURI() == uri) {
		return new VideoStreamingRequestHandler(webcamService, server);
	}
	else
	{
		return nullptr;
	}
	/*else {
		return new InvalidUriRequestHandler;
	}*/
}
