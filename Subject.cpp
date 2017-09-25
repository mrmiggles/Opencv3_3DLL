#include "stdafx.h"
#include "Subject.h"


void Subject::setImage(Mat sI)
{
	subjectImage = sI.clone();
}

Mat Subject::getImage()
{
	return subjectImage;
}

void Subject::setKeypoints(vector<cv::KeyPoint> kp)
{
	keypointsA = kp;
}

void Subject::setDescriptors(Mat ds)
{
	descriptorsA = ds;
}

Mat Subject::getDescriptors()
{
	return descriptorsA;
}

vector<cv::KeyPoint> Subject::getKeypoints()
{
	return keypointsA;
}

void Subject::checkSubjectKeypoints() {
	cout << "Subject Keypoints: " << keypointsA.size() << endl;
	cout << "Subject Descriptors: " << descriptorsA.size() << endl;
}

void Subject::cleanUp()
{
	keypointsA.clear();
	descriptorsA.release();
	subjectImage.release();
}

Subject::Subject()
{
}


Subject::~Subject()
{
}
