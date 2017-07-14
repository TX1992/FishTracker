#include "main.h"

using namespace std;
using namespace cv;
using namespace tinyxml2;

string datafolder ="../../data";
int radius = 15;

int main(int argc, char* argv[])
{
	if (argc < 2) 
	{
		cerr << "Error introducing input arguments!" << endl;
		cerr << "The format needs to be: ./ReadXML_test xmlfile" << endl;
		return -1;
	}
	string xml_file = argv[1];

	XMLDocument xml_doc;
	XMLError eResult = xml_doc.LoadFile(xml_file.c_str());
	if (eResult != XML_SUCCESS) 
		return false;

	XMLText* textNode2 = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "positive" )->FirstChildElement( "number" )->FirstChild()->ToText();
	int no_pos_obj = atoi(textNode2->Value());
	XMLText* textNode3 = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "negative" )->FirstChildElement( "number" )->FirstChild()->ToText();
	int no_neg_obj = atoi(textNode3->Value());

	XMLNode* root = xml_doc.FirstChild();

	XMLText* textNode = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "folder" )->FirstChild()->ToText();
	const char* foldername = textNode->Value();
	string foldername1 = foldername;

	XMLText* textNode1 = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "filename" )->FirstChild()->ToText();
	const char* imgfilename = textNode1->Value();
	string imgfilename1 = imgfilename;

	string imgfile = datafolder + "/" + foldername1 + "/" + imgfilename1;

	Mat image = imread(imgfile);
	string window_name = "Image";
	namedWindow(window_name, WINDOW_NORMAL);
	imshow(window_name, image);

	XMLElement* textNode_obj = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "positive" )->FirstChildElement( "object" );

	for (int i=0;i<no_pos_obj;i++)
	{
		if (i!=0)
		{
			textNode_obj = textNode_obj->NextSiblingElement();
		}
		XMLText* textNode_obj1 = textNode_obj->FirstChildElement( "center" )->FirstChild()->ToText();
		const char* center = textNode_obj1->Value();
		string center1 = center;
		Point center_point = getCenter(center1);

		Point pt1(center_point.x-radius,center_point.y-radius);//top
		Point pt2(center_point.x+radius,center_point.y+radius);//bottom
		rectangle(image, pt1, pt2, Scalar(0,255,0), 1, 8);
		imshow(window_name, image);
	}

	XMLElement* textNode_negobj = xml_doc.FirstChildElement( "annotation" )->FirstChildElement( "negative" )->FirstChildElement( "object" );

	for (int i=0;i<no_neg_obj;i++)
	{
		if (i!=0)
		{
			textNode_negobj = textNode_negobj->NextSiblingElement();
		}
		XMLText* textNode_obj1 = textNode_negobj->FirstChildElement( "center" )->FirstChild()->ToText();
		const char* center = textNode_obj1->Value();
		string center1 = center;
		Point center_point = getCenter(center1);

		Point pt1(center_point.x-radius,center_point.y-radius);//top
		Point pt2(center_point.x+radius,center_point.y+radius);//bottom
		rectangle(image, pt1, pt2, Scalar(0,0,255), 1, 8);
		imshow(window_name, image);
	}

	waitKey(-1);
	return 0;
}

cv::Point getCenter(const std::string& path)
{
	Point center;
	size_t lastdot = path.find_last_of(",");
	center.y = atoi(path.substr(lastdot + 1).c_str());
	center.x = atoi(path.substr(0, lastdot).c_str());
	return center;
}