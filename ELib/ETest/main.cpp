// ETest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Tangram/TangramDetection.h"
#include <vector>

using namespace cv;
using namespace std;
cv::Mat rotateImage(cv::Mat img, int degree)
{
	//旋转中心为图像中心
	cv::Point center;
	center.x = float(img.cols / 2.0 + 0.5);
	center.y = float(img.rows / 2.0 + 0.5);
	//计算二维旋转的仿射变换矩阵
	float m[6];
	cv::Mat M = cv::Mat(2, 3, CV_32F, m);
	M = cv::getRotationMatrix2D(center, degree, 1);
	//变换图像，并用黑色填充其余值

	cv::Mat img_rotate;
	cv::warpAffine(img, img_rotate, M, img.size());
	return img_rotate;
}
void Test()
{

	//cv::Mat src = cv::imread("Parallel.jpg");
	//cv::Mat src = cv::imread("Triangle.jpg");
	//cv::Mat src = cv::imread("Square.jpg");
	//cv::Mat gray;
	//cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	//cv::threshold(gray, gray, 200, 255, cv::THRESH_BINARY_INV);

	//int step = 5;
	//for (int i = 0; i < 360; i += step)
	//{
	//	auto img_rot = rotateImage(gray, i);
	//	auto contour = FindLargestContour(img_rot);

	//	double epsilon = img_rot.rows / 16;
	//	std::vector<cv::Point> approxes;
	//	cv::approxPolyDP(contour, approxes, epsilon, true);

	//	for (int i = 0; i < approxes.size(); i++)
	//	{
	//		approxes[i] = cv::Point(approxes[i].x, img_rot.rows - approxes[i].y);
	//	}
	//	//TriangleElementInfo tri;
	//	SquareElementInfo sqrare;
	//	sqrare.Init(approxes, TangramElementInfo::TangramTypeName::TTN_PARA);

	//	printf("\n %f", sqrare.GetAngle());

	//	cv::imshow("img_rot", img_rot);
	//	cv::waitKey();
	//}


	/*int step = 1;
	for (int i = 0; i < 360; i += step)
	{
		auto img_rot = rotateImage(gray, i);
		auto contour = FindLargestContour(img_rot);

		double epsilon = img_rot.rows / 16;
		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);

		for (int i = 0; i < approxes.size(); i++)
		{
			approxes[i] = cv::Point(approxes[i].x, img_rot.rows - approxes[i].y);
		}
		ParallelElementInfo parallel;
		parallel.Init(approxes,TangramElementInfo::TangramTypeName::TTN_PARA);

		printf("\n %d %f", parallel.IsFlip(), parallel.GetAngle());
	}

	printf("\n-----------------------------------------------------------------------------------------");

	cv::flip(gray, gray, 0);

	for (int i = 0; i < 360; i += step)
	{
		auto img_rot = rotateImage(gray, i);
		auto contour = FindLargestContour(img_rot);

		double epsilon = img_rot.rows / 16;
		std::vector<cv::Point> approxes;
		cv::approxPolyDP(contour, approxes, epsilon, true);

		for (int i = 0; i < approxes.size(); i++)
		{
			approxes[i] = cv::Point(approxes[i].x, img_rot.rows - approxes[i].y);
		}

		ParallelElementInfo parallel;
		parallel.Init(approxes,TangramElementInfo::TangramTypeName::TTN_PARA);

		printf("\n %d %f", parallel.IsFlip(), parallel.GetAngle());
	}*/


}

cv::Mat FixWidthResize(cv::Mat src, int fixedWidth)
{
	cv::Mat ret;
	int fixedHeight = fixedWidth * src.rows / src.cols;
	cv::resize(src, ret, cv::Size(fixedWidth, fixedHeight));
	return ret;
}

void TangramDetection()
{
	cv::Mat templateFrame = cv::imread("ECat.jpg");
	templateFrame = FixWidthResize(templateFrame, 512);



	cv::Mat testImage = cv::imread("CatWithWood.png");
	testImage = FixWidthResize(testImage, 512);


	auto cap = cv::VideoCapture(0);
	TangramDetector detector;

	detector.SetTemplateGraph(templateFrame);


	bool quit = false;
	cv::Mat frame;

	int frameCount = 0;
	while (cap.isOpened()) {

		//real camera
		//bool ret = cap.read(frame);

		//test image
		frame = testImage.clone();
		bool ret = true;

		if (ret)
		{
			detector.Update(frame);

			cv::imshow("frame", frame);
			if (cv::waitKey(1) == 27)
			{
				break;
			}
		}
	}
}


void TestMoments()
{
	cv::Mat src_gray;
	cv::Mat src = imread("Tangram.png", 1);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	/// Convert image to gray and blur it
	//cvtColor( src, src_gray, COLOR_BGR2GRAY );
	//blur( src_gray, src_gray, Size(3,3) );
	//namedWindow( "image", WINDOW_AUTOSIZE );
	//imshow( "image", src );
	//Mat canny_output;
	////利用canny算法检测边缘
	//int thresh = 3;
	//Canny( src_gray, canny_output, thresh, thresh*2, 3 );
	//namedWindow( "canny", WINDOW_AUTOSIZE );
	//imshow("canny", canny_output);
	////查找轮廓
	//findContours(canny_output, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	
	

	cv::Mat hsv;
	cvtColor(src, hsv, COLOR_BGR2HSV);
	std::vector<cv::Mat> hsvChannels;
	cv::split(hsv, hsvChannels);
	cv::Mat gray;
	cv::threshold(hsvChannels[1], gray, 64, 255, cv::THRESH_BINARY_INV);
	//查找轮廓
	findContours(gray, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point(0, 0));


	vector<vector<Point>> approxContours(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		double epsilon = src.rows / 16;
		vector<Point> approx;
		approxPolyDP(contours[i], approx, epsilon, true);
		approxContours[i] = approx;

	}
	contours = approxContours;


	//计算轮廓矩
	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ )  
	{ 
		mu[i] = moments(contours[i], false );
	}
	//计算轮廓的质心
	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ )  
	{ 


		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}
	//画轮廓及其质心
	RNG rng;
	Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )   
	{   
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) ); 
		drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );  
		circle( drawing, mc[i], 4, color, -1, 8, 0 );  
	}
	namedWindow( "Contours", WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );
	//打印轮廓面积和轮廓长度
	printf("\t Info: Area and Contour Length \n");
	for( int i = 0; i< contours.size(); i++ )  
	{  
		printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) ); 
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		//drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );  
		//circle( drawing, mc[i], 4, color, -1, 8, 0 );  
	}

	imshow("drawing",drawing);
	waitKey();
}

int main()
{
	TangramDetection();
	//TestMoments();
}

