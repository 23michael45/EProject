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

double GetDist(cv::Point p1, cv::Point p2)
{
	return sqrt(pow((double)p2.x - (double)p1.x, 2) + pow((double)p2.y - (double)p1.y, 2));
}
