#include "VideoStreamingRequestHandler.h"
#include "WebcamService.h"
#include "VideoStreamingServer.h"

#include <Poco/Net/MultipartWriter.h>
#include <Poco/Net/MessageHeader.h>

using Poco::Net::MessageHeader;
using Poco::Net::HTTPResponse;
using Poco::Net::MultipartWriter;

VideoStreamingRequestHandler::VideoStreamingRequestHandler(SharedPtr<WebcamService> webcamService, VideoStreamingServer& server)
	: webcamService(webcamService), server(server) {
	boundary = "VIDEOSTREAM";
}

VideoStreamingRequestHandler::~VideoStreamingRequestHandler() {
	//do not delete, since it is a shared pointer
	webcamService = nullptr;
}

void VideoStreamingRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
	Poco::Logger& logger = Poco::Logger::get("VideoStreamingRequestHandler");
	logger.information("Video stream request by client: " + request.clientAddress().toString());

	std::cout << "Video stream request by client: " << request.clientAddress().toString() << std::endl;

	// check if webcam service is running correctly
	if (!webcamService->IsRecording()) {
		logger.warning("No stream available. Closing connection to " + request.clientAddress().toString());
		response.setStatus(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
		response.send();

		std::cout << "Send Error to Client " << request.clientAddress().toString() << std::endl;
		return;
	}

	logger.information("Video streaming started for client " + request.clientAddress().toString());

	response.set("Max-Age", "0");
	response.set("Expires", "0");
	response.set("Cache-Control", "no-cache, private");
	response.set("Pragma", "no-cache");
	response.setContentType("multipart/x-mixed-replace; boundary=--" + boundary);
	response.setChunkedTransferEncoding(false);

	std::ostream& out = response.send();
	int frames = 0;
	//double start = 0.0;
	//double dif = 0.0;
	std::cout << "Prepare to Send frames to Client " << request.clientAddress().toString() << "out is" << out.good() << std::endl;

	while (out.good() && webcamService->IsRecording()) {
		//start = CLOCK();

		MultipartWriter writer(out, boundary);

		//std::cout << "status good" << std::endl;

		vector<uchar>* buf = webcamService->GetModifiedImage(); //take ownership

		if (buf->size() == 0) {
			logger.warning("Read empty stream image");
			//std::cout << "Zero Image" << std::endl;
			delete buf;
			continue;
		}

		MessageHeader header = MessageHeader();
		header.set("Content-Length", std::to_string(buf->size()));
		header.set("Content-Type", "image/jpeg");
		writer.nextPart(header);
		out.write(reinterpret_cast<const char*>(buf->data()), buf->size());
		out << "/r/n/r/n";

		//std::cout << "Sending frames to Client " << request.clientAddress().toString() << std::endl;

		delete buf;
		buf = nullptr;

		//dif = CLOCK() - start;
		//printf("Sending: %.2f ms; avg: %.2f ms/r", dif, avgdur(dif));
		++frames;
	}

	printf("Frames: %u", frames);

	logger.information("Video streaming stopped for client " + request.clientAddress().toString());
	server.HandleClientLostConnection(request.clientAddress());
}
