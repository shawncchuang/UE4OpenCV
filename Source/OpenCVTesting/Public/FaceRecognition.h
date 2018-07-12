#define OPENCVTESTING_API  

#pragma once 
#include <vector>
/*
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp" 
*/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;


class OPENCVTESTING_API FaceRecognition {

public:
	FaceRecognition();

	bool captureCamera(int cameraId, int width, int height);
	bool getActualVideoSize(int& width, int& height);
	void releaseCamera();
	bool loadClassifierFile(const char *classifierFile);
	
	void update(std::vector<uchar>& buffer);

	static cv::Mat frame;

	void getFrame(Mat& frame);

private:

	VideoCapture* stream;
	bool m_isOpen;

	cv::CascadeClassifier faceCascade;
};


   
