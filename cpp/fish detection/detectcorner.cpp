
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void drawCornerOnImage(Mat& image, const Mat&binary);
void drawCornerOnImage(Mat& image, const Mat&binary, const Mat& eigens);

int main()

{ 

	Mat image = imread("img.jpg");
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	cvNamedWindow("original", 0);
	imshow("original", gray);

	////reduce the fish fins
	//Mat shape = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	//dilate(gray, gray, shape);
	//GaussianBlur(gray, gray, Size(3, 3),0.5);
	//cvNamedWindow("dilation", 0);
	//imshow("dilation", gray);

	// Detect Harris Corners
	Mat cornerStrength;
	cornerHarris(gray, cornerStrength, 5, 5, 0.04);
	//compute eigenvalues and eigenvectors
	Mat eigens;
	cornerEigenValsAndVecs(gray, eigens, 5, 5);

	// find the maxima and minima
	double maxStrength;
	double minStrength;
	minMaxLoc(cornerStrength, &minStrength, &maxStrength);


	Mat dilated;
	Mat locaMax;
	Mat element = getStructuringElement(MORPH_RECT, Size(7, 7));
	// do non-maximum suppression by dilating image 
	dilate(cornerStrength, dilated, element);
	// comparision logic, to return the points with unchanged intensity values
	compare(cornerStrength, dilated, locaMax, CMP_EQ);

	Mat cornerMap;
	double qualityLevel = 0.05;
	double th = qualityLevel*maxStrength; // quality level
	threshold(cornerStrength, cornerMap, th, 255, THRESH_BINARY);
	cornerMap.convertTo(cornerMap, CV_8U);
	//  find points whose values are both local maximum and larger than threshold 
	bitwise_and(cornerMap, locaMax, cornerMap);

	drawCornerOnImage(image, cornerMap,eigens);
	namedWindow("result", CV_WINDOW_NORMAL);
	imshow("result", image);
	waitKey();

	return 0;

}

void drawCornerOnImage(Mat& image, const Mat&binary)
{
	Mat_<uchar>::const_iterator it = binary.begin<uchar>();
	Mat_<uchar>::const_iterator itd = binary.end<uchar>();
	for (int i = 0; it != itd; it++, i++)
	{
		if (*it)
			circle(image, Point(i%image.cols, i / image.cols), 5, Scalar(0, 255, 0), 1);
	}
}

void drawCornerOnImage(Mat& image, const Mat&binary,const Mat& eigens)
{
	Mat_<uchar>::const_iterator it = binary.begin<uchar>();
	Mat_<uchar>::const_iterator itd = binary.end<uchar>();
	for (int i = 0; it != itd; it++, i++)
	{
		if (*it)
		{
			int row = i / image.cols;
			int col = i%image.cols;
			Vec6f eigen = eigens.at<Vec6f>(row, col);
			float minor = 4/sqrt(eigen[0]);
			float primary = 4/sqrt(eigen[1]);
			float angle = atan(eigen[5] / eigen[4]) * 180.0;
			//circle(image, Point(i%image.cols, i / image.cols), 5, Scalar(0, 255, 0), 1);
			if (primary / minor > 3)
				continue;
			ellipse(image, Point(col, row), Size(primary, minor), angle, 0, 360, Scalar(0, 255, 0), 1);
		}
	}
}