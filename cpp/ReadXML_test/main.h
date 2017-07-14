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

#include <tinyxml2.h>

cv::Point getCenter(const std::string& path);