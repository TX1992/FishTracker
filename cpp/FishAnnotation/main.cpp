#include "main.h"

using namespace std;
using namespace cv;

int cnt = (-1), key = 0;
int xp1,yp1;
string imgfolder_res ="../../res";
char filename_res[256];

int main()
{
	string res_file_pos = imgfolder_res + "/" + "annotation_pos.txt";
	string res_file_neg = imgfolder_res + "/" + "annotation_neg.txt";
	ofstream resfile_pos;
	resfile_pos.open(res_file_pos, ios::out | ios::app);
	ofstream resfile_neg;
	resfile_neg.open(res_file_neg, ios::out | ios::app);

	string data_file = imgfolder_res + "/" + "data.txt";
	ifstream datafile;
	datafile.open(data_file);

	string window_name = "Click for co-ordinates, Press D for display, Press S for save, Press Q for next image";

	int check[200];
	int check_size;

	string filename;
	if (datafile)
	{
		while (getline( datafile, filename ))
		{
			//string filename = "../../data/CAM0_11_00_15__03.jpg";
	
			Mat image = imread(filename);
			Mat img_orig = image.clone();
			Mat gray;
			cvtColor(image, gray, CV_BGR2GRAY);

			string filename_img = getFilename(filename);

			//detect the fish head
			vector<Point2f> points;
			goodFeaturesToTrack(gray,
				points,     //store detected corners
				100,        //the maximum number qof corners you want to retain
				0.1,       //qualitylevel  eg： best corner has the quality measure = 1500, and the
							//qualityLevel = 0.01, then all the corners with the quality measure less than 15 are rejected.
				20.,       //Minimum distances between two corner points：Minimum possible Euclidean distance between the returned corners
				Mat(),    //region of interest 
				13,         //w(x, y)
				true,     //useHarrisDetector
				0.04);    //cornerHarris 's K parameter

			Mat eigens;
			cornerEigenValsAndVecs(gray, eigens, 9, 5);
			int radius = 15;

			//checker for annotation
			check_size = points.size();
			for (int i=0;i<check_size;i++)
				check[i]=0;
			int idx_save = 0, idx_constraint = (-1);

			for (vector<Point2f>::const_iterator itBegin = points.begin(); itBegin != points.end(); itBegin++)
			{
				idx_constraint++;

				Point center = *itBegin;
				Vec6f eigen = eigens.at<Vec6f>(center); //eigenvalues and eigenvectors
				float minor = 1 / sqrt(eigen[0]);  //minor axis of ellipseq
				float primary = 1 / sqrt(eigen[1]); //primary axis of ellipse
				float angle = atan(eigen[5] / eigen[4]); //orientation angle of ellipse
				Point end = Point(center.x - sin(angle) * radius, center.y - cos(angle) * radius);
				if (primary / minor > 3)   //size constraints
				{
					check[idx_constraint]=1;
					continue;
				}
				//ellipse(image, center, Size(primary, minor), angle, 0, 360, Scalar(0, 255, 0), 1);
				//circle(image, center, radius, Scalar(0,255,0), 1, 8);	
				Point pt1(center.x-radius,center.y-radius);
				Point pt2(center.x+radius,center.y+radius);
				rectangle(image, pt1, pt2, Scalar(0,255,0), 1, 8);
				//drawArrow(image, center, end, 3, 60, Scalar(255, 0, 0), 1, CV_AA);
			}
			namedWindow(window_name, 0);
			cvSetMouseCallback(window_name.c_str(), mouseEvent, NULL);
			imshow(window_name, image);

			Mat img_crop, img_disp;
			Point closest_pts;

			string filename_jpg = filename_img + ".jpg";

			//add code to do annotation
			while (1)
			{
				key = cvWaitKey(0);

				switch (key)
				{
					case 'd':
						{
							Point point_marked(xp1,yp1);
							double min_dist = 100000;
							int idx = 0, i = 0;
							for (vector<Point2f>::const_iterator itBegin = points.begin(); itBegin != points.end(); itBegin++)
							{
								Point center = *itBegin;
								double dx = point_marked.x - center.x;
								double dy = point_marked.y - center.y;

								double tmp = dx*dx + dy*dy;
								double distance = sqrt(tmp);
								if (distance < min_dist)
								{
									min_dist = distance;
									idx = i;
								}
								i++;
							}
							idx_save = idx;
							closest_pts.x = points[idx].x;
							closest_pts.y = points[idx].y;//center
							Point pt1(closest_pts.x-radius,closest_pts.y-radius);
							Point pt2(closest_pts.x+radius,closest_pts.y+radius);
							img_disp = image.clone();
							rectangle(img_disp, pt1, pt2, Scalar(0,0,255), 1, 8);
							imshow(window_name, img_disp);

							continue;
						}
					case 's':
						{
							cnt++;
							
							//sprintf_s(filename_res,"%s/%s_head%d.jpg", imgfolder_res.c_str(), getFilename(filename).c_str(), cnt);
							/*cv::Rect roi;
							roi.x = closest_pts.x-6;
							roi.y = closest_pts.y-6;
							roi.width = 12;
							roi.height = 12;
							img_crop = img_orig(roi);
							imwrite(filename_res, img_crop);*/

							Point pt1(closest_pts.x-radius,closest_pts.y-radius);//top
							Point pt2(closest_pts.x+radius,closest_pts.y+radius);//bottom
							rectangle(image, pt1, pt2, Scalar(0,0,255), 1, 8);

							if (!check[idx_save])
								resfile_pos << filename << "\t" << closest_pts.x << "\t" << closest_pts.y << "\n";
							check[idx_save]=1;

							continue;
						}
					case 'q':
						{
							for (int i=0;i<check_size;i++)
								if (!check[i])
									resfile_neg << filename << "\t" << points[i].x << "\t" << points[i].y << "\n";
							break;
						}
				}
				break;
			}
		}
		datafile.close();
	}

	resfile_pos.close();
	resfile_neg.close();
	//waitKey();
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

void mouseEvent(int evt, int m, int n, int flags, void* param)
{
    if(evt == CV_EVENT_LBUTTONDOWN)
	{
		xp1 = m;
		yp1 = n;
		cout << "Left button of the mouse is clicked - position (" << m << ", " << n << ")" << endl;
		cout << "count = " << cnt << endl;
		//printf("Point clicked = %d %d\n",m,n);
    }
}

std::string getFilename(const std::string& path)
{
	size_t lastdot = path.find_last_of(".");
	string str1;
	string str2;
	if(path.substr(lastdot + 1) == "jpg")
		str1 = path.substr(0, lastdot);
	size_t lastslash = str1.find_last_of("/");
	str2 = str1.substr(lastslash+1);
	return str2;
}
