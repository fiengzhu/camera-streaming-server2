#include "VideoStreamingController.h"
#include "WebcamService.h"
#include "VideoStreamingServer.h"

#include <Poco/Delegate.h>

using Poco::Delegate;

VideoStreamingController::VideoStreamingController(SharedPtr<WebcamService> webcamService)
{
	streamingServer = new VideoStreamingServer(4711, "/videostream", webcamService);
	streamingServer->ClientLostConnection += Poco::delegate(this, &VideoStreamingController::HandleLostConnections);
}

VideoStreamingController::~VideoStreamingController() {
	streamingServer->ClientLostConnection -= Poco::delegate(this, &VideoStreamingController::HandleLostConnections);
	delete streamingServer;
	streamingServer = nullptr;
}

void VideoStreamingController::StartStreamingServer() {
	if (!streamingServer->IsRunning()) {
		streamingServer->StartServer();
	}
}

bool VideoStreamingController::StopStreamingServer() {
	if (streamingServer->IsRunning()) {
		streamingServer->StopServer();
	}

	return true;
}

void VideoStreamingController::HandleLostConnections(const void* pSender, const SocketAddress& arg) {
	//Message* message = new Message(MessageCommandEnum::connectionlost);
	//message->AddParam("ip", arg.toString());
}
