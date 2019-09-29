#include "Tangram/TangramData.h"

TangramElementInfo::TangramElementInfo()
{
	
}

void TangramElementInfo::Init(std::vector<cv::Point> contour)
{
	mContour = contour;
	cv::Moments M = cv::moments(contour);
	mCenterPt.x = int(M.m10 / M.m00);
	mCenterPt.y = int(M.m01 / M.m00);

	mArea = cv::contourArea(contour);
	mPerimeter = cv::arcLength(contour, false);

	mBoundingRect = minAreaRect(contour);


	//Point2f P[4];
	//rect.points(P);

	//for (int j = 0; j <= 3; j++)
	//{
	//	line(contourImg, P[j], P[(j + 1) % 4], Scalar(0, 0, 255), 1);
	//	line(contourImg, P[j], P[(j + 1) % 4], Scalar(111), 2);
	//}

	//
	////绘制轮廓的最小外结圆
	//Point2f center; float radius;
	//minEnclosingCircle(contours[i], center, radius);
	//circle(画布1, center, radius, Scalar(255), 2);
}

bool TangramElementInfo::Equal(std::shared_ptr<TangramElementInfo> spInfo)
{
	double threshold = 5.0f;
	if (mType == spInfo->mType)
	{
		if (GetDist(mCenterPt, spInfo->mCenterPt) < threshold)
		{
			if (abs(GetAngle() - spInfo->GetAngle()) < threshold)
			{
				return true;

			}
		}

		return true;
	}
	return false;
}

double TangramElementInfo::GetAngle()
{
	return 0;
}

TriangleElementInfo::TriangleElementInfo()
{

}

double TriangleElementInfo::GetAngle()
{
	return get_point_angle(mBoundingRect.center, mCenterPt);
}

SquareElementInfo::SquareElementInfo()
{

}

double SquareElementInfo::GetAngle()
{
	return mBoundingRect.angle;
}

ParallelElementInfo::ParallelElementInfo()
{

}

double ParallelElementInfo::GetAngle()
{
	cv::Point e0 = mContour[1] - mContour[0];
	cv::Point e1 = mContour[2] - mContour[1];

	auto v2 = cv::Point2f(1.0f, 0.0f);
	auto v0 = cv::Point2f(0.0f, 0.0f);

	double theta = 0;
	if (Magnitude(e0) > Magnitude(e1))
	{
		auto v1 = cv::Point2f(float(e0.x), float(e0.y));
		theta = getAngelOfTwoVector(v1, v2, v0);

		if (theta < 0)
		{
			theta += 180;
		}
		if (theta > 180)
		{
			theta -= 180;
		}
	}
	else
	{

		auto v1 = cv::Point2f(float(e1.x), float(e1.y));
		theta = getAngelOfTwoVector(v1, v2, v0);

		if (theta < 0)
		{
			theta += 180;
		}
		if (theta > 180)
		{
			theta -= 180;
		}
	}
	return theta;
}

double ParallelElementInfo::getAngelOfTwoVector(cv::Point2f &pt1, cv::Point2f &pt2, cv::Point2f &c)
{
	double theta = atan2(pt1.x - c.x, pt1.y - c.y) - atan2(pt2.x - c.x, pt2.y - c.y);
	if (theta > CV_PI)
		theta -= 2 * CV_PI;
	if (theta < -CV_PI)
		theta += 2 * CV_PI;

	theta = theta * 180.0 / CV_PI;
	return theta;
}

double ParallelElementInfo::Magnitude(cv::Point vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

bool ParallelElementInfo::IsFlip()
{
	cv::Point e0 = mContour[1] - mContour[0];
	cv::Point e1 = mContour[2] - mContour[1];

	auto v1 = cv::Point2f((float)e0.x, (float)e0.y);
	auto v2 = cv::Point2f((float)e1.x, (float)e1.y);
	auto v0 = cv::Point2f(0.0f, 0.0f);
	double theta = getAngelOfTwoVector(v1, v2, v0);

	if (Magnitude(e0) > Magnitude(e1))
	{
		if (theta < 90 && theta >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (theta < 90 && theta >= 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}
