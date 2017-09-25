#include "stdafx.h"
#include "MatcherW.h"


void MatcherW::setMatcher(Ptr<DescriptorMatcher> m)
{
	matcher = m;
}

void MatcherW::findMatches(Mat descriptorsA, Mat descriptorsB)
{
	matcher->match(descriptorsA, descriptorsB, matches);
}

void MatcherW::findknnMatches(Mat descriptorsA, Mat descriptorsB)
{
	matcher->knnMatch(descriptorsA, descriptorsB, nn_matches, 2);
}

bool MatcherW::checkIfGoodMatch() {
	int count = 0;
	for (size_t i = 0; i < nn_matches.size(); i++) {
		DMatch first = nn_matches[i][0];
		float dist1 = nn_matches[i][0].distance;
		float dist2 = nn_matches[i][1].distance;

		if (dist1 < 0.6f * dist2) {
			count++;
			//cout << "Acceptable distance between: " << dist1 << " and " << dist2 << endl;
			good_matches.push_back(first);
		}
	}

	if (count > 7) {
		cout << "Object found!" << endl;
		return true;
	}
	else if (count >= 2) {
		cout << "Possible Detection. Number of good matches: " << count << endl;
	}
	else {
		cout << "Object not found!" << endl;
		return false;
	}
}
bool MatcherW::checkIfGoodMatch(float nndrRatio) {

	for (size_t i = 0; i < nn_matches.size(); i++) {
		DMatch first = nn_matches[i][0];
		float dist1 = nn_matches[i][0].distance;
		float dist2 = nn_matches[i][1].distance;

		if (dist1 < nndrRatio * dist2) {
			good_matches.push_back(first);
		}
	}

	if (good_matches.size() > 7) {
		cout << "Object found!" << endl;
		return true;
	}
	else {
		cout << "Object not found!" << endl;
		return false;
	}
}

void MatcherW::paintGoodMatches(Mat subject, Mat scene, vector<cv::KeyPoint> keypointsA, vector<cv::KeyPoint> keypointsB)
{
	Mat good;
	drawMatches(subject, keypointsA, scene, keypointsB, good_matches, good, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("BRISK Good Matches", good);
	waitKey(0);
}


bool MatcherW::checkGoodMatchWHomography(Mat subjectImage, vector<KeyPoint> kpts1, Mat sceneImage, vector<KeyPoint> kpts2, Mat subjectDescriptors)
{
	Mat homography, inlier_mask;

	bool retvalue = false;

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < subjectDescriptors.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- good matches whose distance is less than 3*min_dist )
	for (int i = 0; i < subjectDescriptors.rows; i++)
	{
		if (matches[i].distance < min_dist)
		{
			//cout << matches[i].distance << endl;
			//cout << 3 * min_dist << endl;
			good_matches.push_back(matches[i]);
		}
	}

	if (good_matches.size() > 7) {
		cout << "Object found!" << endl;
		retvalue = true;
	}
	else {
		cout << "Object NOT found!" << endl;
		retvalue = false;
	}

	//Draw only "good" matches
	Mat img_matches;
	drawMatches(subjectImage, kpts1, sceneImage, kpts2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/*
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (int i = 0; i < good_matches.size(); i++)
	{
	//-- Get the keypoints from the good matches
	obj.push_back(kpts1[good_matches[i].queryIdx].pt);
	scene.push_back(kpts2[good_matches[i].trainIdx].pt);
	}



	Mat H = findHomography(obj, scene, CV_RANSAC);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(subjectImage.cols, 0);
	obj_corners[2] = cvPoint(subjectImage.cols, subjectImage.rows); obj_corners[3] = cvPoint(0, subjectImage.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )

	line(img_matches, scene_corners[0] + Point2f(subjectImage.cols, 0), scene_corners[1] + Point2f(subjectImage.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(subjectImage.cols, 0), scene_corners[2] + Point2f(subjectImage.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(subjectImage.cols, 0), scene_corners[3] + Point2f(subjectImage.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(subjectImage.cols, 0), scene_corners[0] + Point2f(subjectImage.cols, 0), Scalar(0, 255, 0), 4);
	*/
	//-- Show detected matches
	resize(img_matches, img_matches, Size(img_matches.cols / 2, img_matches.rows / 2));
	namedWindow("Good Matches & Object detection", CV_WINDOW_AUTOSIZE);
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);



	return retvalue;
}

/* checkGoodMatchWHomography must be called first */
void MatcherW::drawGoodMatchesWHomography(Mat img_object, Mat img_scene, vector<KeyPoint> keypoints_object, vector<KeyPoint> keypoints_scene, Mat homography) {


	Mat img_matches;
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);
	perspectiveTransform(obj_corners, scene_corners, homography);
	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	//-- Show detected matches
	imshow("Good Matches & Object detection", img_matches);
	waitKey(0);
}

void MatcherW::cleanUp() {

	matches.clear();
	nn_matches.clear();
	good_matches.clear();
	matched1.clear();
	matched2.clear();
	inliers1.clear();
	inliers2.clear();
}

MatcherW::MatcherW()
{
	//dwRet = GetCurrentDirectoryA(sizeof(working_directory), working_directory);
}


MatcherW::~MatcherW()
{
}
