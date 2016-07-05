#include "WebCamController.h"
#include "WebcamService.h"

WebCamController::WebCamController(SharedPtr<WebcamService> webcamService) : 
	webcamService(webcamService) 
	{}

WebCamController::~WebCamController()	{
	//do not delete, since it is a shared pointer
	webcamService = nullptr;
}

void WebCamController::StartWebCam() {
	std::cout << "in StartWebCam Function" << std::endl;
	if (!webcamService->IsRecording()) {
		webcamService->StartRecording();
		std::cout << "Called StartRecording" << std::endl;
	}
}

void WebCamController::StopWebCam() {
	if (webcamService->IsRecording()) {
		webcamService->StopRecording();
	}
}
