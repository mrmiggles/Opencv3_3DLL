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

void Subject::clearDescAsChar()
{
	delete[] descAsChar;
}

char * Subject::getDescriptorsAsText()
{
	std::string com = "[";
	std::string text = "";
	
	for (int i = 0; i < descriptorsA.rows; i++) {
		for (int j = 0; j<descriptorsA.cols; j++) {
			text += com + std::to_string(descriptorsA.at<float>(i, j));
			com = ",";
		}
	}

	text += "]";
	descAsChar = new char[text.size() + 1];
	std::copy(text.begin(), text.end(), descAsChar);
	descAsChar[text.size()] = '\0';
	return descAsChar;
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
	clearDescAsChar();
}

Subject::Subject()
{
}


Subject::~Subject()
{
}
