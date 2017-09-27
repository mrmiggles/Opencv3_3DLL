// OpenCV3_3_DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
extern "C" {
	Ptr<FeatureDetector> detector;
	Ptr<DescriptorMatcher> m;
	char *mt;  //Char for checking which matcher we have
			   //Ptr<DescriptorExtractor> extractor;
	Subject s; //Image subject class
	MatcherW matcher; //Opencv Matcher Wrapper class just to hide a few functions

	DECLDIR bool setSubjectFromString(char *mat) {

		cout << mat << endl;
		return true;
	}

	DECLDIR bool setSubjectImage(void *buf, int h1, int w1) {
		if (!buf) {
			cout << "Initial Buffers were null" << endl;
			return false;
		}


		Mat subjectImage;
		if (!subjectImage.empty()) {
			cout << "Image already set" << endl;
			return true;
		}

		/* Set Global subjectImage (the image we're comparing to every other image) */
		subjectImage = Mat(h1, w1, CV_8UC3, Scalar(0, 0, 0));
		subjectImage.data = (uchar *)buf;
		//subjectImage.convertTo(subjectImage, CV_BGR2GRAY);

		if (!subjectImage.data && subjectImage.empty()) {
			return false;
		}
		s.setImage(subjectImage);
		return true;
	}

	DECLDIR bool setSubjectKeypoints() {
		vector<cv::KeyPoint> keypointsA;
		Mat descriptorsA;
		checkDetector(); //check if detector has been set

		detector->detectAndCompute(s.getImage(), noArray(), keypointsA, descriptorsA);
		s.setKeypoints(keypointsA);
		s.setDescriptors(descriptorsA);
		descriptorsA.release();
		keypointsA.clear();

		return true;
	}

	DECLDIR bool checkSubject() {

		if (!s.getImage().data && s.getImage().empty()) {
			cout << "Subject image is empty" << endl;
			return false;
		}
		cout << "Subject is set" << endl;
		return true;
	}

	/*
		
	*/
	DECLDIR char* getSubjectDescriptors() {
		
		return s.getDescriptorsAsText();
	}

	DECLDIR uchar* getDesc(){
		s.MatToBytes();
		return s.image_uchar;
	}

	DECLDIR void printSubjectDescriptors() {

		cout << "subject descriptors from DLL:" << endl;
		cout << s.getDescriptors() << endl;
	}

	DECLDIR bool setDetector(int type) {
		char *det;
		switch (type) {
		case 1: //"AKAZE";
			detector = AKAZE::create();
			det = "AKAZE";
			break;
		case 2: //"ORB";
			detector = ORB::create(10000);
			det = "ORB";
			break;
		case 3: //"SURF";
			det = "SURF";
			break;
		case 4: //"SIFT";
			det = "SIFT";
			break;
		case 5: //"FREAK";
			det = "FREAK";
			break;
		default:
			//"BRISK";
			//Brisk Parameters
			int Threshl = 45;
			int Octaves = 4;
			float PatternScales = 1.0f;
			detector = BRISK::create(Threshl, Octaves, PatternScales);

			det = "BRISK";
			break;
		}

		cout << det << " detector set" << endl;
		return true;
	}

	DECLDIR void setMatcher(int type) {

		switch (type) {
		case 1: //"FLANN";
			mt = "FLANN";
			m = FlannBasedMatcher::create();//DescriptorMatcher::create("FlannBased");//
			break;
		default: //"Brute Force"
			mt = "Brute Force";
			m = DescriptorMatcher::create("BruteForce-Hamming");//BFMatcher::create(NORM_HAMMING);
			break;
		}

		cout << mt << " Matcher set" << endl;
		matcher.setMatcher(m);
	}

	DECLDIR bool compareToSubject(void *buf, int h1, int w1) {
		if (!buf) {
			cout << "Image to compare was null" << endl;
			return false;
		}

		Mat sceneImage;
		/* Set Global subjectImage (the image we're comparing to every other image) */
		sceneImage = Mat(h1, w1, CV_8UC3, Scalar(0, 0, 0));
		sceneImage.data = (uchar *)buf;
		//sceneImage.convertTo(sceneImage, CV_BGR2GRAY);
		if (!sceneImage.data && sceneImage.empty()) {
			cout << "Could not creat cv::Mat from buffer" << endl;
			return false;
		}

		vector<cv::KeyPoint> keypointsB;
		Mat descriptorsB;
		detector->detectAndCompute(sceneImage, noArray(), keypointsB, descriptorsB);

		checkMatcher();


		//FLANN REQUIRES desciriptors of type CV_32F
		if (mt == "FLANN") {
			if (s.getDescriptors().type() != CV_32F) {
				Mat ds;
				s.getDescriptors().convertTo(ds, CV_32F);
				s.setDescriptors(ds);
				ds.release();
			}

			if (descriptorsB.type() != CV_32F) {
				descriptorsB.convertTo(descriptorsB, CV_32F);
			}
		}

		//matcher.findknnMatches(s.getDescriptors(), descriptorsB);
		//bool ev = matcher.checkIfGoodMatch();
		//matcher.paintGoodMatches(s.getImage(), sceneImage, s.getKeypoints(), keypointsB);

		matcher.findMatches(s.getDescriptors(), descriptorsB);
		bool ev = matcher.checkGoodMatchWHomography(s.getImage(), s.getKeypoints(), sceneImage, keypointsB, s.getDescriptors());

		descriptorsB.release();
		keypointsB.clear();
		sceneImage.release();
		matcher.cleanUp();
		return ev;

	}

	DECLDIR void cleanUp() {
		s.cleanUp();
		detector.release();
		m.release();
	}

	/* Non-export functions */
	void checkDetector() {
		if (!detector) {
			setDetector(0);
		}
	}
	void checkMatcher() {
		if (!m) {
			setMatcher(0);
		}
	}

	// Save matrix to binary file
	int saveMat(const string& filename, const Mat& M) {
		if (M.empty()) {
			return 0;
		}
		ofstream out(filename.c_str(), ios::out | ios::binary);
		if (!out)
			return 0;

		int cols = M.cols;
		int rows = M.rows;
		int chan = M.channels();
		int eSiz = (M.dataend - M.datastart) / (cols*rows*chan);

		// Write header
		out.write((char*)&cols, sizeof(cols));
		out.write((char*)&rows, sizeof(rows));
		out.write((char*)&chan, sizeof(chan));
		out.write((char*)&eSiz, sizeof(eSiz));

		// Write data.
		if (M.isContinuous()) {
			out.write((char *)M.data, cols*rows*chan*eSiz);
		}
		else {
			return 0;
		}

		cout << "closing stream" << endl;
		out.close();
		return 1;
	}

	void saveMatToYML(const string& filename, const Mat& M) {
		FileStorage fs(filename, FileStorage::WRITE);
		fs << "yourMat" << M;
		
	}
}