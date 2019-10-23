#include "Tangram/TangramUtility.h"

double get_point_angle(cv::Point pointO, cv::Point pointA)
{
	double angle = 0;
	cv::Point point;
	double temp;

	point = cv::Point((pointA.x - pointO.x), (pointA.y - pointO.y));

	if ((0 == point.x) && (0 == point.y))
	{
		return 0;
	}

	if (0 == point.x)
	{
		angle = 90;
		return angle;
	}

	if (0 == point.y)
	{
		angle = 0;
		return angle;
	}

	temp = fabsf(float(point.y) / float(point.x));
	temp = atan(temp);
	temp = temp * 180 / CV_PI;

	if ((0 < point.x) && (0 < point.y))
	{
		angle = 360 - temp;
		return angle;
	}

	if ((0 > point.x) && (0 < point.y))
	{
		angle = 360 - (180 - temp);
		return angle;
	}

	if ((0 < point.x) && (0 > point.y))
	{
		angle = temp;
		return angle;
	}

	if ((0 > point.x) && (0 > point.y))
	{
		angle = 180 - temp;
		return angle;
	}

	return -1;
}

double get_vector_angle(cv::Point v1, cv::Point v2)
{
	double dot = v1.x*v2.x + v1.y * v2.y;
	double det = v1.x*v2.y - v1.y * v2.x;
	double ang = atan2(det,dot) * 180 / 3.14f;
	return ang;
}

double GetDist(cv::Point p1, cv::Point p2)
{
	return sqrt(pow((double)p2.x - (double)p1.x, 2) + pow((double)p2.y - (double)p1.y, 2));
}

cv::Vec3b HSV_U2CV(cv::Vec3b c)
{
	float h = c[0];
	float s = c[1];
	float v = c[2];

	return cv::Vec3b(h / 2, s / 100 * 255, v / 100 * 255);
}

double GetColorDist(cv::Vec3b c1, cv::Vec3b c2)
{
	return sqrt(pow(c2[0] - c1[0], 2) + pow(c2[1] - c1[1], 2) + pow(c2[2] - c1[2], 2));
}

cv::Mat GetHsvMask(cv::Mat src, cv::Vec3b color, double dist)
{
	//std::cout << src << std::endl;

	cv::Mat mask(src.size(), CV_8UC1);
	for (int c = 0; c < src.cols; c++)
	{
		for (int r = 0; r < src.rows; r++)
		{

			cv::Vec3b& col = src.at<cv::Vec3b>(r, c);

			double cdist = GetColorDist(col, color);
			if (cdist < dist)
			{
				mask.at<uchar>(r, c) = 255;
			}
			else
			{
				mask.at<uchar>(r, c) = 0;
			}
		}
	}
	return mask;
}

std::vector<cv::Point> FindLargestContour(cv::Mat src)
{
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	findContours(src, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	int largest_area = 0;
	int largest_contour_index = 0;
	cv::Rect bounding_rect;

	if (contours.size() > 0)
	{

		for (int i = 0; i < contours.size(); i++) // iterate through each contour.
		{
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if (a > largest_area) {
				largest_area = a;
				largest_contour_index = i;                //Store the index of largest contour
				bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}
		}

		//cv::Mat draw = src.clone();
		//drawContours(draw, contours, largest_contour_index, cv::Scalar(255, 0, 0), cv::FILLED, 8);
		//cv::imshow("img_rot", draw);
		//cv::waitKey();

		return contours[largest_contour_index];
	}
	else
	{
		std::vector<cv::Point> emptyVec;
		return emptyVec;
	}
}

cv::Vec3b MeanColorInMask(cv::Mat frame, std::vector<cv::Point> contour)
{
	cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1); 
	std::vector<std::vector<cv::Point> > contours;
	contours.emplace_back(contour);
	drawContours(mask, contours, 0, cv::Scalar(255), cv::FILLED);


	int count = 0;
	cv::Vec3f color = 0;
	for (int r = 0 ; r < mask.rows;r++)
	{
		for (int c = 0; c < mask.cols; c++)
		{
			if (mask.at<uchar>(r, c) > 0)
			{
				auto addColor = frame.at<cv::Vec3b>(r, c);
				color[0] += addColor[0];
				color[1] += addColor[1];
				color[2] += addColor[2];
				count++;
			}

		}
	}
	cv::Vec3b mean = color / count;
	return mean;
}


void FindTangramContour(cv::Mat frame,cv::Mat draw,bool bGray)
{
	cv::Mat gray;

	if (bGray)
	{
		gray = frame.clone();
	}
	else
	{
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);


	}

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);




	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(draw, contours, i, cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
	}

	//cv::imshow("draw", draw);
	//cv::imshow("gray", gray);
	//cv::waitKey();
}
