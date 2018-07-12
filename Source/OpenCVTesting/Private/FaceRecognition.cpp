// FaceRecognition.cpp : Defines the exported functions for the DLL application.
//

#include "FaceRecognition.h"
#include <array>
#include <iostream>
#include <mutex>
#include "opencv2/imgproc.hpp"
#include "Engine.h"

using namespace cv;
using namespace std;

std::mutex frame_mutex;
//cv::Mat FaceRecognition::androidFrame;
cv::Mat FaceRecognition::frame;

FaceRecognition::FaceRecognition()
{
	m_isOpen = false;
}

bool FaceRecognition::captureCamera(int cameraId, int width, int height)
{
 
	stream = new cv::VideoCapture();
	stream->open(cameraId);
	stream->set(CV_CAP_PROP_FRAME_WIDTH, width);
	stream->set(CV_CAP_PROP_FRAME_HEIGHT, height);

	m_isOpen = stream->isOpened();
	return m_isOpen;
}

bool FaceRecognition::getActualVideoSize(int& width, int& height)
{
	
	if (m_isOpen && stream != nullptr) {
		Mat frame;
		getFrame(frame);
		//stream->read(frame);
		if (!frame.data) {
			return false;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("frame.data"));
		width = frame.size().width;
		height = frame.size().height;
		return true;
	}
	return false;
}

void FaceRecognition::releaseCamera()
{
	if (m_isOpen && stream != nullptr && stream->isOpened()) {
		stream->release();
		delete stream;
	}

	m_isOpen = false;
}

bool FaceRecognition::loadClassifierFile(const char *classifierFile)
{
	return faceCascade.load(classifierFile);
}

void FaceRecognition::getFrame(Mat& frame)
{
	if (!m_isOpen) {
		return;
	}
	frame_mutex.lock();
	//frame = FaceRecognition::androidFrame.clone();
	frame = FaceRecognition::frame.clone();
	stream->read(frame);
	frame_mutex.unlock();
}

void FaceRecognition::update(std::vector<uchar>& buffer)
{
	Mat frame;
	getFrame(frame);

	if (!frame.data) {
		return;
	}

	std::vector<cv::Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//stream->retrieve(frame, CV_CAP_OPENNI_BGR_IMAGE);

	faceCascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(60, 60));

 
	for (int i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
	}
 
	
	(buffer).assign(frame.datastart, frame.dataend);
 
}


 