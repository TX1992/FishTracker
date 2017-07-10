
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
	cv::Scalar& color, int thickness, int lineType);

int main()
{
	Mat image = imread("img.jpg");
	//Mat image = imread("C:\\Users\\lenovo\\Desktop\\Codes\\Blob Detection\\11\\CAM0_11_00_16__04.jpg");
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);

	vector<Point2f> points;
	goodFeaturesToTrack(gray,
		points,     //store detected corners
		100,        //the maximum number of corners you want to retain
		0.05,       //qualitylevel  eg： best corner has the quality measure = 1500, and the
					//qualityLevel = 0.01, then all the corners with the quality measure less than 15 are rejected.
		10.,       //Minimum distances between two corner points：Minimum possible Euclidean distance between the returned corners
		Mat(),    //region of interest 
		9,         //w(x, y)
		true,     //useHarrisDetector
		0.04);    //cornerHarris 's K parameter

	Mat eigens;
	cornerEigenValsAndVecs(gray, eigens, 9, 5);

	for (vector<Point2f>::const_iterator itBegin = points.begin(); itBegin != points.end(); itBegin++)
	{
	    int radius = 6;
		Point center = *itBegin;
		Vec6f eigen = eigens.at<Vec6f>(center); //eigenvalues and eigenvectors
		float minor = 1 / sqrt(eigen[0]);  //minor axis of ellipse
		float primary = 1 / sqrt(eigen[1]); //primary axis of ellipse
		float angle = atan(eigen[5] / eigen[4]); //orientation angle of ellipse
		Point end = Point(center.x - sin(angle) * radius, center.y - cos(angle) * radius);
		if (primary / minor > 3)   //size constraints
			continue;
		//ellipse(image, center, Size(primary, minor), angle, 0, 360, Scalar(0, 255, 0), 1);
		circle(image, center, radius, Scalar(0,255,0), 1);	
		//drawArrow(image, center, end, 3, 60, Scalar(255, 0, 0), 1, CV_AA);
	}
	namedWindow("corners", 0);
	imshow("corners", image);
	waitKey();
	return 0;
}

//draw arrows
void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
	cv::Scalar& color, int thickness, int lineType)
{
	const double PI = 3.1415926;
	Point arrow;
	//计算 θ 角（最简单的一种情况在下面图示中已经展示，关键在于 atan2 函数，详情见下面）   
	double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));
	line(img, pStart, pEnd, color, thickness, lineType);
	//计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置） 
	arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);
	arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);
	line(img, pEnd, arrow, color, thickness, lineType);
	arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);
	arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);
	line(img, pEnd, arrow, color, thickness, lineType);
}