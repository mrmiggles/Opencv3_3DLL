// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

//define 
#define DECLDIR __declspec(dllexport)
#define DLL_EXPORT

// Windows Header Files:
#include <windows.h> 

// TODO: reference additional headers your program requires here
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Subject.h"
#include "MatcherW.h"


using namespace std;
using namespace cv;

extern "C"
{
	DECLDIR void enableCoutToFile();
	DECLDIR bool setSubjectImage(void *buf, int h1, int w1);
	DECLDIR bool checkSubject();
	DECLDIR bool setDetector(int type);
	DECLDIR bool compareToSubject(void *buf, int h1, int w1);
	DECLDIR void cleanUp();
	void checkDetector();
	void checkMatcher();
	int saveMat(const string& filename, const Mat& M);
	void saveMatToYML(const string& filename, const Mat& M);
}