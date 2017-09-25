#pragma once

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
class MatcherW
{
public:
	Ptr<DescriptorMatcher> matcher;
	vector<DMatch> matches;
	vector<vector<DMatch>> nn_matches;
	vector<DMatch> good_matches;
	vector<KeyPoint> matched1, matched2, inliers1, inliers2;

	/*stuff for working directory */
	char working_directory[MAX_PATH + 1];
	DWORD dwRet;

	const float inlier_threshold = 2.5f; // Distance threshold to identify inliers
	const double akaze_thresh = 3e-4; // AKAZE detection threshold set to locate about 1000 keypoints
	const double ransac_thresh = 2.5f; // RANSAC inlier threshold
	const double nn_match_ratio = 0.7f; // Nearest-neighbour matching ratio

	void setMatcher(Ptr<DescriptorMatcher> m);
	void findMatches(Mat descriptorsA, Mat descriptorsB);
	void findknnMatches(Mat descriptorsA, Mat descriptorsB);
	bool checkIfGoodMatch();
	bool checkIfGoodMatch(float nndrRatio);
	bool checkGoodMatchWHomography(Mat subject, vector<KeyPoint> kpts1, Mat scene, vector<KeyPoint> kpts2, Mat subjectDescriptors);
	void paintMatches(Mat subject, Mat scene, vector<cv::KeyPoint> keypointsA, vector<cv::KeyPoint> keypointsB);
	void paintGoodMatches(Mat subject, Mat scene, vector<cv::KeyPoint> keypointsA, vector<cv::KeyPoint> keypointsB);
	void drawGoodMatchesWHomography(Mat img_object, Mat img_scene, vector<KeyPoint> keypoints_object, vector<KeyPoint> keypoints_scene, Mat homography);
	void cleanUp();
	MatcherW();
	~MatcherW();
};