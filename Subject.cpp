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
	cout << "rows: " << descriptorsA.rows << endl;
	cout << "cols: " << descriptorsA.cols << endl;
	cout << "type: " << descriptorsA.type() << endl;
	std::string com = "";// "[";
	std::string text = "";
	

	for (int i = 0; i < descriptorsA.rows; i++) {
		for (int j = 0; j<descriptorsA.cols; j++) {
			//text += com + std::to_string(descriptorsA.at<float>(i, j)); //losing precision
			std::ostringstream out;
			out << descriptorsA.at<float>(i, j);
			text += com + out.str();
			out.clear();
			com = ",";
		}
	}

	//text += "]";
	descAsChar = new char[text.size() + 1];
	std::copy(text.begin(), text.end(), descAsChar);
	descAsChar[text.size()] = '\0';
	return descAsChar;
}

char * Subject::getKeypointsAsText() {
	std:string text = "";
	//cout << keypointsA.data() << endl;
	
	ofstream outfile("C:\\output\\subjectKeypoints.txt", ios::out | std::ofstream::binary);
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(outfile.rdbuf()); //redirect std::cout to new.txt!
	for (int i = 0; i < keypointsA.size(); i++) {
		cout << std::to_string(keypointsA.at(i).pt.x) << " " << std::to_string(keypointsA.at(i).pt.y) << ",";
	}
	
	cout << "keypoints size: " << keypointsA.size() << endl;
	

	return NULL;
}

void Subject::addToKeyPoints(float x, float y) {
	cv::KeyPoint kp;
	cv::Point2f p;
	p.x = x;
	p.y = y;
	kp.pt = p;
	keypointsA.push_back(kp);
}


 void Subject::MatToBytes()
{
	// = *pimage_uchar;
	//class data members of ints
	int image_rows = descriptorsA.rows;
	int image_cols = descriptorsA.cols;
	int image_type = descriptorsA.type();

	int image_size = descriptorsA.total() * descriptorsA.elemSize();
	image_uchar = new uchar[image_size];
	std::memcpy(image_uchar, descriptorsA.data, image_size * sizeof(uchar));
	//return 1;
}

cv::Mat Subject::BytesToMat()
{
	cv::Mat img(descriptorsA.rows, descriptorsA.cols, descriptorsA.type(), image_uchar, cv::Mat::AUTO_STEP);
	return img;
}

Mat Subject::getDescriptors()
{
	return descriptorsA;
}

vector<cv::KeyPoint> Subject::getKeypoints()
{
	return keypointsA;
}

void Subject::printDescriptors() {
	cout << descriptorsA << endl;
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
