#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"



void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
	cv::Scalar& color, int thickness, int lineType);

void mouseEvent(int evt, int m, int n, int flags, void* param);

std::string getFilename(const std::string& path);