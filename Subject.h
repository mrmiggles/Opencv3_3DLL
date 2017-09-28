#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>


using namespace std;
using namespace cv;
class Subject
{
public:
	Mat subjectImage;
	vector<cv::KeyPoint> keypointsA;
	Mat descriptorsA;
	Mat descriptorsB;
	char *descAsChar;
	uchar *image_uchar;
	vector<float> tmpMatVector;

	void setImage(Mat sI);
	Mat getImage();

	void setKeypoints(vector<cv::KeyPoint> kp);
	vector<cv::KeyPoint> getKeypoints();
	void printDescriptors();
	void checkSubjectKeypoints();

	void setDescriptors(Mat ds);
	void MatToBytes();
	cv::Mat BytesToMat();
	Mat getDescriptors();
	char *getDescriptorsAsText();
	char * getKeypointsAsText();
	void addToKeyPoints(float x, float y);
	char * getDescriptorsAsChar();
	void clearDescAsChar();
	
	void cleanUp();
	Subject();
	~Subject();
};

