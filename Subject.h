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

	void setImage(Mat sI);
	Mat getImage();
	void setKeypoints(vector<cv::KeyPoint> kp);
	vector<cv::KeyPoint> getKeypoints();
	void checkSubjectKeypoints();
	void setDescriptors(Mat ds);
	Mat getDescriptors();
	void cleanUp();
	Subject();
	~Subject();
};

