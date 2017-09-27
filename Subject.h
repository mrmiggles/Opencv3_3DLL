#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>


using namespace std;
using namespace cv;
class Subject
{
public:
	Mat subjectImage;
	vector<cv::KeyPoint> keypointsA;
	Mat descriptorsA;
	char *descAsChar;
	uchar *image_uchar;

	void setImage(Mat sI);
	Mat getImage();

	void setKeypoints(vector<cv::KeyPoint> kp);
	vector<cv::KeyPoint> getKeypoints();
	void checkSubjectKeypoints();

	void setDescriptors(Mat ds);
	void MatToBytes();
	cv::Mat BytesToMat();
	Mat getDescriptors();
	char *getDescriptorsAsText();
	char * getDescriptorsAsChar();
	void clearDescAsChar();
	
	void cleanUp();
	Subject();
	~Subject();
};

